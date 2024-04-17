
#define _XOPEN_SOURCE
#include "time.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "sev_step_cache_attack_log.h"

#include "sev_step_print_macros.h"
#include "ansi-color-codes.h"
#include "sev_step_api.h"




int timestamped_filename(char* prefix, char** result) {
    //get time and convert to structured format
    time_t now = time(NULL);
    struct tm now_tm;
    localtime_r(&now, &now_tm);

    //create filename
    uint64_t filename_max_len = strlen(prefix) + 300;
    char* filename = malloc( sizeof(char) * filename_max_len);

    int used_chars = snprintf(filename, filename_max_len, "%s_%02d-%02d-%04d_%02d-%02d-%02d.json",
        prefix,
        now_tm.tm_mday, now_tm.tm_mon, 1900 + now_tm.tm_year,
        now_tm.tm_hour,now_tm.tm_min, now_tm.tm_sec
    );
    if( used_chars < 0 ) {
        flf_printf("snprintf for filename failed FAILED\n");
        free(filename);
        return SEV_STEP_ERR;
    }

    if(  (uint64_t)used_chars > filename_max_len ) {
        flf_printf("snprintf for filename failed FAILED. %d bytes would overflow\n",
            used_chars
        );
        free(filename);
        return SEV_STEP_ERR;
    }

    *result = filename;
    return SEV_STEP_OK;

}

cache_attack_logfile_t* cache_attack_log_new_log(lookup_table_t* lookup_tables, uint64_t lookup_tables_len, char* perfctr_name,
    uint64_t way_count) {

    //serialize lookup_tables to json
    json_object* json_lookup_tables = json_object_new_array_ext(lookup_tables_len);
    if( json_lookup_tables == NULL ) {
        flf_printf("json_object_new_array_ext(%ju) failed\n",lookup_tables_len);
        return NULL;
    }

    for(uint64_t idx = 0; idx < lookup_tables_len; idx++) {

        json_object* json_lookup_table = json_object_new_object();
        if( json_lookup_table == NULL ) {
            flf_printf("json_object_new_object failed\n");
            return NULL;
        }
        json_object* addr = json_object_new_uint64(lookup_tables[idx].base_vaddr_table);
        if( addr == NULL ) {
            flf_printf("json_object_new_uint64 base_vaddr_table failed\n");
            return NULL;
        }
        json_object* bytes = json_object_new_uint64(lookup_tables[idx].table_bytes);
        if( bytes == NULL ) {
            flf_printf("json_object_new_uint64 table_bytes failed\n");
            return NULL;
        }
        if( json_object_object_add(json_lookup_table, "addr", addr) ) {
            flf_printf("json_object_object_add addr failed\n");
            return NULL;
        }
        if( json_object_object_add(json_lookup_table, "bytes", bytes) ) {
            flf_printf("json_object_object_add bytes failed\n");
            return NULL;
        }

        if( json_object_array_add(json_lookup_tables, json_lookup_table) ) {
             flf_printf("json_object_array_add json_lookup_table failed\n");
            return NULL;
        }
    }

    json_object* root = json_object_new_object();
    if( root == NULL ) {
        flf_printf("json_object_new_object root failed\n");
        return NULL;
    }

    if( json_object_object_add(root, "lookup_tables", json_lookup_tables) ) {
        flf_printf("json_object_object_add lookup_tables failed\n");
        return NULL;
    }

    if( json_object_object_add(root, "perfctr_name", json_object_new_string(perfctr_name)) ) {
        flf_printf("json_object_object_add perfctr_name failed\n");
        return NULL;
    }
    if( json_object_object_add(root, "way_count", json_object_new_uint64(way_count)) ) {
        flf_printf("json_object_object_add way_count failed\n");
        return NULL;
    }

   

    if( json_object_object_add(root, "traces", json_object_new_array()) ) {
        flf_printf("json_object_object_add traces failed\n");
        return NULL;
    }

    cache_attack_logfile_t* result = malloc(sizeof(cache_attack_logfile_t));
    result->root = root;
    return result;
}

