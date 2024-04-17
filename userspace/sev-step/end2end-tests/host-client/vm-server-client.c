#include "vm-server-client.h"
#include "../../sev-step-lib/sev_step_http_client.h"
#include "../../sev-step-lib/sev_step_error_codes.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>


#include "my-error-codes.h"
#include "json-c/json.h"





/**
 * @brief Call pingpong-pagetrack/init endpoint. This will give us the gpa of two pages
 * who will be accessed via tracking_type once vm_server_pagetrack_start has been called.
 * 
 * @param tracking_type either "READ", "WRITE" or "EXEC"
 * @param result Caller allocated. Filled with resulting gpas
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int vm_server_pagetrack_init(char* tracking_type, pagetrack_gpas_t* result) {
    json_object* json_in;
    json_object* json_out;

    //Prepare request data
    json_in = json_object_new_object();
    json_object_object_add(json_in, "access_type",json_object_new_string(tracking_type));
    char* url = "http://localhost:8080/pingpong-pagetrack/init";

    //issue request via helper function
    //printf("calling curl_post_json\n");
    if( SEV_STEP_ERR == curl_post_json(url,json_in,&json_out) ) {
        fprintf(stderr, "ERROR: curl_post_json to %s failed\n",url);
        json_object_put(json_in);
        return HOST_CLIENT_ERROR;
    }
    json_object_put(json_in);
    //printf("curl_post_json returned\n");
     /* debugging */
    //printf("vm_server_pagetrack_init: Parsed JSON: %s\n", json_object_to_json_string(json_out));

    //parse json result
    if( SEV_STEP_ERR == json_uint64_helper(json_out,"gpa1",&(result->gpa1) ) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }
    if( SEV_STEP_ERR == json_uint64_helper(json_out,"gpa2",&(result->gpa2) ) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }



    json_object_put(json_out);

    return HOST_CLIENT_SUCCESS;
}

