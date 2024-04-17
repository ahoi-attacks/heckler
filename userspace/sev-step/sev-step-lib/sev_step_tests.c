#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>

#include "sev_step_error_codes.h"
#include "sev_step_eviction_set_builders.h"
#include "ansi-color-codes.h"
#include "sev_step_pagemap_parser.h"
#include "sev_step_cache_attack_log.h"
#include "sev_step_print_macros.h"

typedef struct {
    char* name;
    int (*test_function)(void* args);
    void* args;
} test_t;


int test_alloc_free_epyc3_build_l2_eviction_set(void* args) {
    (void)args;

    int exit_code = SEV_STEP_OK;

    const int victim_table_bytes = 4096;
    uint8_t* victim_table = mmap(NULL,victim_table_bytes,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    uint8_t* victim_ptr = victim_table + 3 * 64;
    uint64_t* eviction_set;
    uint64_t eviction_set_len;

    printf("Allocating eviction set\n");
    if( SEV_STEP_ERR == epyc3_build_l2_eviction_set((uint64_t)victim_ptr,&eviction_set,
        &eviction_set_len) ) {
        printf("%s:%d : epyc3_build_l2_eviction_set:" BHRED " FAILED\n"reset,__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    printf("Success. Freeing eviction set\n");

    epyc3_free_l2_eviction_set(eviction_set, eviction_set_len);

cleanup:
    if( victim_table != NULL ) {
        munmap(victim_table,victim_table_bytes);
    }
    return exit_code;
}


int test_alloc_free_epyc3_build_l2_eviction_set_for_lookup_table(void* args) {
    (void)args;
    int exit_code = SEV_STEP_OK;

    void* victim_table = mmap(NULL,4096,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS | MAP_POPULATE, 0, 0);
    lookup_table_t lut = {
        .base_vaddr_table = (uint64_t)victim_table,
        .table_bytes = 3 * 64,
    };

    lookup_table_eviction_set_t result;
    uint64_t way_count;

    uint64_t paddr;
    if( virt_to_phys_user(&paddr,getpid(),lut.base_vaddr_table) ) {
        printf("%s:%d virt_to_phys_user failed\n",__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }
    if( paddr == 0 ) {
        printf("%s:%d virt_to_phys_user returned paddr 0, this should not happen\n",__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }
    printf("Allocating Eviction set for table at paddr 0x%lx...\n",paddr);
    if( SEV_STEP_ERR == epyc3_build_l2_eviction_set_for_lookup_table(paddr,lut.table_bytes,&result,&way_count) ) {
        printf("%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:" BHRED " FAILED\n"reset,__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    printf("Success. Freeing eviction set...\n");
    epyc3_free_lookup_table_eviction_set_t(result);

    cleanup:
    munmap(victim_table,4096);
    return exit_code;
}

int test_epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse_across_page_boundary(void* args) {
    (void)args;
    int exit_code = SEV_STEP_OK;

    uint8_t* victim_table = mmap(NULL,2 * 4096,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS | MAP_POPULATE, 0, 0);
    lookup_table_t lut = {
        .base_vaddr_table = (uint64_t)victim_table + 0xc0,
        .table_bytes = 4096,
    };

    uint64_t way_count = 8;

    uint64_t paddrs[2];
    uint64_t base_vaddrs[2] = {(uint64_t)lut.base_vaddr_table,(uint64_t)(victim_table + 4096)};

    for(uint64_t idx = 0; idx < (sizeof(base_vaddrs)/sizeof(*base_vaddrs)); idx++ ) {
        if( virt_to_phys_user(paddrs+idx,getpid(),base_vaddrs[idx]) ) {
            printf("%s:%d virt_to_phys_user failed\n",__FILE__,__LINE__);
            exit_code = SEV_STEP_ERR;
            goto cleanup;
        }
        if( paddrs[idx] == 0 ) {
            printf("%s:%d virt_to_phys_user returned paddr 0, this should not happen\n",__FILE__,__LINE__);
            exit_code = SEV_STEP_ERR;
            goto cleanup;
        }
    }
    
    lookup_table_eviction_set_t ev;
    uint8_t** base_mappings;
    uint64_t base_mappings_len;
    printf("Allocating Eviction set\n");
    int err = epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse(
        paddrs,
        sizeof(paddrs)/sizeof(*paddrs),
        lut.table_bytes,
        &ev,
        &base_mappings,
        &base_mappings_len,
        way_count
    );
    if( SEV_STEP_ERR == err ) {
        printf("%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:" BHRED " FAILED\n"reset,__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    flf_printf("base mappings are:\n");
    for(uint64_t idx = 0; idx < base_mappings_len; idx++) {
        printf("\t 0x%jx\n", (uint64_t)base_mappings[idx]);
    }

    flf_printf("eviction set:\n");
    for(uint64_t idx = 0; idx < ev.eviction_sets_len; idx++) {
        printf("\t %jx\n",ev.eviction_sets[idx]);
    }

    printf("Success. Freeing eviction set...\n");
    epyc3_free_reuse_lookup_table_eviction_set(ev,base_mappings,base_mappings_len);

    cleanup:
    munmap(victim_table,2*4096);
    return exit_code;
}

int test_cache_attack_log_workflow(void* args) {
    (void)args;
    int exit_code = SEV_STEP_OK;
    lookup_table_t luts[] = {
        {
            .base_vaddr_table = 0x1000,
            .table_bytes = 16 * 64,
        },
    };

    flf_printf("calling cache_attack_log_new_log")
    cache_attack_logfile_t* cache_attack_log;
    cache_attack_log = cache_attack_log_new_log(
        luts,
        1,
        "perf name todo",
        8
    );
    if( cache_attack_log == NULL ) {
        flf_printf("cache_attack_log_new_log failed");
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    const uint64_t len = 3;
    uint64_t timings[] = {1,2,3};
    uint64_t perf_diffs[] = {0,1,0};
    int err = cache_attack_log_add_trace(
        cache_attack_log,
        0,
        timings,
        perf_diffs,
        len,
        0,
        0,
        0,
        NULL
    );
    if( err == SEV_STEP_ERR ) {
        flf_printf("cache_attack_log_add_trace failed");
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }


    if( SEV_STEP_ERR == cache_attack_log_to_file(cache_attack_log,"/tmp/sev_step_json_test") ) {
        flf_printf("cache_attack_log_to_file failed");
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    cleanup:
    cache_attack_log_free(cache_attack_log);
    return exit_code;
}

int main() {

    test_t tests[] = {
        {
            .name = "test_alloc_free_epyc3_build_l2_eviction_set",
            .test_function = test_alloc_free_epyc3_build_l2_eviction_set,
            .args = NULL,
        },
        {
            .name = "test_alloc_free_epyc3_build_l2_eviction_set_for_lookup_table",
            .test_function = test_alloc_free_epyc3_build_l2_eviction_set_for_lookup_table,
            .args = NULL,
        },
        {
            .name = "test_cache_attack_log_workflow",
            .test_function = test_cache_attack_log_workflow,
            .args = NULL,
        },
        {
            .name = "test_epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse_across_page_boundary",
            .test_function = test_epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse_across_page_boundary,
            .args = NULL,
        },
    };


    int tests_len = sizeof(tests)/sizeof(test_t);
    for( int test_idx = 0; test_idx < tests_len; test_idx++) {
        test_t test = tests[test_idx];
        printf("[%d/%d] - Running test %s...\n",test_idx+1,tests_len,test.name);
        if( SEV_STEP_ERR == test.test_function(test.args)) {
            printf("\nTest %s" BRED "FAILED\n" reset,test.name);
            return 1;
        }
        printf("\n[%d/%d] - Test %s" BHGRN " SUCCEEDED\n" reset,test_idx+1,tests_len,test.name);

    }
    printf(BHGRN "\n All Tests SUCCEEDED\n Happy Pwning !\n" reset);
}