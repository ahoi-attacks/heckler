#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include "sev_step_api.h"
#include "raw_spinlock.h"

#include "sev_step_print_macros.h"

perf_ctl_config_t host_os_l2_hit_from_l1_dc_miss = {
	.EventSelect = 0x64,
    .UintMask = 0xf0,
    .HostGuestOnly = 0x2,
    .CntMask = 0x0,
    .Inv = 0x0,
    .En = 0x1,
    .Int = 0x0,
    .Edge = 0x0,
    .OsUserMode = 0x2,
    .descriptive_name = "host_os_l2_hit_from_l1_dc_miss",
};

perf_ctl_config_t host_os_l2_miss_from_l1_dc_miss = {
	.EventSelect = 0x64,
    .UintMask = 0x08,
    .HostGuestOnly = 0x2,
    .CntMask = 0x0,
    .Inv = 0x0,
    .En = 0x1,
    .Int = 0x0,
    .Edge = 0x0,
    .OsUserMode = 0x2,
    .descriptive_name = "host_os_l2_miss_from_l1_dc_miss",
};

perf_ctl_config_t host_ost_l2_cache_miss_from_l2_cache_hwpf_1 = {
	.EventSelect = 0x71,
    .UintMask = 0x1f,
    .HostGuestOnly = 0x2,
    .CntMask = 0x0,
    .Inv = 0x0,
    .En = 0x1,
    .Int = 0x0,
    .Edge = 0x0,
    .OsUserMode = 0x2,
    .descriptive_name = "host_ost_l2_cache_miss_from_l2_cache_hwpf_1",

};
perf_ctl_config_t host_ost_l2_cache_miss_from_l2_cache_hwpf_2 = {
	.EventSelect = 0x72,
    .UintMask = 0x1f,
    .HostGuestOnly = 0x2,
    .CntMask = 0x0,
    .Inv = 0x0,
    .En = 0x1,
    .Int = 0x0,
    .Edge = 0x0,
    .OsUserMode = 0x2,
    .descriptive_name = "host_ost_l2_cache_miss_from_l2_cache_hwpf_2",
};

perf_ctl_config_t host_os_l1d_fill_all = {
	.EventSelect = 0x44,
    .UintMask = 0xff,
    .HostGuestOnly = 0x2,
    .CntMask = 0x0,
    .Inv = 0x0,
    .En = 0x1,
    .Int = 0x0,
    .Edge = 0x0,
    .OsUserMode = 0x2,
    .descriptive_name ="host_os_l1d_fill_all",
};

perf_ctl_config_t host_os_l1d_fill_from_memory = {
    .EventSelect = 0x44,
    .UintMask = 0x48,
    .HostGuestOnly = 0x2,
    .CntMask = 0x0,
    .Inv = 0x0,
    .En = 0x1,
    .Int = 0x0,
    .Edge = 0x0,
    .OsUserMode = 0x2,
    .descriptive_name = "host_os_l1d_fill_from_memory",
};

perf_ctl_config_t host_os_all_data_cache_accesses = {
    .EventSelect = 0x29,
    .UintMask = 0x07,
    .HostGuestOnly = 0x2,
    .CntMask = 0x0,
    .Inv = 0x0,
    .En = 0x1,
    .Int = 0x0,
    .Edge = 0x0,
    .OsUserMode = 0x2,
    .descriptive_name = "host_os_all_data_cache_accesses",
};

int SHARED_MEM_BYTES = 20 * 4096;

static volatile int keepRunning = 1;
void sig_handler(int dummy);

