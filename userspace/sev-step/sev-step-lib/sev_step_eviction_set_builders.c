#define __USE_GNU
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <stdbool.h>
#include <x86intrin.h>
#include <pthread.h>
#include <time.h>
#include <math.h>


#include "sev_step_eviction_set_builders.h"
#include "sev_step_api.h"
#include "sev_step_pagemap_parser.h"
#include "sev_step_print_macros.h"

#include "sev_step_error_codes.h"
#include "ansi-color-codes.h"

typedef struct {
    /// @brief mapping for corresponding victim_pfn_bits entry in l2_ev_thread_args
    uint8_t** mapping_for_victim;
    /// @brief unaligned mappings that back mapping_for_victim
    uint8_t** unaligned_mappings;
    /// @brief convenience var, showing the remaining empty entries in mapping_for_victim
    uint64_t open_victim_count;
    /// @brief if true, there was an error in one of the other threads
    bool error_in_other_thread;
    pthread_mutex_t lock;
} l2_ev_thread_shared_state;

typedef struct {
    l2_ev_thread_shared_state* shared_state;
    /// @brief we want pages with these pfn bits
    uint64_t* victim_pfn_bits;
    /// @brief length of victim_pfn_bits
    uint64_t victim_count;
    /// @brief bit mask for comparing the paddr of a page with entries in
    /// victim_pfn_bits
    uint64_t victim_pfn_bits_mask;
    /// @brief exit status of thread
    int exit_code;
    /// @brief unique id for this thread. This is NOT the pthread_t handle!
    int id;
} l2_ev_thread_args;

