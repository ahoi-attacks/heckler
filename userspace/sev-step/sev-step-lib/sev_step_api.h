/**
 * @file sev_step_api.h
 * @author Luca Wilke (l.wilke@its.uni-luebeck.de)
 * @brief User space code for consuming the kernel API.
 * Move more complex functionality to sev_step_routines.{h,c}
 * @date 2023-01-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _SEV_STEP_API_H
#define _SEV_STEP_API_H

#include "linux/kvm.h"
#include "linux/sev-step/sev-step.h"

#include <stdint.h>
#include <stdbool.h>


#include "sev_step_error_codes.h"

char* descriptive_perf_name(perf_ctl_config_t cfg);

/// @brief L2 Cache Hit From L1 Data Cache Miss Event for Host in OS Mode
extern perf_ctl_config_t host_os_l2_hit_from_l1_dc_miss;
extern perf_ctl_config_t host_os_l2_miss_from_l1_dc_miss;

extern perf_ctl_config_t host_ost_l2_cache_miss_from_l2_cache_hwpf_1;
extern perf_ctl_config_t host_ost_l2_cache_miss_from_l2_cache_hwpf_2;

extern perf_ctl_config_t host_os_l1d_fill_all;
extern perf_ctl_config_t host_os_l1d_fill_from_memory;
extern perf_ctl_config_t host_os_all_data_cache_accesses;




/**
 * @brief Converts a register name from string to vmsa_register_name_t for
 * lookup in sev_step_partial_vmcb_save_area_t.register_values
 * 
 * @param c name of the register as str. Not case sensitive
 * @param result if SEV_STEP_OK, this contains the vmsa_register_name_t for the
 * register described by c
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int vmsa_register_name_from_str(char* c, vmsa_register_name_t* result);
/**
 * @brief Returns the name of reg as a string
 * 
 * @param reg enum value for the register
 * @return char* statically allocated string with the register name, i.e. no need to free.
 * If there is not match for reg, NULL is returned
 */
char* vmsa_register_name_to_str(vmsa_register_name_t reg);


typedef struct {
    int pid;
    shared_mem_region_t* shared_mem_region;
    uint64_t next_id;
    int force_reset;
    
    //just for internal use. Used to remember get_user_pages_unlocked
    //page to be able to unpinn it on ctx destruction
    struct page* _page_for_shared_mem;
    int kvm_fd;
    
    /// @brief we use this to track if the VM should be in a halted/paused
    /// state based on our event receive function and our ack function
    bool vm_is_paused;
} usp_poll_api_ctx_t;


//
// Convenience Functions
//

void print_single_step_event(char* fp, sev_step_event_t* event);


//
// SEV-STEP Functions
//
/**
 * @brief
 *
 * @param ctx
 * @param got_event
 * @param event_type
 * @param event Callers must free with usp_free_event
 * @return int
 */
int usp_poll_event(usp_poll_api_ctx_t* ctx, int* got_event, usp_event_type_t* event_type, void** event);
void free_usp_event(usp_event_type_t type, void *event);
void usp_ack_event(usp_poll_api_ctx_t *ctx);
int usp_block_until_event(usp_poll_api_ctx_t* ctx, usp_event_type_t* event_type, void** event);
/**
 * @brief Blocks until event is received or SIGINT is received or should_abort(should_abort_args) returns true.
 * 
 * 
 * @param ctx 
 * @param event_type 
 * @param event 
 * @param should_abort 
 * @param should_abort_args 
 * @return int SEV_STEP_OK if event is received, SEV_STEP_ERR_ABORT if should_abort evaluates to true
 * , SEV_STEP_ERR if interrupted by SIGINT or other error occurs
 */
int usp_block_until_event_or_cb(usp_poll_api_ctx_t* ctx, usp_event_type_t* event_type, void** event, bool(should_abort)(void*), void* should_abort_args);
int get_size_for_event(usp_event_type_t event_type, uint64_t* size);

int enable_boot_track(usp_poll_api_ctx_t *ctx, int enabled);

int track_page(usp_poll_api_ctx_t *ctx, uint64_t gpa, enum kvm_page_track_mode mode);
int untrack_page(usp_poll_api_ctx_t *ctx, uint64_t gpa, enum kvm_page_track_mode mode);
int track_all_pages(usp_poll_api_ctx_t *ctx, enum kvm_page_track_mode mode);
int untrack_all_pages(usp_poll_api_ctx_t *ctx, enum kvm_page_track_mode mode);

int inject_interrupt(usp_poll_api_ctx_t *ctx, int number);