char* vmsa_register_name_to_str(vmsa_register_name_t reg) {
    switch (reg)
    {
    case VRN_RFLAGS:
        return "rflags";
        break;
    case VRN_RIP:
        return "rip";
        break;
    case VRN_RSP:
        return "rsp";
        break;
    case VRN_R10:
        return "r10";
        break;
    case VRN_R11:
        return "r11";
        break;
    case VRN_R12:
        return "r12";
        break;
    case VRN_R13:
        return "r13";
        break;
    case VRN_R8:
        return "r8";
        break;
    case VRN_R9:
        return "r9";
        break;
    case VRN_RBX:
        return "rbx";
        break;
    case VRN_RCX:
        return "rcx";
        break;
    case VRN_RDX:
        return "rdx";
        break;
    case VRN_RSI:
        return "rsi";
        break;
    case VRN_CR3:
        return "cr3";
        break;
    default:
        return NULL;
        break;
    }
}

int vmsa_register_name_from_str(char* c, vmsa_register_name_t* result) {
    int res = SEV_STEP_OK;
    //create copy with all lowercase
    char* low = strdup(c);
    for(int i = 0; i < low[i]; i++ ) {
        low[i] = tolower(low[i]);
    }

    if( 0 ==strcmp(low,"rflags") ) {
        *result = VRN_RFLAGS;
    } else if( 0 ==strcmp(low,"rip") ) {
        *result = VRN_RIP;
    } else if( 0 ==strcmp(low,"rsp") ) {
        *result = VRN_RSP;
    } else if( 0 ==strcmp(low,"r10") ) {
        *result = VRN_R10;
    } else if( 0 ==strcmp(low,"r11") ) {
        *result = VRN_R11;
    } else if( 0 ==strcmp(low,"r12") ) {
        *result = VRN_R12;
    } else if( 0 ==strcmp(low,"r13") ) {
        *result = VRN_R13;
    } else if( 0 ==strcmp(low,"r8") ) {
        *result = VRN_R8;
    } else if( 0 ==strcmp(low,"r9") ) {
        *result = VRN_R9;
    } else if( 0 ==strcmp(low,"rbx") ) {
        *result = VRN_RBX;
    } else if( 0 ==strcmp(low,"rcx") ) {
        *result = VRN_RCX;
    } else if( 0 ==strcmp(low,"rdx") ) {
        *result = VRN_RDX;
    } else if( 0 ==strcmp(low,"rsi") ) {
        *result = VRN_RSI;
    } else if( 0 ==strcmp(low,"cr3") ) {
        *result = VRN_CR3;
    } else {
        res = SEV_STEP_ERR;
    }

    free(low);
    return res;
}

void print_single_step_event(char* fp, sev_step_event_t* event) {
     if( event->is_decrypted_vmsa_data_valid ) {
        printf("%sSingle Step Event: {RIP:0x%lx, StepSize: %u}\n",fp,event->decrypted_vmsa_data.register_values[VRN_RIP], event->counted_instructions);
    } else {
        printf("%sSingle Step Event (no vmsa data): {StepSize: %u}\n",fp,event->counted_instructions);
        if(event->counted_instructions > 0) {
            //NEMESIS
            printf("%s {Latency: %lu}\n", fp,  event->tsc_latency);
        }
    }
}

void free_usp_event(usp_event_type_t type, void *event) {
    if (type == SEV_STEP_EVENT ) {
        sev_step_event_t* step_event = (sev_step_event_t*)event;
        if(step_event->cache_attack_timings != NULL ) {
            free(step_event->cache_attack_timings);
        }
        if(step_event->cache_attack_perf_values != NULL) {
            free(step_event->cache_attack_perf_values);
        }
    }
    free(event);
}

