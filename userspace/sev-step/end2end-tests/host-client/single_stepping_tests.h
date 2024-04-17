#ifndef SINGLE_STEPPING_TESTS_H
#define SINGLE_STEPPING_TESTS_H

#include <stdio.h>
#include <json-c/json.h>

#include "../../sev-step-lib/ansi-color-codes.h"
#include "../../sev-step-lib/sev_step_api.h"
#include "my-error-codes.h"
#include "vm-server-client.h"


typedef struct {
    int timer_value;
    //value between 0 and 1. If strictly more than this fraction of zero steps, mark test as failed.
    float zero_step_abort_fraction;
    //like zero_step_abort_fraction but for multi steps
    float multi_step_abort_fraction;
    //if true, we expect step events to contain rip data and check them against the expected offsets
    bool check_debug_rip;
    // json filename to save stepping to
    char* filename;
    //if true, save single stepping to json file
    bool save_logs_to_file;
    // instruction slide to use
    victim_program_t instruction_slide;
} test_single_step_nop_slide_args_t;
int test_single_step_nop_slide(char* format_prefix,void* void_args) {
     //
    //Initialize API connection and prepare tracked pages on server/vm side
    //
    test_single_step_nop_slide_args_t* args = (test_single_step_nop_slide_args_t*)void_args;

    json_object *root, *steps = NULL;
    if (args->save_logs_to_file) {
        root = json_object_new_object();
        steps = json_object_new_object();
        json_object_object_add(root, "steps", steps);
       
    }

        usp_poll_api_ctx_t ctx;
    int res = HOST_CLIENT_SUCCESS;
    usp_event_type_t event_type;
    void *event_buffer = NULL;

    bool api_open = false;
    printf("%sInitializing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,args->check_debug_rip)) {
        printf("%s" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        return HOST_CLIENT_ERROR;
    }
    printf("%sAPI initialization done!\n",format_prefix);
    api_open = true;

    victim_program_data_t* victim_program_data = malloc(sizeof(victim_program_data_t)); 
    printf("%scalling vm_server_single_step_victim_init to prepare scenario\n",format_prefix);
    if (HOST_CLIENT_ERROR == vm_server_single_step_victim_init(args->instruction_slide, victim_program_data))
    {
        printf("%svm_server_single_step_victim_init " BRED "FAILED\n" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%s Victim program: GPA is 0x%lx and VADDR is 0x%lx\n",
           format_prefix, victim_program_data->gpa, victim_program_data->vaddr);

    //
    // Exec track gpa of victim function and signal vm server to start victim program async
    // Then, enter main event loop
    //

    printf("%stracking gpa 0x%lx with mode %s\n",format_prefix,victim_program_data->gpa,tracking_mode_to_string(KVM_PAGE_TRACK_EXEC));
    if( SEV_STEP_OK != track_page(&ctx,victim_program_data->gpa,KVM_PAGE_TRACK_EXEC) ) {
        printf("%sinitial track page for 0x%lx" BRED " FAILED\n" reset,format_prefix,victim_program_data->gpa);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%scalling vm_server_single_step_victim_start\n",format_prefix);
    if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_start(args->instruction_slide) ) {
        printf("%svm_server_single_step_victim_start " BRED "FAILED\n" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%sentering main event loop\n",format_prefix);




    //if true, the last page fault event belonged to the target pages
    bool on_victim_pages = false;

    uint64_t zero_steps_on_target = 0;
    uint64_t single_steps_on_target = 0;
    uint64_t multi_steps_on_target = 0;

    /*We expect the victim program do be done after this many events. Contains some slack to account for scheduling and zero steps
    victim_program_data->expected_offsets_len is the exact number of instructions/single step events for the target
    */
    int upper_event_thresh = 4 * victim_program_data->expected_offsets_len;
    //if true, we have single stepped through the whole target program successfully
    bool finished_single_stepping_target = false;

    for( int current_event_idx = 0; current_event_idx < upper_event_thresh && !finished_single_stepping_target; current_event_idx++) {

        //printf("%sWaiting next event, event_idx=%d\n",format_prefix,current_event_idx);
        if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer) ) {
            printf("%susp_block_until_event" BRED " FAILED\n" reset,format_prefix);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        
        if( event_type == PAGE_FAULT_EVENT ) {
            usp_page_fault_event_t* pf_event = (usp_page_fault_event_t*)event_buffer;
            printf("%s Pagefault Event: {GPA:0x%lx}\n",format_prefix,pf_event->faulted_gpa);

            if( on_victim_pages ) {
                if( pf_event->faulted_gpa == victim_program_data->gpa ) {
                    printf("%s on_victim_pages=true but got fault for victim page! This should not happen\n",format_prefix);
                    res = HOST_CLIENT_ERROR;
                    goto cleanup;
                } else {
                    printf("%s Left victim pages with fault at GPA 0x%lx. Disabling single stepping and re-tracking victim pages\n",
                        format_prefix,pf_event->faulted_gpa);
                    if( SEV_STEP_OK != disable_single_stepping(&ctx) ) {
                        printf("%sfailed to disable single stepping\n",format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    //track only the target page
                    if( SEV_STEP_OK != untrack_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
                        printf("%suntrack_all_pages failed\n",format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( SEV_STEP_OK != track_page(&ctx,victim_program_data->gpa,KVM_PAGE_TRACK_EXEC)) {
                        printf("%sfailed to to track victim page\n",format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    on_victim_pages = false;
                }
            } else { //currently not on victim pages
                if( pf_event->faulted_gpa == victim_program_data->gpa ) {
                    printf("%s entering victim pages. enabling single stepping (timer 0x%x) and tracking all but the target page\n",
                        format_prefix,args->timer_value);
                    
                    //track all pages but the target page
                    if( SEV_STEP_OK != track_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
                        printf("%strack_all_pages failed\n",format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( SEV_STEP_OK != untrack_page(&ctx,victim_program_data->gpa,KVM_PAGE_TRACK_EXEC)) {
                        printf("%suntrack_page failed\n",format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    if( SEV_STEP_OK != enable_single_stepping(&ctx,args->timer_value,&victim_program_data->gpa,1) ) {
                        printf("%sfailed to enable single stepping\n",format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }

                    on_victim_pages = true;
                } else {
                    printf("currently not on victim pages and page fault is not on victim pages : nothing to do\n");
                }
            }
        } else if( event_type == SEV_STEP_EVENT) {
            sev_step_event_t* step_event = (sev_step_event_t*)event_buffer;
            //print_single_step_event(format_prefix,step_event);

            if(args->save_logs_to_file){
                json_object *step = json_object_new_object();
                json_object_object_add(step, "counted instructions", json_object_new_int(step_event->counted_instructions));
                if( step_event->counted_instructions > 0) {
                    //json_object_object_add(step, "VM exit", json_object_new_int64(step_event->tsc_vm_exit));
                    //json_object_object_add(step, "VM enter", json_object_new_int64(step_event->tsc_vm_enter));
                    json_object_object_add(step, "Latency", json_object_new_int64(step_event->tsc_latency));
                }
		char s[8];
		sprintf(s, "%d", current_event_idx);
                json_object_object_add(steps, s, step);
            }

            if( !on_victim_pages ) {
                printf("%s on_victim_pages=false but single step event. This should not happen\n",format_prefix);
                res = HOST_CLIENT_ERROR;
                goto cleanup;
            }

            switch (step_event->counted_instructions) {
            case 0:
                zero_steps_on_target += 1;
                break;
            case 1:
                single_steps_on_target += 1;

                if( args->check_debug_rip && (single_steps_on_target < victim_program_data->expected_offsets_len) ) {
                    if( !step_event->is_decrypted_vmsa_data_valid)  {
                        printf("%s"BHRED"NO VMSA DATA"reset" args->check_debug_rip was specified but VMSA data not valid\n",format_prefix);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    uint64_t expected_vaddr = victim_program_data->vaddr + victim_program_data->expected_offsets[single_steps_on_target-1];
                    uint64_t rip = step_event->decrypted_vmsa_data.register_values[VRN_RIP];
                    if( (rip >> 12) != (expected_vaddr >> 12 )) {
                         printf("%s"BHRED "vaddr check failed: page part of vaddr is wrong, single_steps_on_target = %ju, expected vaddr 0x%lx got 0x%lx\n" reset,
                            format_prefix,single_steps_on_target,expected_vaddr,rip);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    }
                    if( rip != expected_vaddr ) {
                        printf("%s"BHRED "vaddr check failed:  single_steps_on_target = %ju,"
                            " page part correct, expected page offset 0x%lx got 0x%lx\n" reset,
                            format_prefix,single_steps_on_target,expected_vaddr & 0xfff,rip & 0xfff);
                        res = HOST_CLIENT_ERROR;
                        goto cleanup;
                    } 
                    printf("%s step event is at expected vaddr\n",format_prefix);
                    
                }

                if( single_steps_on_target > victim_program_data->expected_offsets_len ) {
                    printf("%sFinished single stepping target program !\n",format_prefix);
                    finished_single_stepping_target = true;
                }
               
                break;
            default:
                printf("%s" BHRED "MULTISTEP"reset,format_prefix);
                res = HOST_CLIENT_ERROR;
                goto cleanup;
                break;
            }       
        } else {
            printf("%s Unexpected event type! Aborting\n",format_prefix);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        printf("%ssending ack for event_idx %d\n",format_prefix,current_event_idx);
        usp_ack_event(&ctx);
        free_usp_event(event_type, event_buffer);
        event_buffer = NULL;
    } //end of main event loop

    printf("%sClosing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        res = HOST_CLIENT_ERROR;
        api_open = false;
        goto cleanup;
    } else {
        printf("%sAPI closed!\n",format_prefix);
    }
    api_open = false;

    print_step_histogram_args_t histogram_args = {
        .want_runs = 1,
        .timer_value = args->timer_value ,
        .total_zero_step_count = zero_steps_on_target,
        .total_single_step_count = single_steps_on_target,
        .total_multi_step_count = multi_steps_on_target,
        .total_step_count = zero_steps_on_target + single_steps_on_target + multi_steps_on_target,
    };
    print_step_histogram(format_prefix,histogram_args);

    if(args->save_logs_to_file){
        json_object *histogram = json_object_new_object();

        int total_steps = zero_steps_on_target + single_steps_on_target;
        double zero_step_fraction = (double)zero_steps_on_target/total_steps;
        double single_step_fraction = (double)single_steps_on_target/total_steps;

        json_object_object_add(histogram, "timer value", json_object_new_int(args->timer_value));
        json_object_object_add(histogram, "zero steps", json_object_new_int(zero_steps_on_target));
        json_object_object_add(histogram, "single steps", json_object_new_int(single_steps_on_target));
        json_object_object_add(histogram, "total steps", json_object_new_int(total_steps));
        json_object_object_add(histogram, "zero step fraction (%)", json_object_new_double(zero_step_fraction*100));
        json_object_object_add(histogram, "single step fraction (%)", json_object_new_double(single_step_fraction*100));

        json_object_object_add(root, "statistics", histogram);

        json_object_to_file(args->filename, root);
        json_object_put(root);
    }
    
    double zero_step_fraction = (double)zero_steps_on_target/histogram_args.total_step_count;
    double multi_step_fraction = (double)multi_steps_on_target/histogram_args.total_step_count;
    if( zero_step_fraction > args->zero_step_abort_fraction) {
        printf("%s" BHRED "more than %0.1f %% zero steps! Test failed.\n" reset, format_prefix,args->zero_step_abort_fraction*100);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    };

    if( multi_step_fraction > args->multi_step_abort_fraction) {
        printf("%s" BHRED "more than %0.1f %% multi steps! Test failed.\n" reset, format_prefix,args->multi_step_abort_fraction*100);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    };

    if( !finished_single_stepping_target ) {
        printf("%s" BHRED "Did not finish target!. Wanted %lu single step events on target, got only %ju\n" reset,
            format_prefix,victim_program_data->expected_offsets_len,single_steps_on_target);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    };

    //
    // interact with the vm to ensure that it is not stuck
    //
    bool vm_alive = false;
    for( int retries = 1; !vm_alive && retries >= 0; retries--) {
        victim_program_data_t* _dummy = malloc(sizeof(victim_program_data_t));
        printf("%ssending dummy request to vm to ensure that it is alive and well\n",format_prefix);
        if (HOST_CLIENT_ERROR == vm_server_single_step_victim_init(args->instruction_slide, _dummy))
        {
            printf("%sdummy request " BRED "FAILED. %d retries remaining\n" reset,format_prefix,retries);
        }
        else
        {
            vm_alive = true;
        }
        free_victim_program_data_t(_dummy);
    }
   if( vm_alive ) {
        printf("%s VM seems to be alive and well\n",format_prefix);
   } else {
        printf("%svm is stuck\n",format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
   }

cleanup:
    if( api_open ) {
        printf("%sClosing API...\n",format_prefix);
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        res = HOST_CLIENT_ERROR;
        } else {
        printf("%sAPI closed!\n",format_prefix);
        }
    }

    if( event_buffer != NULL ) {
        free_usp_event(event_type,event_buffer);
    }
    free_victim_program_data_t(victim_program_data);

    return res;
}

typedef struct  {
    //value for apic timer
    int timer_value;
    //amount of steps that we want
    int want_steps;

} single_step_simple_args_t;
typedef struct {
    uint32_t stepsize;
} single_step_simple_step_event_t;
typedef struct {
    single_step_simple_step_event_t* events;
    int single_steps;
    int zero_steps;
    int multi_steps;
} single_step_simple_results_t;

single_step_simple_results_t* alloc_single_step_simple_results_t(int want_steps) {
    single_step_simple_results_t* r = malloc(sizeof(single_step_simple_results_t));
    r->multi_steps = 0;
    r->single_steps = 0;
    r->zero_steps = 0;
    r->events = malloc(sizeof(single_step_simple_step_event_t) * want_steps);
    return r;
};

void free_single_step_simple_results_t(single_step_simple_results_t* p) {
    free(p->events);
    free(p);
}

/**
 * @brief Enables single stepping and logs the amount of steps specified in @args. Results are recored in @results.
 * Intended as common building block for test functions
 * 
 * @param format_prefix 
 * @param args caller allocated. configures behaviour
 * @param results caller allocated. on HOST_CLIENT_SUCCESS this contains statistics about the steps
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int single_step_simple(char* format_prefix,single_step_simple_args_t* args,single_step_simple_results_t* results ) {

    //
    // Setup API + Start Single Stepping
    //

    usp_poll_api_ctx_t ctx;
    //TODO: make configurable
    int res = HOST_CLIENT_SUCCESS;
    bool single_stepping_enabled = false;
    bool api_open = false;

    printf("%sInitializing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,false)) {
        printf("%s" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        return HOST_CLIENT_ERROR;
    }
    printf("%sAPI initialization done!\n",format_prefix);
    api_open = true;

    printf("%s Enabling  single stepping\n",format_prefix);
    if( SEV_STEP_OK != enable_single_stepping(&ctx,args->timer_value,NULL,0)) {
        printf("%sfailed to enable single stepping\n",format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    single_stepping_enabled = true;
    usp_event_type_t event_type;
    void* event_buffer;

    //
    //Main Event Loop. Simply log @args->want_steps many steps before disabling single stepping again
    //


    bool encountered_multistep = false;
    for( int current_event_idx = 0; current_event_idx < args->want_steps; current_event_idx++) {

        printf("%sWaiting next event, event_idx=%d\n",format_prefix,current_event_idx);
        if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer) ) {
            printf("%susp_block_until_event" BRED " FAILED\n" reset,format_prefix);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        if( event_type != SEV_STEP_EVENT ) {
            printf("%s unexpected event type\n",format_prefix);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }
        
        sev_step_event_t* step_event = (sev_step_event_t*)event_buffer;
        print_single_step_event(format_prefix,step_event);

        switch (step_event->counted_instructions)
        {
            case 0:
                results->zero_steps += 1;
                break;
            case 1:
                results->single_steps += 1;
                break;
            default:
                results->multi_steps += 1;
                if( !encountered_multistep ) {
                    encountered_multistep = true;
                }
                break;
        }

        results->events[current_event_idx].stepsize = step_event->counted_instructions;

        printf("%ssending ack for event_idx %d\n",format_prefix,current_event_idx);
        usp_ack_event(&ctx);
        free_usp_event(event_type,event_buffer);
    }
    
    printf("%sDisabling single stepping\n",format_prefix);
    if( SEV_STEP_OK != disable_single_stepping(&ctx) ) {
        printf("%sfailed to disable single stepping\n",format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    single_stepping_enabled = false;
    printf("%sClosing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%sAPI closed!\n",format_prefix);
    api_open = false;
    


    bool vm_alive = false;
    for( int retries = 1; !vm_alive && retries >= 0; retries--) {
        victim_program_data_t* _dummy = malloc(sizeof(victim_program_data_t));
        printf("%ssending dummy request to vm to ensure that it is alive and well\n",format_prefix);
        if ( HOST_CLIENT_ERROR == vm_server_single_step_victim_init(VICTIM_PROGRAM_NOP_SLIDE,_dummy) ) {
            printf("%sdummy request " BRED "FAILED. %d retries remaining\n" reset,format_prefix,retries);
        } else {
            vm_alive = true;
        }
        free_victim_program_data_t(_dummy);
    }
    //interact with the vm to ensure that it is not stuck
   if( vm_alive ) {
        printf("%s VM seems to be alive and well\n",format_prefix);
   } else {
        printf("%svm is stuck\n",format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
   }
    
   

cleanup:
    if( single_stepping_enabled ) {
        printf("%sDisabling single stepping\n",format_prefix);
        if( SEV_STEP_OK != disable_single_stepping(&ctx) ) {
            printf("%sfailed to disable single stepping\n",format_prefix);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }
    }

    if( api_open ) {
        printf("%sClosing API...\n",format_prefix);
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
            printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",format_prefix);
            res = HOST_CLIENT_ERROR;
        } else {
            printf("%sAPI closed!\n",format_prefix);
        }
    }
    return res;

}

typedef struct {
    //value for apic timer
    int timer_value;
    //amount of steps that we want
    int want_steps;
    //value between 0 and 1. If strictly more than this fraction of zero steps, mark test as failed.
    float zero_step_abort_fraction;
    //like zero_step_abort_fraction but for multi steps
    float multi_step_abort_fraction;
} test_single_step_simple_args_t;
int test_single_step_simple(char* format_prefix, void* void_args) {

    //
    // Setup args and call single_step_simple
    //

    test_single_step_simple_args_t* args = (test_single_step_simple_args_t*)void_args;

    single_step_simple_args_t sub_args = {
        .timer_value = args->timer_value,
        .want_steps = args->want_steps,
    };
    single_step_simple_results_t* result = alloc_single_step_simple_results_t(sub_args.want_steps);

    int r = single_step_simple(format_prefix,&sub_args,result);
    if( HOST_CLIENT_ERROR == r)  {
        printf("%s" BRED "test_single_step_simple: single_step_simple failed\n" reset,format_prefix);
        goto cleanup;
    }

    //
    // Analyze results
    //

    double zero_step_fraction = (double)result->zero_steps/sub_args.want_steps;
    double multi_step_fraction = (double)result->multi_steps/sub_args.want_steps;

    printf("%sStep Histogram (%d events) \n",format_prefix,sub_args.want_steps);
    printf("%s\t Timer value    : 0x%x\n",format_prefix,sub_args.timer_value);
    printf("%s\t Zero   Steps   : %02.2f%% (%06d events)\n",format_prefix, zero_step_fraction*100,result->zero_steps);
    printf("%s\t Single Steps   : %02.2f%% (%06d events)\n",
        format_prefix,(double)result->single_steps/sub_args.want_steps*100,result->single_steps);
    printf("%s\t Multi  Steps   : %02.2f%% (%06d events)\n",
        format_prefix,multi_step_fraction*100,result->multi_steps);


    if( zero_step_fraction > args->zero_step_abort_fraction) {
        printf("%s" BHRED "more than %0.1f %% zero steps! Test failed.\n" reset, format_prefix,args->zero_step_abort_fraction*100);
        r = HOST_CLIENT_ERROR;
        goto cleanup;
    };
     if( multi_step_fraction > args->multi_step_abort_fraction) {
        printf("%s" BHRED "more than %0.1f %% multi steps! Test failed.\n" reset, format_prefix,args->multi_step_abort_fraction*100);
        r = HOST_CLIENT_ERROR;
        goto cleanup;
    };
   

cleanup:
    free_single_step_simple_results_t(result);
    return r;
};


typedef struct {
    //value for apic timer
    int timer_value;
    //amount of steps that we want for a single run
    int want_steps_per_run;
    //how many runs we want (each with @want_steps_per_run steps)
    int want_runs;
    //value between 0 and 1. If strictly more than this fraction of zero steps, mark test as failed.
    //applied to the accumulated step values, not to each run individually
    float zero_step_abort_fraction;
    //like zero_step_abort_fraction but for multi steps
    float multi_step_abort_fraction;
} test_single_step_simple_rep_args_t;
int test_single_step_simple_rep(char* format_prefix,void* void_args) {

    //
    // Setup args and call single_step_simple
    //
    
    test_single_step_simple_rep_args_t* args = (test_single_step_simple_rep_args_t*) void_args;

    single_step_simple_args_t sub_args = {
        .timer_value = args->timer_value,
        .want_steps = args->want_steps_per_run,
    };

    const int total_step_count = args->want_runs * args->want_steps_per_run;
    int total_zero_step_count = 0;
    int total_single_step_count = 0;
    int total_multi_step_count = 0;
    int runs_without_multisteps = 0;
    int r;
    single_step_simple_results_t* result = alloc_single_step_simple_results_t(sub_args.want_steps);
    for( int run_idx = 0; run_idx < args->want_runs; run_idx++) {
        //reset result struct. event array is overwritten anyways, thus we do not touch it here
        result->multi_steps = 0;
        result->single_steps = 0;
        result->zero_steps = 0;

        r = single_step_simple(format_prefix,&sub_args,result);
        if( HOST_CLIENT_ERROR == r)  {
            printf("%s" BRED "test_single_step_simple: single_step_simple failed\n" reset,format_prefix);

            goto cleanup;
        }
        total_zero_step_count += result->zero_steps;
        total_single_step_count += result->single_steps;
        total_multi_step_count += result->multi_steps;
        if ( result->multi_steps == 0) {
            runs_without_multisteps += 1;
        }
    }
   

    //
    // Analyze results
    //

    double zero_step_fraction = (double)total_zero_step_count/total_step_count;
    double multi_step_fraction = (double)total_multi_step_count/total_step_count;

    printf("%sStep Histogram (%d events over %d runs) \n",format_prefix,total_step_count,args->want_runs);
    printf("%s\t Timer value    : 0x%x\n",format_prefix,sub_args.timer_value);
    printf("%s\t Zero   Steps   : %02.2f%% (%06d events)\n",format_prefix, zero_step_fraction*100,total_zero_step_count);
    printf("%s\t Single Steps   : %02.2f%% (%06d events)\n",
        format_prefix,(double)total_single_step_count/total_step_count*100,total_single_step_count);
    printf("%s\t Multi  Steps   : %02.2f%% (%06d events)\n",
        format_prefix,multi_step_fraction*100,total_multi_step_count);
    printf("%sRuns withtout multisteps: %02.2f%% (%d runs)",format_prefix,(double)runs_without_multisteps/args->want_runs*100,runs_without_multisteps);


    if( zero_step_fraction > args->zero_step_abort_fraction) {
        printf("%s" BHRED "more than %0.1f %% zero steps! Test failed.\n" reset, format_prefix,args->zero_step_abort_fraction*100);
        r = HOST_CLIENT_ERROR;
        goto cleanup;
    };
     if( multi_step_fraction > args->multi_step_abort_fraction) {
        printf("%s" BHRED "more than %0.1f %% multi steps! Test failed.\n" reset, format_prefix,args->multi_step_abort_fraction*100);
        r = HOST_CLIENT_ERROR;
        goto cleanup;
    };

    cleanup:
    free_single_step_simple_results_t(result);
    return r;
}


#endif