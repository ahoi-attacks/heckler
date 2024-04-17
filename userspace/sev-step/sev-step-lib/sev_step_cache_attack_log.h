#ifndef SEV_STEP_CACHE_ATTACK_LOG
#define SEV_STEP_CACHE_ATTACK_LOG

#include "sev_step_api.h"
#include <json-c/json.h>

typedef struct {
    json_object *root;
} cache_attack_logfile_t;

/**
 * @brief Creates a string with a formatted timestamp and the given suffix.
 * Intended for using as filename for the logfiles. 
 * 
 * @param prefix prefix to prepend to formatted timestamp
 * @param result Result Param. Caller must free
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int timestamped_filename(char* prefix, char** result);

/**
 * @brief Creates a new cache_attack_logfile_t log for the given configuration. Pass the
 * result to cache_attack_log_add_trace to add traces and finally serialize to a json file
 * using cache_attack_log_to_file. Caller must free returned log with cache_attack_log_free.
 * 
 * @param lookup_tables all lookup tables used in the cache attack
 * @param lookup_tables_len length of lookup_tables array
 * @param perfctr_name Descriptive name for the performance counter monitored by this cache attack
 * @param way_count way count of the attacked cache
 * @return cache_attack_logfile_t* or NULL on error
 */
cache_attack_logfile_t* cache_attack_log_new_log(lookup_table_t* lookup_tables, uint64_t lookup_tables_len, char* perfctr_name,
    uint64_t way_count);

/**
 * @brief Add a new cache attack trace to the log
 * 
 * @param l log to add data to
 * @param lookup_table_idx index of lookup table attacked by this trace.
 * See lookup_tables param of cache_attack_log_new_log
 * @param timings timing data
 * @param perf_diffs  performance counter data
 * @param len length of timings as well ass perf_diffs array (i.e. they must be same length)
 * @param expected_offset offset from start of attacked lookup table where we expect a high access time
 * @param step_size how large the step was between priming and probing. Usually you have zero step or single step
 * @param memaccess_id Caller defined id that e.g. can be used that multiple access belong the the same instance of a memory access.
 * @param register_values Optional. If not null, this must be an array of length VRN_MAX with the register values at the time of the memory access using vmsa_register_name_t for index to register mapping. This is e.g. the case for decrypted_vmsa_data.register_values
 * in the sev step event
 * Intended to record multiple zero step cache traces for the same memory access
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int cache_attack_log_add_trace(cache_attack_logfile_t* l, uint64_t lookup_table_idx, uint64_t* timings,
    uint64_t* perf_diffs, uint64_t len, uint64_t expected_offset,uint64_t step_size, uint64_t memaccess_id, uint64_t* register_values);

/**
 * @brief Serialize log to json and write to file
 * 
 * @param l log to serialize
 * @param filename_prefix file path + prefix for filename. We still append a timestamp and the ".json" ending to this
 */
int cache_attack_log_to_file(cache_attack_logfile_t* l, char* filename_prefix);

void cache_attack_log_free(cache_attack_logfile_t* l);

#endif