int usp_poll_event(usp_poll_api_ctx_t* ctx, int* got_event, usp_event_type_t* event_type, void** event) {

    raw_spinlock_lock(&ctx->shared_mem_region->spinlock);
    if( !ctx->shared_mem_region->have_event ) {
        *got_event = 0;
        raw_spinlock_unlock(&ctx->shared_mem_region->spinlock);
        //no an event is NOT an error
        return SEV_STEP_OK;
    }

    //if we are here, we hold the lock and there was an event
    *got_event = 1;
    ctx->vm_is_paused = true;
    *event_type = ctx->shared_mem_region->event_type;

    uint64_t event_bytes;
    uint64_t event_buffer_offset = 0;
    if( get_size_for_event(*event_type,&event_bytes) ) {
        fprintf(stderr,"get_size_for_event failed\n");
        return SEV_STEP_ERR;
    }
    *event = malloc(event_bytes);
    memcpy(*event,ctx->shared_mem_region->event_buffer + event_buffer_offset ,event_bytes);
    event_buffer_offset += event_bytes;
    if( *event_type == SEV_STEP_EVENT ) {
        sev_step_event_t* step_event = (sev_step_event_t*)(*event);
        if( step_event->cache_attack_timings != NULL ) {
            //fprintf(stderr,"step event with timing data. %lu entries\n",step_event->cache_attack_data_len);
            uint64_t cache_attack_timings_bytes = sizeof(uint64_t) * step_event->cache_attack_data_len;
            //the ptr is still from kernel space, we need to copy the data out of the event buffer anyways
            step_event->cache_attack_timings = malloc(cache_attack_timings_bytes);
            memcpy(step_event->cache_attack_timings, ctx->shared_mem_region->event_buffer + event_buffer_offset,
                cache_attack_timings_bytes
            );
            event_buffer_offset += cache_attack_timings_bytes;
        }
        if( step_event->cache_attack_perf_values != NULL ) {
            //fprintf(stderr,"step event with perf data. %lu entries\n",step_event->cache_attack_data_len);
            uint64_t cache_attack_perf_bytes = sizeof(uint64_t) * step_event->cache_attack_data_len;
            //the ptr is still from kernel space, we need to copy the data out of the event buffer anyways
            step_event->cache_attack_perf_values = malloc(cache_attack_perf_bytes);
            memcpy(step_event->cache_attack_perf_values, ctx->shared_mem_region->event_buffer + event_buffer_offset,
                cache_attack_perf_bytes
            );
            event_buffer_offset += cache_attack_perf_bytes;
        }
    }

    raw_spinlock_unlock(&ctx->shared_mem_region->spinlock);
    return SEV_STEP_OK;
    
}

void sig_handler(int dummy) {
    (void)dummy;
    keepRunning = 0;
}

int usp_block_until_event(usp_poll_api_ctx_t* ctx, usp_event_type_t* event_type, void** event) {
    int err;
    int got_event;

    if( ctx->vm_is_paused ) {
        flf_printf("ctx-vm_is_paused is set to true, this will probably deadlock!\n");
    }

    // interrupt while loop
    signal(SIGINT, sig_handler);
    while( keepRunning ) {
        err = usp_poll_event(ctx,&got_event,event_type,event);
        if(err) {
            fprintf(stderr,"usp_poll_event failed with %d\n",err);
            return SEV_STEP_ERR;
        }
        if( got_event ) {
            return SEV_STEP_OK;
        }
    }
    printf("Abort to prevent freeze.\n");
    return SEV_STEP_ERR;
}

int usp_block_until_event_or_cb(usp_poll_api_ctx_t* ctx, usp_event_type_t* event_type, void** event, bool(should_abort)(void*), void* should_abort_args) {
        int err;
    int got_event;

    if( ctx->vm_is_paused ) {
        flf_printf("ctx-vm_is_paused is set to true, this will probably deadlock!\n");
    }

    // interrupt while loop
    signal(SIGINT, sig_handler);
    while( keepRunning ) {
        usleep(1000);
        err = usp_poll_event(ctx,&got_event,event_type,event);
        if(err) {
            fprintf(stderr,"usp_poll_event failed with %d\n",err);
            return SEV_STEP_ERR;
        }
        if( got_event ) {
            return SEV_STEP_OK;
        }
        if( should_abort(should_abort_args) ) {
            return SEV_STEP_ERR_ABORT;
        }
    }
    printf("Abort to prevent freeze.\n");
    return SEV_STEP_ERR;
}

bool is_vm_paused(usp_poll_api_ctx_t* ctx) {
    return ctx->vm_is_paused;
}