void* find_l2_ev_thread(void* void_args) {

    l2_ev_thread_args* args = (l2_ev_thread_args*)void_args;

    /// @brief allow to hold this many mappings/allocations before freeing any memory 
    uint64_t unused_mappings_max_len = 100 * (0x1ULL << 18);
    uint64_t unused_mappings_idx = 0;
    uint64_t* unused_mappings = malloc(sizeof(uint64_t) * unused_mappings_max_len);
    
    //flf_printf("thread id %d: started\n",args->id);
    while(1) {
        //check if there is still work to do
    
        pthread_mutex_lock(&args->shared_state->lock);
        if( args->shared_state->open_victim_count == 0 ) {
            pthread_mutex_unlock(&args->shared_state->lock);
            //printf("%s:%d: thread %d: no more work to do \n",__FILE__,__LINE__,args->id);
            args->exit_code = SEV_STEP_OK;
            goto exit_free_mappings;
        }
        if( args->shared_state->error_in_other_thread) {
            pthread_mutex_unlock(&args->shared_state->lock);
            printf("%s:%d: thread %d: terminating due to error in other thread \n",__FILE__,__LINE__,args->id);
            args->exit_code = SEV_STEP_OK;
            goto exit_free_mappings;
        }
        pthread_mutex_unlock(&args->shared_state->lock);
        
       
        //if we are here, there is still work to do

        //malloc has better performance than mmap but might not give use aligned mappings, thus we allocate
        //to pages and align manually

        /// @brief unaligned malloc allocation
        uint8_t* candidate_base = malloc(2*4096);
        if( candidate_base == NULL ) {
            flf_printf("thread id %d: malloc failed\n",args->id);
            args->exit_code = SEV_STEP_ERR;
            goto exit_free_mappings;
        }
        /// @brief aligned malloc allocation
        uint8_t* candidate_virt;
        memset(candidate_base,0,2*4096);
        if( ( (uint64_t)candidate_base % 4096) != 0 ) {
            candidate_virt = (uint8_t*)(((uint64_t)(candidate_base + 4096)) & ~0xfff);
        } else {
            candidate_virt = candidate_base;
        }
    
        //get paddr
        uint64_t candidate_phys;
        if(virt_to_phys_user(&candidate_phys, getpid(), (uint64_t)candidate_virt))
        {
            flf_printf("thread id: %d : virt_to_phys_user failed\n", args->id );
            args->exit_code = SEV_STEP_ERR;
            goto exit_free_mappings;
        }
        if( candidate_phys == 0 ) {
            flf_printf("thread id %d: candidate_phys == 0. Are we root?\n",
                args->id
            );
            args->exit_code = SEV_STEP_ERR;
            goto exit_free_mappings;
        }

        //check if pfn bit matches any of the searched pfn bits, otherwise store the allocation
        // in backlog, to avoid getting the same memory on the next allocation

        bool used_candidate = false;
        for(uint64_t victim_idx = 0; victim_idx < args->victim_count && !used_candidate; victim_idx++) {

            uint64_t candidate_bits = candidate_phys & args->victim_pfn_bits_mask;
            uint64_t victim_bits = args->victim_pfn_bits[victim_idx];

            if(candidate_bits == victim_bits ) { //found match => update shared state
                pthread_mutex_lock(&args->shared_state->lock);
                //now that we have the lock, double check that no one else found a match
                //before we update the shared data structure
                if( args->shared_state->mapping_for_victim[victim_idx] == NULL ) {
                    args->shared_state->mapping_for_victim[victim_idx] = candidate_virt;
                    args->shared_state->unaligned_mappings[victim_idx] = candidate_base;
                    args->shared_state->open_victim_count -= 1;
                    used_candidate = true;
                    flf_printf("thread id %d: found mapping for victim idx %ju\n",
                        args->id,victim_idx
                    );
                } 
                pthread_mutex_unlock(&args->shared_state->lock);
            }
        }

        //no match. Don't free memory immediately, to avoid getting the same page
        //again
        if(!used_candidate ) {
            //printf("%s:%d: thread %d: unused mapping\n",__FILE__,__LINE__,args->id);
            if( unused_mappings_idx >= unused_mappings_max_len ) {
                flf_printf("thread id %d: unused_mappings overflow, freeing!\n",
                    args->id
                );
                for(uint64_t j = 0; j < unused_mappings_idx; j++) {
                    free((void*)unused_mappings[j]);
                }
                unused_mappings_idx = 0;
            }
            
            unused_mappings[unused_mappings_idx] = (uint64_t)candidate_base;
            unused_mappings_idx += 1;
            
        }
    } //end of main while loop

    //flf_printf("thread %d: main loop done!\n", args->id);
    exit_free_mappings:
    for(uint64_t i = 0; i < unused_mappings_idx; i++) {
        free((void*)unused_mappings[i]);
    }
    free(unused_mappings);
    if( args->exit_code != SEV_STEP_OK ) {
        pthread_mutex_lock(&args->shared_state->lock);
        args->shared_state->error_in_other_thread = true;
        pthread_mutex_unlock(&args->shared_state->lock);
    }
   
    return (void*)((uintptr_t)args->exit_code);

}

