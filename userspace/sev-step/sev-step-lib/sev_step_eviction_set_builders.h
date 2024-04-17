#ifndef SEV_STEP_EVICTION_SET_BUILDERS
#define SEV_STEP_EVICTION_SET_BUILDERS

#include <stdint.h>

#include "sev_step_api.h"

/**
 * @brief Builds an l2 eviction set for the AMD EPYC 7003 Series
 * 
 * @param victim_hpa addr to evict. In SEV case this is the host physical address
 * for the guest physical address where the lookup table is located
 * @param eviction_set Callee allocated result param.
 * Contains the address values of the eviction set.
 * Free with epyc3_free_l2_eviction_set.
 * @param eviction_set_len Result param. Length of eviction_set array
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int epyc3_build_l2_eviction_set(uint64_t victim_hpa,
    uint64_t** eviction_set, uint64_t* eviction_set_len);

/**
 * @brief Frees evictions sets allocated with epyc3_build_l2_eviction_set
 * 
 * @param eviction_set 
 * @param eviction_set_len 
 */
void epyc3_free_l2_eviction_set(uint64_t* eviction_set,
    uint64_t eviction_set_len);


/**
 * @brief Build an eviction set for each cacheline contained by the lookup table using
 * epyc3_build_l2_eviction_set
 * 
 * @param lookup_table_hpa host physical address of lookup table for which we want the eviction set
 * @param lookup_table_bytes size of in lookup_table_hpa bytes
 * @param result result param. Inner values allocated by callee.
 * Free with epyc3_free_lookup_table_eviction_set_t
 * @param way_count result param
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int epyc3_build_l2_eviction_set_for_lookup_table(uint64_t lookup_table_hpa, uint64_t lookup_table_bytes,
    lookup_table_eviction_set_t* result, uint64_t* way_count);

/**
 * @brief Build an eviction set for each cacheline contained by the lookup table.
 *  We only search for way count addrs that collided with the pfn bits of lookup_table_hpa and re-use them in the eviction
 * sets for the individual cache lines covered by the lookup table
 * 
 * @param lookup_table_hpas host physical addresses backing the lookup table. Order must correspond to order in virtual address space
 * @param lookup_table_hpas length of lenlookup_table_hpas
 * @param lookup_table_bytes size of in lookup_table_hpa bytes
 * @param result result param. Inner values allocated by callee. Free with epyc3_free_reuse_lookup_table_eviction_set_t
 * @param base_mappings Callee allocated result param. Filled with base mappings backing the eviction set. Free with epyc3_free_reuse_lookup_table_eviction_set_t
 * @param base_mappings_len Result param with the length of base_mappings. Convenience value. Should always be len(lookup_table_hpas) * way_count
 * @param way_count way count of the cache
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse(uint64_t* lookup_table_hpas,uint64_t lookup_table_hpas_len, uint64_t lookup_table_bytes,
    lookup_table_eviction_set_t* result, uint8_t*** base_mappings,uint64_t* base_mappings_len ,uint64_t way_count);

/**
 * @brief Split eviction set for lookup table into multiple sub eviction set. Convenience functions. Useful because it is faster to first build
 * a large ev and then split it, as we can reuse the backing pages.
 * lut = lookup table
 * @param combined_lut Some lookup table
 * @param combined_ev Eviction set for combined_lut
 * @param sub_luts Multiple luts that in total cover combined_lut. Each lut must be cacheline aligned
 * @param sub_luts_len length of sub_luts
 * @param sub_evs Callee allocated result param of length sub_luts_len. Each entry contains the parts from combined_ev that form an ev
 * for the corresponding sub_luts entry. Free with free_sub_evs
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int split_large_ev_into_separate_luts(lookup_table_t combined_lut, lookup_table_eviction_set_t* combined_ev, 
    lookup_table_t* sub_luts, uint64_t sub_luts_len, lookup_table_eviction_set_t** sub_evs, uint64_t way_count);

/**
 * @brief Frees sub_evs result param from split_large_ev_into_separate_luts
 * 
 * @param sub_evs as returned from split_large_ev_into_separate_luts
 * @param sub_evs_len same as sub_luts_len param used for split_large_ev_into_separate_luts call
 */
void free_sub_evs(lookup_table_eviction_set_t* sub_evs, uint64_t sub_evs_len );
/**
 * @brief Free callee allocated results prams of epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse
 * 
 * @param evs "result" param returned by epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse
 * @param base_mappings as returned by epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse
 * @param base_mappings_len as returned by epyc3_build_l2_eviction_set_for_lookup_table_parallel_reuse
 */
void epyc3_free_reuse_lookup_table_eviction_set(lookup_table_eviction_set_t evs, uint8_t** base_mappings, uint64_t base_mappings_len);

/**
 * @brief Free lookup_table_eviction_set_t allocated by
 * epyc3_build_l2_eviction_set_for_lookup_table
 * 
 * @param evs 
 */
void epyc3_free_lookup_table_eviction_set_t(lookup_table_eviction_set_t evs);
#endif