void usp_ack_event(usp_poll_api_ctx_t* ctx) {
    raw_spinlock_lock(&ctx->shared_mem_region->spinlock);
    if( ctx->shared_mem_region->event_acked) {
        fprintf(stderr,"usp_ack_event was called but ctx->shared_mem_region->event_acked was true");
    }

    // set status flags
    ctx->shared_mem_region->event_acked = 1;
    ctx->shared_mem_region->have_event = 0;
    ctx->vm_is_paused = false;
    raw_spinlock_unlock(&ctx->shared_mem_region->spinlock);
}

int get_size_for_event(usp_event_type_t event_type, uint64_t* size) {
    switch (event_type)
    {
        case PAGE_FAULT_EVENT:
            *size = sizeof(usp_page_fault_event_t);
            return SEV_STEP_OK;
        case SEV_STEP_EVENT:
            *size = sizeof(sev_step_event_t);
            return SEV_STEP_OK;
        default:
            return SEV_STEP_ERR;
    }
}

int track_page(usp_poll_api_ctx_t *ctx, uint64_t gpa, enum kvm_page_track_mode mode) {
    track_page_param_t tp = {
        .gpa = gpa,
        .track_mode = mode,
    };
    if(ioctl(ctx->kvm_fd, KVM_TRACK_PAGE, &tp) < 0) {
        perror("track_page: Error calling KVM_TRACK_PAGE ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int untrack_page(usp_poll_api_ctx_t *ctx, uint64_t gpa, enum kvm_page_track_mode mode) {
     track_page_param_t tp = {
        .gpa = gpa,
        .track_mode = mode,
    };
    if(ioctl(ctx->kvm_fd, KVM_UNTRACK_PAGE, &tp) < 0) {
        perror("untrack_page: Error calling KVM_UNTRACK_PAGE ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int track_all_pages(usp_poll_api_ctx_t *ctx, enum kvm_page_track_mode mode) {
    track_all_pages_t param = {
        .track_mode = mode,
    };
    if(ioctl(ctx->kvm_fd, KVM_TRACK_ALL_PAGES, &param) < 0) {
        perror("track_all_pages: Error calling KVM_TRACK_ALL_PAGES ioctl");
        return SEV_STEP_ERR;
    }
    
    return SEV_STEP_OK;
}

int untrack_all_pages(usp_poll_api_ctx_t *ctx, enum kvm_page_track_mode mode) {
    track_all_pages_t param = {
        .track_mode = mode,
    };
    if(ioctl(ctx->kvm_fd, KVM_UNTRACK_ALL_PAGES, &param) < 0) {
        perror("untrack_all_pages: Error calling KVM_UNTRACK_ALL_PAGES ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int enable_single_stepping(usp_poll_api_ctx_t *ctx, uint32_t timer_value, uint64_t *gpas_target_pages, uint64_t gpas_target_pages_len) {
    sev_step_param_t params = {
        .gpas_target_pages = gpas_target_pages,
        .gpas_target_pages_len = gpas_target_pages_len,
        .tmict_value = timer_value,
        .do_tlb_flush_before_each_step = true,
    };
    if(ioctl(ctx->kvm_fd, KVM_SEV_STEP_ENABLE, &params) < 0) {
        perror("enable_single_stepping: Error calling KVM_SEV_STEP_ENABLE ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int enable_single_stepping_ex(usp_poll_api_ctx_t *ctx, uint32_t timer_value, uint64_t *gpas_target_pages, uint64_t gpas_target_pages_len, bool tlb_flush)
{
    sev_step_param_t params = {
        .gpas_target_pages = gpas_target_pages,
        .gpas_target_pages_len = gpas_target_pages_len,
        .tmict_value = timer_value,
        .do_tlb_flush_before_each_step = tlb_flush,
    };
    if (ioctl(ctx->kvm_fd, KVM_SEV_STEP_ENABLE, &params) < 0)
    {
        perror("enable_single_stepping: Error calling KVM_SEV_STEP_ENABLE ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int disable_single_stepping(usp_poll_api_ctx_t *ctx) {
    if(ioctl(ctx->kvm_fd, KVM_SEV_STEP_DISABLE, NULL) < 0) {
        perror("disable_single_stepping: Error calling KVM_SEV_STEP_DISABLE ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int usp_new_ctx(usp_poll_api_ctx_t* ctx, bool debug_mode) {
    //alloc memory for shared mem
    int shared_mem_struct_bytes = sizeof(shared_mem_region_t);
    if( shared_mem_struct_bytes > SHARED_MEM_BYTES ) {
        fprintf(stderr,"shared_mem_region_t struct has %d bytes which is larger than the %d shared mem backing buffer\n",shared_mem_struct_bytes,SHARED_MEM_BYTES);
        return SEV_STEP_ERR;
    }
    ctx->shared_mem_region = mmap(NULL,SHARED_MEM_BYTES,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if( ctx->shared_mem_region == NULL ) {
        fprintf(stderr,"Failed to mmap memory for shared memory region\n");
        return SEV_STEP_ERR;
    }
    memset((void*)ctx->shared_mem_region,0,SHARED_MEM_BYTES);

    //initialize shared mem struct
    INIT_SPINLOCK(&ctx->shared_mem_region->spinlock);
    ctx->shared_mem_region->event_acked = 1;
    ctx->shared_mem_region->have_event = 0;

    usp_init_poll_api_t params = {
        .pid = getpid(),
        .user_vaddr_shared_mem = (uint64_t)ctx->shared_mem_region,
        .decrypt_vmsa = debug_mode,
    };

    //init user/kernel poll api
    int kvm = open("/dev/kvm", O_RDWR|O_CLOEXEC);
    if(kvm < 0) {
        perror("Error calling open /dev/kvm");
        return SEV_STEP_ERR;
    }

    ctx->kvm_fd = kvm;
    int err = ioctl(ctx->kvm_fd, KVM_USP_INIT_POLL_API, &params);
    if (err < 0) {
        perror("Error calling KVM_USP_INIT_POLL_API ioctl");
        return SEV_STEP_ERR;
    }
    ctx->vm_is_paused = false;
    return SEV_STEP_OK;
}

int usp_close_ctx(usp_poll_api_ctx_t *ctx) {

    disable_single_stepping(ctx);

   
    enum kvm_page_track_mode modes[] = {
        KVM_PAGE_TRACK_WRITE,
        KVM_PAGE_TRACK_ACCESS,
        KVM_PAGE_TRACK_EXEC
    };
    for(uint64_t i = 0; i < sizeof(modes)/sizeof(modes[0]); i++) {
        if(SEV_STEP_OK != untrack_all_pages(ctx,modes[i]) ) {
            flf_printf("untrack_all_pages with mode %d failed\n",modes[i]);
        }
    }

    int err = ioctl(ctx->kvm_fd, KVM_USP_CLOSE_POLL_API, NULL );
    if (err < 0) {
        perror("Error calling KVM_USP_INIT_POLL_API ioctl");
        return SEV_STEP_ERR;
    }

    close(ctx->kvm_fd);
    munmap(ctx->shared_mem_region,SHARED_MEM_BYTES);
    ctx->vm_is_paused = false;
    return SEV_STEP_OK;
}

int inject_nmi(usp_poll_api_ctx_t* ctx) {
     if(ioctl(ctx->kvm_fd, KVM_SEV_STEP_INJECT_NMI, NULL) < 0) {
        perror("inject_nmi: Error calling KVM_SEV_STEP_INJECT_NMI ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}


int sev_step_import_user_evs(usp_poll_api_ctx_t* ctx, import_user_eviction_set_param_t* params) {
    for( uint64_t outer_idx=0; outer_idx < params->len; outer_idx++ ) {
        uint64_t* inner_array = params->eviction_sets[outer_idx].eviction_sets;
        for( uint64_t inner_idx=0; inner_idx < params->eviction_sets[outer_idx].eviction_sets_len;inner_idx++) {
            memset((void*)inner_array[inner_idx], 0, 64);
        }
    }
    if(ioctl(ctx->kvm_fd, KVM_SET_STEP_IMPORT_USER_EVS, params) < 0) {
        perror("sev_step_import_user_evs: Error calling KVM_SET_STEP_IMPORT_USER_EVS ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int sev_step_free_evs(usp_poll_api_ctx_t* ctx) {
    if(ioctl(ctx->kvm_fd, KVM_SEV_STEP_FREE_EVS, NULL) < 0) {
        perror("sev_step_free_evs: Error calling KVM_SEV_STEP_FREE_EVS ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int sev_step_build_alias_evs(usp_poll_api_ctx_t* ctx,build_eviction_set_param_t* build_ev_req) {
     if(ioctl(ctx->kvm_fd, KVM_SEV_STEP_BUILD_ALIAS_EVS, build_ev_req) < 0) {
        perror("sev_step_build_alias_evs: Error calling KVM_SEV_STEP_BUILD_ALIAS_EVS ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;  
}

int sev_step_do_cache_attack_next_step(usp_poll_api_ctx_t* ctx, uint64_t lookup_table_idx,
    bool use_custom_apic_timer_value, uint32_t custom_apic_timer_value) {
    do_cache_attack_param_t params = {
        .lookup_table_index = lookup_table_idx,
        .apic_timer_value_valid = use_custom_apic_timer_value,
        .custom_apic_timer_value = custom_apic_timer_value,
    };
    if(ioctl(ctx->kvm_fd, KVM_SEV_STEP_DO_CACHE_ATTACK_NEXT_STEP, &params) < 0) {
        perror("sev_step_do_cache_attack_next_step: Error calling KVM_SEV_STEP_DO_CACHE_ATTACK_NEXT_STEP ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int sev_step_gpa_to_hpa(usp_poll_api_ctx_t* ctx, uint64_t gpa, uint64_t *hpa) {
    gpa_to_hpa_param_t params = {
        .in_gpa = gpa,
        .out_hpa = 0
    };
    if(ioctl(ctx->kvm_fd, KVM_SEV_STEP_GPA_TO_HPA, &params) < 0) {
        perror("sev_step_gpa_to_hpa: Error calling KVM_SEV_STEP_GPA_TO_HPA ioctl");
        return SEV_STEP_ERR;
    }
    if( params.out_hpa == 0 ) {
        perror("sev_step_gpa_to_hpa: out_hpa was zero. This should never happen");
        return SEV_STEP_ERR;
    }
    (*hpa) = params.out_hpa;
    return SEV_STEP_OK;

}

int sev_step_cache_attack_testbed(usp_poll_api_ctx_t* ctx) {
    if(ioctl(ctx->kvm_fd, KVM_SEV_STEP_CACHE_ATTACK_TESTBED, NULL) < 0) {
        perror("sev_step_cache_attack_testbed: Error calling KVM_SEV_STEP_CACHE_ATTACK_TESTBED ioctl");
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

char* tracking_mode_to_string(enum kvm_page_track_mode mode) {
    switch (mode)
    {
    case KVM_PAGE_TRACK_WRITE:
        return "KVM_PAGE_TRACK_WRITE";
    case KVM_PAGE_TRACK_ACCESS:
        return "KVM_PAGE_TRACK_ACCESS";
    case KVM_PAGE_TRACK_RESET_ACCESSED:
        return "KVM_PAGE_TRACK_RESET_ACCESSED";
    case KVM_PAGE_TRACK_EXEC:
        return "KVM_PAGE_TRACK_EXEC";
    case KVM_PAGE_TRACK_MAX:
        return "KVM_PAGE_TRACK_MAX";
    case KVM_PAGE_TRACK_RESET_EXEC:
        return "KVM_PAGE_TRACK_RESET_EXEC";
    }
}