int epyc3_build_l2_eviction_set(uint64_t victim_hpa, uint64_t** eviction_set, uint64_t* eviction_set_len) {
    // Some cache constants
    const int ways = 8;
    //Using only 14 bits would give us and l1d eviction set. Not sure why so many bits are relevant
    const int setBits = 24;
    const int pfnSetBits = setBits - 6; // a page covers 12 bits: 6 bits cache line index and 6 bits  for cache hash function
    const uint64_t pfnSetBitsMask = (((1ull << pfnSetBits) - 1) << (6 + 6));


    const int cacheline_bytes = 64;
    if( ( (victim_hpa % cacheline_bytes) != 0)) {
        printf("%s:%d : epyc3_build_l2_eviction_set:"
            "victim_hpa 0x%lx expected multiple of %d\n",__FILE__,__LINE__,victim_hpa,cacheline_bytes);
        return SEV_STEP_ERR;
    }


	//fprintf(stderr, "victim: phys %jx)\n", victim_hpa);
    int pid = getpid();
    uint64_t victimPfnSetBits = victim_hpa & pfnSetBitsMask;
    uint64_t victimNonPfnSetBits = victim_hpa & 0xfff;
    //fprintf(stderr, "victim PFN bits: phys %jx\n", victimPfnSetBits);

    // Generate eviction set
    (*eviction_set) = malloc(sizeof(uint64_t) * ways);
    (*eviction_set_len) = ways;
    int index = 0;
    const uint64_t pages_in_buffer = 1000;
    uint64_t unused_buffers_max_len = 10000000ULL;
    uint64_t unused_buffers_idx = 0;
    void** unused_buffers = malloc(sizeof(void*) * unused_buffers_max_len);
    while(index < ways)
    {
        // Allocate a page (and some more in case the malloc implementation doesn't directly return pages)
        uint8_t *buffer = mmap(
            NULL,
            pages_in_buffer * 4096,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, 
            -1, 
            0
        );
        if( ((uint64_t)buffer) == 0xffffffffffffffff) {
            printf("mmap failed with %s\n",strerror(errno));
            return SEV_STEP_ERR;
        }
        if( (((uint64_t)buffer) & 0xfff) != 0) {
            printf("%s:%d : "BHRED"ASSERTION ERROR "reset"mmap returned addr 0x%lx without page alignment."
                "This should never happen\n",__FILE__,__LINE__,(uint64_t)buffer);
            free(*eviction_set);
            return SEV_STEP_ERR;
        }

        bool used_buffer = false;
        //printf("%s:%d buffer: 0x%jx\n",__FILE__,__LINE__,buffer);
        for( uint64_t buffer_offset = 0; buffer_offset < pages_in_buffer * 4096; buffer_offset += 4096) {
            uint64_t page = (((uint64_t)buffer + buffer_offset));
            //printf("%s:%d page: 0x%jx\n",__FILE__,__LINE__,page);
            uint64_t pagePhys;
            if(virt_to_phys_user(&pagePhys, pid, page))
            {
                printf("%s:%d : "BHRED" ERROR"reset"Could not get eviction page physical address\n",__FILE__,__LINE__);
                free(*eviction_set);
                return SEV_STEP_ERR;
            }
            if( pagePhys == 0 ) {
                printf("%s:%d : "BHRED" ERROR"reset" pagePhys == 0. Are we root?\n",__FILE__,__LINE__);
                free(*eviction_set);
                return SEV_STEP_ERR;
            }

            //fprintf(stderr, "page: %lx -> %lx\n", page, pagePhys);
            //fprintf(stderr, "page PFN bits: %lx\n", (pagePhys & pfnSetBitsMask));

            // Check whether page contains a useful physical address
            if((pagePhys & pfnSetBitsMask) == victimPfnSetBits)
            {
                (*eviction_set)[index] = page + victimNonPfnSetBits;

                //fprintf(stderr, "(*eviction_set)[%d] = %jx (phys %jx)\n", index, (*eviction_set)[index], pagePhys + victimNonPfnSetBits);

                ++index;
                used_buffer = true;
            } 
        }
        //store reference to buffer, to free it once we found an ev for current target
        //we do not want to free immediately, to avoid getting the same memory on the next
        //allocation
        if( !used_buffer ) {
            if( unused_buffers_idx >= unused_buffers_max_len ) {
                printf("%s:%d : unused_buffers overflow! freeing)",__FILE__,__LINE__);
                for( uint64_t i = 0; i < unused_buffers_idx; i++ ) {
                    munmap(unused_buffers[i],pages_in_buffer * 4096);
                }
                unused_buffers_idx = 0;
            }
            unused_buffers[unused_buffers_idx] = buffer;
            unused_buffers_idx += 1;
        }
       
    }
    printf("%s:%d : freeing %ju unused buffers)",__FILE__,__LINE__,unused_buffers_idx);
    for( uint64_t i = 0; i < unused_buffers_idx; i++ ) {
        munmap(unused_buffers[i],pages_in_buffer * 4096);
    }
    free(unused_buffers);
    return SEV_STEP_OK;
}

void epyc3_free_l2_eviction_set(uint64_t* eviction_set, uint64_t eviction_set_len) {
   for( uint64_t i = 0; i < eviction_set_len; i++ ) {
    munmap((void*)(eviction_set[i]), 4096 );
   }
   free(eviction_set);
}

