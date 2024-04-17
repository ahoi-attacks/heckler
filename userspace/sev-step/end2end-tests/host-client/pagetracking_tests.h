#ifndef PAGETRACKING_TESTS_H
#define PAGETRACKING_TESTS_H

#include <stdio.h>

#include "../../sev-step-lib/ansi-color-codes.h"
#include "../../sev-step-lib/sev_step_api.h"
#include "../../sev-step-lib/sev_step_print_macros.h"
#include "my-error-codes.h"
#include "vm-server-client.h"

int test_pagetrack(char* format_prefix,enum kvm_page_track_mode tracking_mode ) {

    //
    //Intialize API connection and prepare tracked pages on server/vm side
    //

    usp_poll_api_ctx_t ctx;
    int res = HOST_CLIENT_SUCCESS;

    printf("%sInitializing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx, false)) {
        printf("%s" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        return HOST_CLIENT_ERROR;
    }
    printf("%sAPI initialization done!\n",format_prefix);


    char* vm_server_tracking_mode;
    switch (tracking_mode)
    {
    case KVM_PAGE_TRACK_WRITE:
        vm_server_tracking_mode = "WRITE";
        break;
     case KVM_PAGE_TRACK_ACCESS:
        vm_server_tracking_mode = "READ";
        break;
    case KVM_PAGE_TRACK_EXEC:
        vm_server_tracking_mode = "EXEC";
        break;
    default:
        printf("%svm server does not support tracking mode %s\n",format_prefix,tracking_mode_to_string(tracking_mode));
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    pagetrack_gpas_t result;
    printf("%scalling vm_server_pagetrack_init to prepare %s scenario\n",format_prefix,vm_server_tracking_mode);
    if ( HOST_CLIENT_ERROR == vm_server_pagetrack_init(vm_server_tracking_mode, &result) ) {
        printf("%svm_server_pagetrack_init " BRED "FAILED\n" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%sGPA1 is 0x%lx and GPA2 is 0x%lx\n",format_prefix,result.gpa1,result.gpa2);
    if( (result.gpa1 >> 12) == (result.gpa2 >> 12)) {
        printf("%s" BHRED " both GPAs point to same page. Something is wrong with the server\n"reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    //
    // Track gpa1 and signal vm server to start accessing gpa1 and gpa2 in alternating manner.
    // Then, enter main event loop
    //

    printf("%stracking gpa 0x%lx with mode %s\n",format_prefix,result.gpa1,tracking_mode_to_string(tracking_mode));
    if( SEV_STEP_OK != track_page(&ctx,result.gpa1,tracking_mode) ) {
        printf("%sinitial track page for 0x%lx" BRED "FAILED" reset,format_prefix,result.gpa1);
    }

    printf("%sissuing vm_server_pagetrack_start\n",format_prefix);
    if ( HOST_CLIENT_ERROR == vm_server_pagetrack_start() ) {
        printf("%svm_server_pagetrack_start " BRED "FAILED\n" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    printf("%sentering main event loop\n",format_prefix);
    const int want_events = 10;
    usp_event_type_t event_type;
    void* event_buffer;
    uint64_t want_gpa = result.gpa1;
    uint64_t retrack_gpa;
    for( int current_event_idx = 0; current_event_idx < want_events; current_event_idx++) {

        printf("%sWaiting for fault at 0x%lx with event_idx %d\n",format_prefix,want_gpa,current_event_idx);
        if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer) ) {
            printf("%susp_block_until_event" BRED "FAILED\n" reset,format_prefix);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        if( event_type != PAGE_FAULT_EVENT ) {
            printf("%sExpected page fault event, got %d\n",format_prefix,event_type);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }
        usp_page_fault_event_t* event = (usp_page_fault_event_t*)event_buffer;

    
        if( current_event_idx % 2 == 0) {
            want_gpa = result.gpa1;
            retrack_gpa = result.gpa2;
        } else {
            want_gpa = result.gpa2;
            retrack_gpa = result.gpa1;
        }
       
       if( event->faulted_gpa != want_gpa) {
            printf("%sExpected page fault event, at gpa 0x%lx but got fault at 0x%lx\n",format_prefix,want_gpa,event->faulted_gpa);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        } else {
            printf("%sgot expected event\n",format_prefix);
        }

        printf("%s(re-)tracking gpa 0x%lx with mode %s\n",format_prefix,retrack_gpa,tracking_mode_to_string(tracking_mode));
        if( SEV_STEP_OK != track_page(&ctx,retrack_gpa,tracking_mode) ) {
            printf("%strack_page(0x%lx,%s)" BRED "FAILED" reset,format_prefix,retrack_gpa,tracking_mode_to_string(tracking_mode));
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

       
        printf("%ssending ack for event_idx %d\n",format_prefix,current_event_idx);
        usp_ack_event(&ctx);
        free_usp_event(event_type,event_buffer);

    } //end of main event loop



cleanup:
    printf("%sClosing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        res = HOST_CLIENT_ERROR;
    } else {
        printf("%sAPI closed!\n",format_prefix);
    }
    printf("%svm_server_pagetrack_teardown\n",format_prefix);
    if ( HOST_CLIENT_ERROR == vm_server_pagetrack_teardown() ) {
        printf("\nvm_server_pagetrack_teardown " BRED "FAILED\n" reset);
        res = HOST_CLIENT_ERROR;
    }
    return res;
}

int test_pagetrack_write(char* format_prefix,void* _args) {
    (void)_args;
    return test_pagetrack(format_prefix,KVM_PAGE_TRACK_WRITE);
}

int test_pagetrack_exec(char* format_prefix,void* _args) {
    (void)_args;
    return test_pagetrack(format_prefix,KVM_PAGE_TRACK_EXEC);
}

int test_pagetrack_access(char* format_prefix, void* _args) {
    (void)_args;
    return test_pagetrack(format_prefix,KVM_PAGE_TRACK_ACCESS);
}


int test_track_all(char* format_prefix, void* _args) {
    (void)_args;
    enum kvm_page_track_mode tracking_mode = KVM_PAGE_TRACK_EXEC;
     //
    //Intialize API connection and prepare tracked pages on server/vm side
    //

    usp_poll_api_ctx_t ctx;
    int res = HOST_CLIENT_SUCCESS;

    printf("%sInitializing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx, false)) {
        printf("%s" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        return HOST_CLIENT_ERROR;
    }
    printf("%sAPI initialization done!\n",format_prefix);


    char* vm_server_tracking_mode;
    switch (tracking_mode)
    {
    case KVM_PAGE_TRACK_WRITE:
        vm_server_tracking_mode = "WRITE";
        break;
     case KVM_PAGE_TRACK_ACCESS:
        vm_server_tracking_mode = "READ";
        break;
    case KVM_PAGE_TRACK_EXEC:
        vm_server_tracking_mode = "EXEC";
        break;
    default:
        printf("%svm server does not support tracking mode %s\n",format_prefix,tracking_mode_to_string(tracking_mode));
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    pagetrack_gpas_t result;
    printf("%scalling vm_server_pagetrack_init to prepare %s scenario\n",format_prefix,vm_server_tracking_mode);
    if ( HOST_CLIENT_ERROR == vm_server_pagetrack_init(vm_server_tracking_mode, &result) ) {
        printf("%svm_server_pagetrack_init " BRED "FAILED\n" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    if( (result.gpa1 >> 12) == (result.gpa2 >> 12)) {
        printf("%s" BHRED " both GPAs point to same page. Something is wrong with the server\n"reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

   

    printf("%stracking gpa 0x%lx with mode %s\n",format_prefix,result.gpa1,tracking_mode_to_string(tracking_mode));
    if( SEV_STEP_OK != track_page(&ctx,result.gpa1,tracking_mode) ) {
        printf("%sinitial track page for 0x%lx" BRED "FAILED" reset,format_prefix,result.gpa1);
    }

    printf("%sissuing vm_server_pagetrack_start\n",format_prefix);
    if ( HOST_CLIENT_ERROR == vm_server_pagetrack_start() ) {
        printf("%svm_server_pagetrack_start " BRED "FAILED\n" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    usp_event_type_t event_type;
    void* event_buffer;


    printf("%sWaiting for fault to get vm in halted state \n",format_prefix);
    if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer) ) {
        printf("%susp_block_until_event" BRED "FAILED\n" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    if( event_type != PAGE_FAULT_EVENT ) {
        printf("%sExpected page fault event, got %d\n",format_prefix,event_type);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    
    printf("%sTracking all pages \n",format_prefix);
    if( SEV_STEP_OK != track_all_pages(&ctx,tracking_mode) ) {
        printf("%strack_all_pages " BRED "FAILED" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    printf("%sUntracking all pages \n",format_prefix);
    if( SEV_STEP_OK != untrack_all_pages(&ctx,tracking_mode) ) {
        printf("%suntrack_all_pages " BRED "FAILED" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    
    printf("%ssending ack\n",format_prefix);
    usp_ack_event(&ctx);
    free_usp_event(event_type,event_buffer);




cleanup:
    printf("%sClosing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        res = HOST_CLIENT_ERROR;
    } else {
        printf("%sAPI closed!\n",format_prefix);
    }
    printf("%svm_server_pagetrack_teardown\n",format_prefix);
    if ( HOST_CLIENT_ERROR == vm_server_pagetrack_teardown() ) {
        printf("\nvm_server_pagetrack_teardown " BRED "FAILED\n" reset);
        res = HOST_CLIENT_ERROR;
    }
    return res;
}


int test_track_all_rep(char* format_prefix, void* _args) { 
    (void)_args;
    for(uint64_t i = 0; i < 10;i++) {
        if( HOST_CLIENT_ERROR == test_track_all(format_prefix,NULL) ) {
            printf("%sRun %ju of test_track_all failed!\n",format_prefix, i);
            return HOST_CLIENT_ERROR;
        }
    }
    return HOST_CLIENT_SUCCESS;
}

int test_track_all_mixed_with_single(char* format_prefix, void* _args) {
    (void)_args;
    //
    //Intialize API connection and prepare tracked pages on server/vm side
    //

    usp_poll_api_ctx_t ctx;
    int res = HOST_CLIENT_SUCCESS;

    printf("%sInitializing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_new_ctx(&ctx, false)) {
        printf("%s" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        return HOST_CLIENT_ERROR;
    }
    printf("%sAPI initialization done!\n",format_prefix);


    enum kvm_page_track_mode tracking_mode = KVM_PAGE_TRACK_EXEC;
    char* vm_server_tracking_mode;
    switch (tracking_mode)
    {
    case KVM_PAGE_TRACK_WRITE:
        vm_server_tracking_mode = "WRITE";
        break;
     case KVM_PAGE_TRACK_ACCESS:
        vm_server_tracking_mode = "READ";
        break;
    case KVM_PAGE_TRACK_EXEC:
        vm_server_tracking_mode = "EXEC";
        break;
    default:
        printf("%svm server does not support tracking mode %s\n",format_prefix,tracking_mode_to_string(tracking_mode));
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    pagetrack_gpas_t result;
    printf("%scalling vm_server_pagetrack_init to prepare %s scenario\n",format_prefix,vm_server_tracking_mode);
    if ( HOST_CLIENT_ERROR == vm_server_pagetrack_init(vm_server_tracking_mode, &result) ) {
        printf("%svm_server_pagetrack_init " BRED "FAILED\n" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }
    printf("%sGPA1 is 0x%lx and GPA2 is 0x%lx\n",format_prefix,result.gpa1,result.gpa2);
    if( (result.gpa1 >> 12) == (result.gpa2 >> 12)) {
        printf("%s" BHRED " both GPAs point to same page. Something is wrong with the server\n"reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    //
    // Track gpa1 and signal vm server to start accessing gpa1 and gpa2 in alternating manner.
    // Then, enter main event loop
    //

    printf("%stracking gpa 0x%lx with mode %s\n",format_prefix,result.gpa1,tracking_mode_to_string(tracking_mode));
    if( SEV_STEP_OK != track_page(&ctx,result.gpa1,tracking_mode) ) {
        printf("%sinitial track page for 0x%lx" BRED "FAILED" reset,format_prefix,result.gpa1);
    }

    printf("%sissuing vm_server_pagetrack_start\n",format_prefix);
    if ( HOST_CLIENT_ERROR == vm_server_pagetrack_start() ) {
        printf("%svm_server_pagetrack_start " BRED "FAILED\n" reset,format_prefix);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }


    printf("%sentering main event loop\n",format_prefix);
    const int want_events = 10;
    usp_event_type_t event_type;
    void* event_buffer;
    uint64_t want_gpa = result.gpa1;
    uint64_t retrack_gpa;
    for( int current_event_idx = 0; current_event_idx < want_events; current_event_idx++) {

        printf("%sWaiting for fault at 0x%lx with event_idx %d\n",format_prefix,want_gpa,current_event_idx);
        if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer) ) {
            printf("%susp_block_until_event" BRED "FAILED\n" reset,format_prefix);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        if( event_type != PAGE_FAULT_EVENT ) {
            printf("%sExpected page fault event, got %d\n",format_prefix,event_type);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }
        usp_page_fault_event_t* event = (usp_page_fault_event_t*)event_buffer;

    
        if( current_event_idx % 2 == 0) {
            want_gpa = result.gpa1;
            retrack_gpa = result.gpa2;
        } else {
            want_gpa = result.gpa2;
            retrack_gpa = result.gpa1;
        }
       
       if( event->faulted_gpa != want_gpa) {
            printf("%sExpected page fault event, at gpa 0x%lx but got fault at 0x%lx\n",format_prefix,want_gpa,event->faulted_gpa);
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        } else {
            printf("%sgot expected event\n",format_prefix);
        }



        flf_printf("issuing track all\n");
        if( SEV_STEP_OK != track_all_pages(&ctx,tracking_mode)) {
            flf_printf("track_all failed\n");
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

        flf_printf("issuing untrack all\n");
        if( SEV_STEP_OK != untrack_all_pages(&ctx,tracking_mode)) {
            flf_printf("untrack_all_pages failed\n");
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }



        printf("%s(re-)tracking gpa 0x%lx with mode %s\n",format_prefix,retrack_gpa,tracking_mode_to_string(tracking_mode));
        if( SEV_STEP_OK != track_page(&ctx,retrack_gpa,tracking_mode) ) {
            printf("%strack_page(0x%lx,%s)" BRED "FAILED" reset,format_prefix,retrack_gpa,tracking_mode_to_string(tracking_mode));
            res = HOST_CLIENT_ERROR;
            goto cleanup;
        }

       
        printf("%ssending ack for event_idx %d\n",format_prefix,current_event_idx);
        usp_ack_event(&ctx);
        free_usp_event(event_type,event_buffer);

    } //end of main event loop



cleanup:
    printf("%sClosing API...\n",format_prefix);
    if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",format_prefix);
        res = HOST_CLIENT_ERROR;
    } else {
        printf("%sAPI closed!\n",format_prefix);
    }
    printf("%svm_server_pagetrack_teardown\n",format_prefix);
    if ( HOST_CLIENT_ERROR == vm_server_pagetrack_teardown() ) {
        printf("\nvm_server_pagetrack_teardown " BRED "FAILED\n" reset);
        res = HOST_CLIENT_ERROR;
    }
    return res;
}

#endif