int cache_attack_log_add_trace(cache_attack_logfile_t* l, uint64_t lookup_table_idx, uint64_t* timings,
    uint64_t* perf_diffs, uint64_t len, uint64_t expected_offset,uint64_t step_size, uint64_t memaccess_id, uint64_t* register_values) {

    json_object* traces;
    if( !json_object_object_get_ex(l->root, "traces", &traces) ) {
        flf_printf("failed to get traces field from cache_attack_logfile_t. This should never happen\n");
        return SEV_STEP_ERR;
    }

    //create new trace object and add simple attributes
    json_object* trace = json_object_new_object();
    if( trace == NULL ) {
        flf_printf("json_object_new_object trace failed\n");
        return SEV_STEP_ERR;
    }

    json_object* json_lookup_table_idx = json_object_new_uint64(lookup_table_idx);
    if( json_lookup_table_idx == NULL ) {
        flf_printf("json_object_new_uint64 json_lookup_table_idx failed");
        return SEV_STEP_ERR;
    }

    if( json_object_object_add(trace, "lookup_table_idx", json_lookup_table_idx) ) {
        flf_printf("json_object_object_add lookup_table_idx failed\n");
        return SEV_STEP_ERR;
    }

    json_object* json_expected_offset = json_object_new_uint64(expected_offset);
    if( json_expected_offset == NULL ) {
        flf_printf("json_object_new_uint64 expected_offset failed\n");
        return SEV_STEP_ERR;
    }
    if( json_object_object_add(trace, "expected_offset", json_expected_offset) ) {
        flf_printf("json_object_object_add expected_offset failed\n");
        return SEV_STEP_ERR;
    }

    json_object* json_step_size = json_object_new_uint64(step_size);
    if( json_step_size == NULL ) {
        flf_printf("json_object_new_uint64 step_size failed\n");
        return SEV_STEP_ERR;
    }
    if( json_object_object_add(trace, "step_size", json_step_size)) { 
        flf_printf("json_object_object_add step_size failed\n");
        return SEV_STEP_ERR;
    }


    json_object* json_memaccess_id = json_object_new_uint64(memaccess_id);
    if( json_memaccess_id == NULL ) {
        flf_printf("json_object_new_uint64 memaccess_id failed\n");
        return SEV_STEP_ERR;
    }
    if( json_object_object_add(trace, "memaccess_id", json_memaccess_id)) { 
        flf_printf("json_object_object_add memaccess_id failed\n");
        return SEV_STEP_ERR;
    }


    //convert timings and perf_diffs arrays to json arrays and add them to trace object
    json_object* json_timings = json_object_new_array_ext(len);
    if( json_timings == NULL ) {
        flf_printf("json_object_new_array_ext(%ju) json_timings failed\n", len);
        return SEV_STEP_ERR;
    }

    json_object* json_perf_diffs = json_object_new_array_ext(len);
    if( json_perf_diffs == NULL ) {
        flf_printf("json_object_new_array_ext(%ju) json_perf_diffs failed\n", len);
        return SEV_STEP_ERR;
    }

    for( uint64_t idx = 0; idx < len; idx++ ) {
        json_object* t = json_object_new_uint64(timings[idx]);
        if( t == NULL ) {
            flf_printf("json_object_new failed\n");
            return SEV_STEP_ERR;
        }
        if( json_object_array_add(json_timings, t) ) {
            flf_printf("json_object_array_add failed\n");
            return SEV_STEP_ERR;
        }

        json_object* pd = json_object_new_uint64(perf_diffs[idx]);
        if( pd == NULL ) {
            flf_printf("json_object_new_uint64 failed");
            return SEV_STEP_ERR;
        }

        if( json_object_array_add(json_perf_diffs, pd) ) {
            flf_printf("json_object_array_add failed\n");
            return SEV_STEP_ERR;            
        }

    }

    if( json_object_object_add(trace, "timings", json_timings) ) {
        flf_printf("json_object_object_add failed\n");
        return SEV_STEP_ERR;         
    }

    if( json_object_object_add(trace, "perf_diffs", json_perf_diffs) ) {
        flf_printf("json_object_object_add failed\n");
        return SEV_STEP_ERR; 
    }

    if( register_values != NULL ) {
        json_object* json_register_values = json_object_new_object();
        if( json_register_values == NULL ) {
            flf_printf("json_object_new_object json_register_values failed\n");
            return SEV_STEP_ERR;
        }
        for(int i=0; i < VRN_MAX; i++) {
            char* reg_name = vmsa_register_name_to_str(i);
            if( reg_name == NULL ) {
                flf_printf("vmsa_register_name_to_str failed for value %d\n",i);
                return SEV_STEP_ERR;
            }

            json_object* json_reg_value = json_object_new_uint64(register_values[i]);
            if( json_reg_value == NULL ) {
                flf_printf("json_object_new_uint64 json_reg_value failed\n");
                return SEV_STEP_ERR;
            }
  

            if(json_object_object_add(json_register_values, reg_name, json_reg_value)) {
                flf_printf("json_object_add failed\n");
                return SEV_STEP_ERR;
            }
        }

        if(json_object_object_add(trace, "register values", json_register_values)) {
            flf_printf("json_object_add failed\n");
            return SEV_STEP_ERR;
        }
    }


    //add trace object to traces
    if( json_object_array_add(traces, trace) ) {
        flf_printf("json_object_object_add failed\n");
        return SEV_STEP_ERR; 
    }


    return SEV_STEP_OK;

}


int cache_attack_log_to_file(cache_attack_logfile_t* l, char* filename_prefix) {

    char* filename;
    if( SEV_STEP_ERR == timestamped_filename(filename_prefix,&filename) ) {
        flf_printf("timestamped_filename failed\n");
        return SEV_STEP_ERR;
    }
    if( -1 == json_object_to_file(filename, l->root) ) {
        flf_printf("json_object_to_file failed : %s", json_util_get_last_err());
        free(filename);
        return SEV_STEP_ERR;
    }

    free(filename);
    return SEV_STEP_OK;
}

void cache_attack_log_free(cache_attack_logfile_t* l) {
    if( l == NULL ) {
        return;
    }
    if( l->root != NULL ) {
        json_object_put(l->root);
    }
    free(l);
}