int epyc3_build_l2_eviction_set_for_lookup_table(uint64_t lookup_table_hpa, uint64_t lookup_table_bytes,
    lookup_table_eviction_set_t* result, uint64_t* way_count) {

    const int cacheline_bytes = 64;
    if( ( (lookup_table_bytes % cacheline_bytes) != 0)) {
        printf("%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:"
            "lookup table has %lu bytes, expected multiple %d\n",__FILE__,__LINE__,lookup_table_bytes,cacheline_bytes);
        return SEV_STEP_ERR;
    }
    int cachelines_in_target = lookup_table_bytes / cacheline_bytes;
    printf("cachelines_in_target = %d\n",cachelines_in_target);

    if( ( (lookup_table_hpa % cacheline_bytes) != 0)) {
        printf("%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:"
            "lookup lookup_table_hpa  0x%lx ist not aligned to cacheline size %d\n",
            __FILE__,__LINE__,lookup_table_hpa,cacheline_bytes);
        return SEV_STEP_ERR;
    }

    //build eviction set for first addr to get eviction_set_len, which is the same regardless of the targeted addr
    uint64_t* eviction_set;
    uint64_t eviction_set_len;
    if( SEV_STEP_ERR == epyc3_build_l2_eviction_set(lookup_table_hpa,&eviction_set,&eviction_set_len) ) {
         printf("%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:"
            "epyc3_build_l2_eviction_set failed for 0x%lx\n",
            __FILE__,__LINE__,lookup_table_hpa);
        return SEV_STEP_ERR;
    }
    uint64_t eviction_set_bytes = sizeof(uint64_t) * eviction_set_len;
    (*way_count) = eviction_set_len;


    //allocate result->eviction_sets and copy first eviction  to it
    result->eviction_sets = malloc(eviction_set_bytes * cachelines_in_target);
    result->eviction_sets_len = cachelines_in_target * eviction_set_len;
    uint64_t offset = 0;
    memcpy(result->eviction_sets + offset, eviction_set, eviction_set_bytes);
    offset += eviction_set_len;
    epyc3_free_l2_eviction_set(eviction_set, eviction_set_len);

    //build eviction sets for the remainder of target and append them to result->eviction_sets
    for( uint64_t target_addr = lookup_table_hpa + cacheline_bytes;
        target_addr < (lookup_table_hpa + lookup_table_bytes); target_addr += cacheline_bytes ) {
        
        printf("%s:%d at target_addr = 0x%lx\n",__FILE__,__LINE__,target_addr);
        if( SEV_STEP_ERR == epyc3_build_l2_eviction_set(target_addr,&eviction_set,&eviction_set_len) ) {
            printf("%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:"
                "epyc3_build_l2_eviction_set failed for 0x%lx\n",
                __FILE__,__LINE__,target_addr);
            //free eviction sets allocated so far
            for(uint64_t i = 0; i < offset; i += eviction_set_len) {
                epyc3_free_l2_eviction_set(result->eviction_sets+i,eviction_set_len);
            }
            epyc3_free_l2_eviction_set(eviction_set,eviction_set_len);
            return SEV_STEP_ERR;
        }

        memcpy(result->eviction_sets + offset, eviction_set, eviction_set_bytes);
        epyc3_free_l2_eviction_set(eviction_set, eviction_set_len);
        offset += eviction_set_len;
            
    }
    

    return SEV_STEP_OK;
}


int epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse(uint64_t* lookup_table_hpas,uint64_t lookup_table_hpas_len, uint64_t lookup_table_bytes,
    lookup_table_eviction_set_t* result, uint8_t*** base_mappings,uint64_t* base_mappings_len ,uint64_t way_count) {

    int exit_code = SEV_STEP_OK;

    //check that lookup table is aligned to 64 byte and size is multiple of 64 byte and that it is not larger than one page

    const int cacheline_bytes = 64;
    if( ( (lookup_table_bytes % cacheline_bytes) != 0)) {
        flf_printf("lookup table has %lu bytes, expected multiple %d\n",
            lookup_table_bytes,cacheline_bytes
        );
        exit_code = SEV_STEP_ERR;
        goto exit;
    }
    uint64_t cachelines_in_target = lookup_table_bytes / cacheline_bytes;
    printf("cachelines_in_target = %ju\n",cachelines_in_target);

    {
        //sanity check lookup_table_hpas_len
        uint64_t expected_hpas = ceil(((lookup_table_hpas[0] & 0xfff) + lookup_table_bytes) / 4096.0);
        if( expected_hpas != lookup_table_hpas_len ) {
            flf_printf("Table starts at offset 0x%jx and spans 0x%jx bytes. => %f pages\n",
                lookup_table_hpas[0] & 0xfff,
                lookup_table_bytes,
                ((double)(lookup_table_hpas[0] & 0xfff) + lookup_table_bytes) / 4096.0
            );
            flf_printf("based on offset + table length, we should have %ju hpas in lookup table but lookup_table_hpas_len is %ju \n",expected_hpas, lookup_table_hpas_len)
            exit_code = SEV_STEP_ERR;
            goto exit;
        }
    }

    if( ( (lookup_table_hpas[0] % cacheline_bytes) != 0)) {
        flf_printf("lookup lookup_table_hpa[0]  0x%lx ist not aligned to cacheline size %d\n",
            lookup_table_hpas[0],cacheline_bytes
        );
        exit_code = SEV_STEP_ERR;
        goto exit;
    }

    //
    // Get way_count addrs that collide with the pfn bits of lookup_table_hpa
    //

    const int setBits = 24;
    const int pfnSetBits = setBits - 6; // a page covers 12 bits: 6 bits cache line index and 6 bits  for cache hash function
    const uint64_t pfnSetBitsMask = (((1ull << pfnSetBits) - 1) << (6 + 6));

    //for each physical page touched by the lookup table, we need way_count colliding addrs
    //store them in flat array here
    uint64_t base_ev_pfn_bits_len = way_count * lookup_table_hpas_len;
    uint64_t* base_ev_pfn_bits = malloc( sizeof(uint64_t) * base_ev_pfn_bits_len);
    //flf_printf("base_ev_pfn_bits_len %ju\n", base_ev_pfn_bits_len);
    for( uint64_t hpa_idx = 0; hpa_idx < lookup_table_hpas_len; hpa_idx++) {
        for( uint64_t i = 0; i < way_count; i++ ) {
            base_ev_pfn_bits[ (hpa_idx * way_count) + i] = lookup_table_hpas[hpa_idx] & pfnSetBitsMask;
            //flf_printf("base_ev_pfn_bits[%ju] = 0x%jx\n",(hpa_idx * way_count) + i, base_ev_pfn_bits[ (hpa_idx * lookup_table_hpas_len) + i] );
        }
    }
  

    //Prepare thread args
    l2_ev_thread_shared_state shared_state = {
        .mapping_for_victim = calloc(way_count * lookup_table_hpas_len, sizeof(uint8_t*)),
        .unaligned_mappings = calloc(way_count * lookup_table_hpas_len, sizeof(uint8_t*)),
        .open_victim_count = way_count * lookup_table_hpas_len,
        .error_in_other_thread = false,
    };
    pthread_mutex_init(&shared_state.lock,NULL);

    const uint64_t thread_count = 20;
    pthread_t* thread_ids = malloc( sizeof(pthread_t) * thread_count );
    l2_ev_thread_args* thread_args = malloc(sizeof(l2_ev_thread_args) * thread_count);
    for(uint64_t i = 0; i < thread_count; i++) {
        thread_args[i].shared_state = &shared_state;
        thread_args[i].victim_pfn_bits = base_ev_pfn_bits;
        thread_args[i].victim_count = base_ev_pfn_bits_len,
        thread_args[i].victim_pfn_bits_mask = pfnSetBitsMask;
        thread_args[i].exit_code = SEV_STEP_OK;
        thread_args[i].id = i;

    }

    printf("Starting search threads\n");
    time_t start = time(NULL);
    // Start threads
    for(uint64_t i = 0; i < thread_count; i++) {
        pthread_create(&thread_ids[i], NULL, find_l2_ev_thread, (void*)(&thread_args[i]));
    }

    printf("Waiting for threads to finish\n");
    // Wait for all threads to finish
    for(uint64_t i = 0; i < thread_count; i++) {
        //printf("Waiting for threads to finish [%ju/%ju]\n",i,thread_count);
        if( SEV_STEP_OK != pthread_join(thread_ids[i],NULL) ) {
            printf("%s:%d: thread %ju terminated with error status\n",
            __FILE__,__LINE__,i);
        } /*else {
             printf("%s:%d: thread %ju finished\n",
            __FILE__,__LINE__,i);
        }*/
    }
    free(thread_ids);
    free(thread_args);
    printf("All threads joined after %lu seconds\n",time(NULL)-start);

    if(shared_state.error_in_other_thread) {
        printf("%s:%d at least one thread failed, aborting\n",__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    printf("All threads finished successfully\n");

    //
    // Build eviction set by combining the pages colliding with the pfn of lookup_table_addr with the page offsets
    // We reuse the pages colliding with the pfn for each offset
    //

    flf_printf("cacheline in target %ju -> allocating %ju entries for ev\n", cachelines_in_target,cachelines_in_target*way_count);
    result->eviction_sets = malloc( sizeof(uint64_t) * cachelines_in_target * way_count );
    result->eviction_sets_len = cachelines_in_target * way_count;

    *base_mappings_len = way_count * lookup_table_hpas_len;
    flf_printf("hpas backing the table: %ju -> allocating %ju entries for base mappings\n", lookup_table_hpas_len, *base_mappings_len);
    (*base_mappings) = malloc( sizeof(uint8_t*) * (*base_mappings_len));
    for( uint64_t i = 0; i < *base_mappings_len; i++ ) {
        (*base_mappings)[i] = (uint8_t*)shared_state.unaligned_mappings[i];
    }

    uint64_t ev_idx = 0;
    uint64_t offset_from_page_start = lookup_table_hpas[0] & 0xfff;
    for(uint64_t cacheline_idx = 0; cacheline_idx < cachelines_in_target; cacheline_idx++) {
        //get the index of the hpa in lookup_table_hpas that this entry is on
        //flf_printf("ping cacheline_idx = %ju\n",cacheline_idx);
        uint64_t hpa_idx = floor(( (lookup_table_hpas[0] & 0xfff ) + (cacheline_idx * 64)) / 4096.0);
        flf_printf("hpa_idx = %ju\n",hpa_idx);
        //uint64_t offset_from_page_start = ( lookup_table_hpas[hpa_idx] & 0xfff ) + (cacheline_idx * 64);
        //flf_printf("offset_from_page_start %ju\n",offset_from_page_start);
        //Since we checked that the table is cache line aligned and pages are a multiple of cacheline size,
        //it cannot happen, that an eviction set crosses a page boundary
        for( uint64_t _way = 0; _way < way_count; _way++) {
            //flf_printf("ev_idx = %ju. idx into mapping_for_victim = %ju\n",ev_idx, (hpa_idx * way_count) + _way);
            result->eviction_sets[ev_idx] = ((uint64_t)shared_state.mapping_for_victim[(hpa_idx * way_count) + _way]) + offset_from_page_start;
            ev_idx += 1;
        }

        offset_from_page_start = (offset_from_page_start + 64) % 4096;
    }


    cleanup:
    free(base_ev_pfn_bits);
    free(shared_state.mapping_for_victim);
    free(shared_state.unaligned_mappings);
    pthread_mutex_destroy(&shared_state.lock);

    exit:
    return exit_code;
}

int split_large_ev_into_separate_luts(lookup_table_t combined_lut, lookup_table_eviction_set_t* combined_ev, 
    lookup_table_t* sub_luts, uint64_t sub_luts_len, lookup_table_eviction_set_t** sub_evs, uint64_t way_count) {


    flf_printf("Sanity checking params...\n");
    //
    //sanity check sub_luts definitions against combined_lut
    //

    if( sub_luts_len <= 1 ) {
        flf_printf("you must provide at least two sub lookup tables for this to make sense\n");
        return SEV_STEP_ERR;
    }
    if( combined_lut.base_vaddr_table != sub_luts[0].base_vaddr_table ) {
        flf_printf("combined_lut and sub_luts[0] must start at same vaddr!");
        return SEV_STEP_ERR;
    }
    if( (combined_lut.base_vaddr_table % 64) != 0) {
        flf_printf("combined_lut.base_vaddr_table not cacheline aligned ");
        return SEV_STEP_ERR;
    }
    if( (combined_lut.table_bytes % 64) != 0) {
        flf_printf("combined_lut.table_bytes a multiple of cacheline size");
        return SEV_STEP_ERR;
    }


    //check that sub luts cover the combined lut and that each sub lut is cacheline aligned
    uint64_t sub_luts_range_start = sub_luts[0].base_vaddr_table;
    uint64_t sub_luts_range_end = sub_luts_range_start + sub_luts[0].table_bytes;
    for(uint64_t idx = 1; idx < sub_luts_len; idx++ ) {
        //shorthand
        lookup_table_t* sl = &sub_luts[idx];

        if( sl->base_vaddr_table != sub_luts_range_end ) {
            flf_printf("sub_luts[%ju] should start at 0x%jx but starts at 0x%jx\n",idx, sub_luts_range_end, sl->base_vaddr_table );
            return SEV_STEP_ERR;
        }
        if( (sl->table_bytes % 64) != 0 ) {
            flf_printf("sub_luts[%ju] : size not multiple of cacheline\n",idx );
            return SEV_STEP_ERR;
        }
        sub_luts_range_end += sl->table_bytes;
    }

    if( sub_luts_range_end != (combined_lut.base_vaddr_table + combined_lut.table_bytes)) {
        flf_printf("sub_luts should span from 0%jx to 0x%jx but end at 0x%jx, i.e. they do not cover the combined_lut\n",
            combined_lut.base_vaddr_table,
            combined_lut.base_vaddr_table + combined_lut.table_bytes,
            sub_luts_range_end
        );
        return SEV_STEP_ERR;
    }

    flf_printf("Params sanity checks done\n");
    //
    // Redistribute the entries from combined_ev onto these "sub" lookup tables
    //

    flf_printf("Allocating %ju entries for sub_evs\n",sub_luts_len);
    (*sub_evs) = malloc(sizeof(lookup_table_eviction_set_t) * sub_luts_len);

    for(uint64_t sub_ev_idx = 0, main_ev_idx = 0; sub_ev_idx < sub_luts_len; sub_ev_idx++ ) {
        //define shorthand
        lookup_table_eviction_set_t* sub_ev = (*sub_evs)+sub_ev_idx;

        //each sub_ev gets this many entries from cache_attack.crypto_ft_tab_eviction_set
        sub_ev->eviction_sets_len = way_count * ( sub_luts[sub_ev_idx].table_bytes/64);
        flf_printf("sub_ev 0x%jx, sub_ev_idx %ju, allocating %ju entries for sub_ev->eviction_sets\n",
            (uint64_t)sub_ev,sub_ev_idx, sub_ev->eviction_sets_len
        );
        sub_ev->eviction_sets = malloc(sizeof(uint64_t) * sub_ev->eviction_sets_len);
        //flf_printf("sub_ev_idx %ju: all entries:\n",sub_ev_idx);
        //copy entries
        for(uint64_t i = 0; i < sub_ev->eviction_sets_len; i++ ) {
            sub_ev->eviction_sets[i] = combined_ev->eviction_sets[main_ev_idx];
            //printf("\t%jx\n",sub_ev->eviction_sets[i]);
            main_ev_idx += 1;
        }
    }

    return SEV_STEP_OK;
}

void free_sub_evs(lookup_table_eviction_set_t* sub_evs, uint64_t sub_evs_len ) {
    for(uint64_t idx = 0; idx < sub_evs_len; idx++ ) {
        free(sub_evs[idx].eviction_sets);
    }
    free(sub_evs);
}
void epyc3_free_reuse_lookup_table_eviction_set(lookup_table_eviction_set_t evs, uint8_t** base_mappings, uint64_t base_mappings_len) {
    for( uint64_t i = 0; i < base_mappings_len; i++ ) {
        free(base_mappings[i]);
    }
    free(base_mappings);
    free(evs.eviction_sets);
}

void epyc3_free_lookup_table_eviction_set_t(lookup_table_eviction_set_t evs) {
    for( uint64_t i = 0; i < evs.eviction_sets_len ; i++) {
        munmap((void*)(evs.eviction_sets[i]), 4096 );
    }
    free(evs.eviction_sets);
}