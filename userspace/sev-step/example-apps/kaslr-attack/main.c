#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#include "../../sev-step-lib/sev_step_api.h"
#include "../../sev-step-lib/ansi-color-codes.h"

#define EXIT_CODE_OK 0
#define EXIT_CODE_ERR 1

int main() {
    int exit_code = 0;

    usp_poll_api_ctx_t ctx;
    bool api_open = false;
    printf("Initializing API...\n");
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,true)) {
        printf(BRED "usp_new_ctx failed." reset " Check dmesg for more information\n");
        return EXIT_CODE_ERR;
    }
    printf("API initialization done!\n");
    api_open = true;

    //
    // Algorithm:
    // 1) Track all pages and wait untill vm is paused
    // 2) Inject NMI
    // 3) Retrack pages
    // 4) continue vm and store next pf
    //


    //track all pages
    if( SEV_STEP_OK != track_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
        printf("track_all_pages: %s:%d " BHRED" FAILED\n"reset,__FILE__,__LINE__);
        exit_code = EXIT_CODE_ERR;
        goto cleanup;
    }

    printf("Waiting for random page fault to pause VM...\n");
    usp_event_type_t event_type;
    void* event_buffer;
    if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer)) {
        printf("usp_block_until_event: %s:%d " BHRED "FAILED"reset,__FILE__,__LINE__);
        exit_code = EXIT_CODE_ERR;
        goto cleanup;
    }

    printf("Got random page fault. Injecting nmi...\n");

    if( SEV_STEP_ERR == inject_nmi(&ctx) ) {
        printf("inject_nmi: %s:%d " BHRED "FAILED"reset,__FILE__,__LINE__);
        exit_code = EXIT_CODE_ERR;
        goto cleanup;
    }

    printf("Re-tracking...\n");
    if( SEV_STEP_OK != track_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
        printf("track_all_pages: %s:%d " BHRED" FAILED\n"reset,__FILE__,__LINE__);
        exit_code = EXIT_CODE_ERR;
        goto cleanup;
    }

    free_usp_event(event_type, event_buffer);
    printf("Acking event..\n");
    usp_ack_event(&ctx);


    printf("Waiting for first pf after nmi injection...\n");
    if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer)) {
        printf("usp_block_until_event: %s:%d " BHRED "FAILED"reset,__FILE__,__LINE__);
        exit_code = EXIT_CODE_ERR;
        goto cleanup;
    }

    if( event_type != PAGE_FAULT_EVENT ) {
        printf("%s:%d : expected page fault event, got %d\n",__FILE__,__LINE__,event_type);
        exit_code = EXIT_CODE_ERR;
        goto cleanup;
    }
    usp_page_fault_event_t* pf_event = (usp_page_fault_event_t*)event_buffer;
    printf(BHGRN "Pagefault Event: {GPA:0x%lx}\n"reset,pf_event->faulted_gpa);

    if( pf_event->is_decrypted_vmsa_data_valid ) {
        printf(BHGRN" RIP: 0x%lx\n"reset, pf_event->decrypted_vmsa_data.register_values[VRN_RIP]);
    } else {
        printf("No RIP data avilable. Activate debug mode\n");
    }

    free_usp_event(event_type, event_buffer);
    printf("Acking event..\n");
    usp_ack_event(&ctx);

    printf("Closing API...\n");
    if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf(BRED "usp_close_ctx failed." reset " Check dmesg for more information\n");
        exit_code = EXIT_CODE_ERR;
        api_open = false;
        goto cleanup;
    } else {
        printf("API closed!\n");
    }
    api_open = false;


cleanup:
    if( api_open ) {
        printf("Closing API...\n");
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf( BRED "usp_close_ctx failed." reset " Check dmesg for more information\n");
        exit_code = EXIT_CODE_ERR;
        } else {
        printf("API closed!\n");
        }
    }

    return exit_code;
}