/**
 * @brief Helper function that returns true if the VM should currently be halted. This intended as a sanity check when splitting complex attack logic
 * through multiple function, e.g. you can check that before and after a
 * subfunction call the VM is in a paused state.
 * 
 * Note: There is no hard link to the kernel state. If we receive an event
 * we set this to true and expect it to stay true until we call usp_ack_event(ctx) or usp_close_ctx(ctx). However, if there is some bug
 * in the kernel counterpart this might not be reliable.
 * 
 * @param ctx 
 * @return true if VM should be paused
 * @return false otherwise
 */
bool is_vm_paused(usp_poll_api_ctx_t* ctx);
/**
 * @brief Open new api connection
 * 
 * @param ctx 
 * @param debug_mode If true, expect the VM to run in debug mode and instruct the kernel counterpart
 * to decrypt the VM's VMCB at each event and send the data with the event
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int usp_new_ctx(usp_poll_api_ctx_t* ctx, bool debug_mode);
int usp_new_ctx_boot(usp_poll_api_ctx_t* ctx, bool debug_mode, bool boot_track);
int usp_close_ctx(usp_poll_api_ctx_t *ctx);
/**
 * @brief Enable single stepping upon next vmentry
 * 
 * @param ctx api ctx to operate on
 * @param timer_value timeout for apic timer
 * @param gpas_target_pages May be null. If set, we reset the ACCESS bits of these pages before vmentry
 * @param gpas_target_pages_len 
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int enable_single_stepping(usp_poll_api_ctx_t *ctx,uint32_t timer_value,uint64_t* gpas_target_pages,uint64_t gpas_target_pages_len);
/**
 * @brief Like enable_single_stepping but exposes additional config options
 *
 * Additional params compared to enable_single_stepping
 * @param tlb_flush If true, flush guest tlb, else not
 * @return int
 */
int enable_single_stepping_ex(usp_poll_api_ctx_t *ctx, uint32_t timer_value, uint64_t *gpas_target_pages, uint64_t gpas_target_pages_len, bool tlb_flush);

int disable_single_stepping(usp_poll_api_ctx_t *ctx);

/**
 * @brief Inject an nmi into vm upon next vmentry
 * 
 * @param ctx 
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int inject_nmi(usp_poll_api_ctx_t* ctx);

/**
 * @brief  "Imports" an eviction set build in userspace by pinning the pages and creating mappings
 * to the underlying pages, so that we can use the eviction set anywhere in the kernel space.
 * Note: Does not work for Way predictor based eviction sets, as these depend on the virtual address, which
 * changes when we create our kernel space mapping (in constrast to the underlying physical page, which stays the same)
 * @param ctx 
 * @param params see struct definitions for docuementation. Don't free eviction set memory
 * until sev_step_free_evs was called
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_import_user_evs(usp_poll_api_ctx_t* ctx, import_user_eviction_set_param_t* params);

/**
 * @brief Free the currently used eviction set in the kernel (if there is any)
 * 
 * @param ctx 
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_free_evs(usp_poll_api_ctx_t* ctx);

/**
 * @brief Request kernel to build aliasing eviction set for load + reload attack from 
 * "Take A Way" paper
 * 
 * @param ctx 
 * @param build_ev_req params for cache attack
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_build_alias_evs(usp_poll_api_ctx_t* ctx,build_eviction_set_param_t* build_ev_req);

/**
 * @brief Perform a cache attack on the next (single) step. Eviction set must already be loaded.
 * Result will be part of the step event
 * 
 * @param ctx 
 * @param lookup_table_idx idx of lookup table to attack
 * @param use_custom_apic_timer_value if, true, custom_apic_timer_value will be used for apic timer
 * when performing this cache attack
 * @param custom_apic_timer_value if use_custom_apic_timer_value is set, use this value for the apic timer
 * for the cache attack
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_do_cache_attack_next_step(usp_poll_api_ctx_t* ctx, uint64_t lookup_table_idx,
    bool use_custom_apic_timer_value, uint32_t custom_apic_timer_value);

/**
 * @brief Translate GPA to HPA. This is e.g. required for eviction sets that are based
 * on phyiscal addresses
 * 
 * @param ctx 
 * @param gpa we want the host physical address for this guest physical address
 * @param hpa Result param
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int sev_step_gpa_to_hpa(usp_poll_api_ctx_t* ctx, uint64_t gpa, uint64_t *hpa);


int sev_step_cache_attack_testbed(usp_poll_api_ctx_t* ctx);

//
//Helper functions
//
char* tracking_mode_to_string(enum kvm_page_track_mode mode);

void usp_block_stop(void);
#endif