/**
 * @brief Call pingpong-pagetrack/start endpoint. You need to call vm_server_pagetrack_init first
 * 
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int vm_server_pagetrack_start() {
    json_object* json_in;

    //Prepare request data
    json_in = json_object_new_object();
    char* url = "http://localhost:8080/pingpong-pagetrack/start";

    //issue request via helper function
    //printf("calling curl_post_json\n");
    if( SEV_STEP_ERR == curl_post_json(url,json_in,NULL) ) {
        fprintf(stderr, "ERROR: curl_post_json to %s failed\n",url);
        json_object_put(json_in);
        return HOST_CLIENT_ERROR;
    }
    json_object_put(json_in);

    return HOST_CLIENT_SUCCESS;
}


/**
 * @brief Call pingpong-pagetrack/teardown endpoint
 * 
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int vm_server_pagetrack_teardown() {
    json_object* json_in;

    //Prepare request data
    json_in = json_object_new_object();
    char* url = "http://localhost:8080/pingpong-pagetrack/teardown";

    //issue request via helper function
    //printf("calling curl_post_json\n");
    if( SEV_STEP_ERR == curl_post_json(url,json_in,NULL) ) {
        fprintf(stderr, "ERROR: curl_post_json to %s failed\n",url);
        json_object_put(json_in);
        return HOST_CLIENT_ERROR;
    }
    json_object_put(json_in);

    return HOST_CLIENT_SUCCESS;
}


int victim_program_to_str(victim_program_t program,char** res) {
    switch (program) {
    case VICTIM_PROGRAM_NOP_SLIDE:
        *res = "NopSlide";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_ADD_SLIDE:
        *res = "AddSlide";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_RDRAND64_SLIDE:
        *res = "Rdrand64Slide";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_RDRAND32_SLIDE:
        *res = "Rdrand32Slide";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_DIVR16_SLIDE:
        *res = "DivSlideR16";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_DIVR32_SLIDE:
        *res = "DivSlideR32";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_DIVR64_SLIDE0:
        *res = "DivSlide0R64";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_DIVR64_SLIDE1:
        *res = "DivSlide1R64";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_DIVR64_SLIDE2:
        *res = "DivSlide2R64";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_DIVR64_SLIDE3:
        *res = "DivSlide3R64";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_DIVR64_SLIDE4:
        *res = "DivSlide4R64";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_MULR64_SLIDE:
        *res = "MulSlideR64";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_RCL_SLIDE:
	*res = "RclSlide";
	return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_LAR_SLIDE:
	*res = "LarSlide";
	return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_SIMPLE_CACHE_VICTIM:
        *res = "SimpleCacheTarget";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_SIMPLE_CACHE_VICTIM_LFENCE:
        *res = "SimpleCacheTargetLfence";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_EVAL_CACHE_VICTIM:
        *res = "EvalCacheTarget";
        return HOST_CLIENT_SUCCESS;
    case VICTIM_PROGRAM_EVAL_CACHE_VICTIM_LFENCE:
        *res = "EvalCacheTargetLfence";
        return HOST_CLIENT_SUCCESS;
    default:
        fprintf(stderr, "ERROR: invalid program value\n");
        return HOST_CLIENT_ERROR;
    }
}



//TODO: implement free for result struct
/**
 * @brief Get addresses and other meta-data required to track the execution of the 
 * victim program
 * Currently fixed to NopSlide. Will expand in future
 *
 * @param program selects which program the remote should prepare 
 * @param response data required to track the execution of the program
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int vm_server_single_step_victim_init(victim_program_t program, victim_program_data_t* response) {
    json_object* json_in;
    json_object* json_out;
    char* program_name;

    if( HOST_CLIENT_ERROR == victim_program_to_str(program,&program_name) ) {
        fprintf(stderr,"victim_program_to_str failed\n");
        return HOST_CLIENT_ERROR;
    }

    //Prepare requests data
    json_in = json_object_new_object();
    json_object_object_add(json_in, "victim_program",json_object_new_string(program_name));
    char* url = "http://localhost:8080/single-step-victim/init";

    //do request
    if( SEV_STEP_ERR == curl_post_json(url,json_in, &json_out)) {
        fprintf(stderr, "ERROR: curl_post_json to %s failed\n",url);
        json_object_put(json_in);
        return HOST_CLIENT_ERROR;
    }
    json_object_put(json_in);

    //parse response
    if( SEV_STEP_ERR == json_uint64_helper(json_out,"gpa",&(response->gpa) ) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }
    if( SEV_STEP_ERR == json_uint64_helper(json_out,"vaddr",&(response->vaddr) ) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }
    if( SEV_STEP_ERR == json_uint64_array_helper(json_out,"expected_offsets",&response->expected_offsets,&response->expected_offsets_len) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }

    //check if we have optional cache attack data
    bool have_cache_attack_data;
    if( SEV_STEP_ERR == json_bool_helper(json_out,"has_cache_attack_data",&have_cache_attack_data ) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }
    if( !have_cache_attack_data) {
        response->cache_attack_data = NULL;
        json_object_put(json_out);
        return HOST_CLIENT_SUCCESS;
    }

    //if we are here, we have the optional cache attack data -> parse it
    cache_attack_data_t* cache_data = malloc(sizeof(cache_attack_data_t));
    if( SEV_STEP_ERR == json_uint64_helper(json_out,"lookup_table_gpa",&cache_data->lookup_table_gpa ) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }
    if( SEV_STEP_ERR == json_uint64_helper(json_out,"lookup_table_vaddr",&cache_data->lookup_table_vaddr ) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }
    if( SEV_STEP_ERR == json_uint64_helper(json_out,"lookup_table_bytes",&cache_data->lookup_table_bytes ) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }
    uint64_t offsets_with_mem_access_len;
    if( SEV_STEP_ERR == json_uint64_array_helper(json_out,"offsets_with_mem_access",&cache_data->offsets_with_mem_access,&offsets_with_mem_access_len) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }
    uint64_t mem_access_target_offset_len;
    if( SEV_STEP_ERR == json_uint64_array_helper(json_out,"mem_access_target_offset",&cache_data->mem_access_target_offset,&mem_access_target_offset_len) ) {
        fprintf(stderr, "ERROR: failed to parse json output\n");
        return HOST_CLIENT_ERROR;
    }
    json_object_put(json_out);


    if(offsets_with_mem_access_len !=  mem_access_target_offset_len ) {
        fprintf(stderr, "%s:%d ASSERTION ERROR: expected offsets_with_mem_access_len and mem_access_target_offset_len to have same length"
            "but got %lu and %lu\n",__FILE__,__LINE__,offsets_with_mem_access_len,mem_access_target_offset_len);
        return HOST_CLIENT_ERROR;
    }
    cache_data->len = offsets_with_mem_access_len;
    response->cache_attack_data = cache_data;


    return HOST_CLIENT_SUCCESS;
}

/**
 * @brief Start the program on the VM. Program will run async, i.e. this call will return before
 * the program on the VM finishes. Setup tracking before using this function
 *  
 * @param program selects which program the remote should start 
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int vm_server_single_step_victim_start(victim_program_t program) {
    json_object* json_in;
    char* program_name;

    if( HOST_CLIENT_ERROR == victim_program_to_str(program,&program_name) ) {
        fprintf(stderr,"victim_program_to_str failed\n");
        return HOST_CLIENT_ERROR;
    }

    //Prepare request data
    json_in = json_object_new_object();
    json_object_object_add(json_in, "victim_program",json_object_new_string(program_name));
    char* url = "http://localhost:8080/single-step-victim/start";

    //do request
    if( SEV_STEP_ERR == curl_post_json(url,json_in,NULL) ) {
        fprintf(stderr, "ERROR: curl_post_json to %s failed\n",url);
        json_object_put(json_in);
        return HOST_CLIENT_ERROR;
    }
    json_object_put(json_in);

    return HOST_CLIENT_SUCCESS;
}

void free_victim_program_data_t(victim_program_data_t* v) {
    if( v->expected_offsets != NULL ) {
        free(v->expected_offsets);
    }
    if( v->cache_attack_data != NULL ) {
        free(v->cache_attack_data->offsets_with_mem_access);
        free(v->cache_attack_data->mem_access_target_offset);
        free(v->cache_attack_data);
    }
    free(v);
}