#ifndef SEV_STEP_HTTP_CLIENT_H
#define SEV_STEP_HTTP_CLIENT_H

#include <stdbool.h>
#include "curl/curl.h"
#include "json-c/json.h"

/* holder for curl fetch */
struct curl_fetch_st {
    char *payload;
    size_t size;
};

CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch);


/**
 * @brief Performs a post request using the given json body and returns the result as json in case of success.
 * 
 * @param url url to perform post to
 * @param json_in json for request body. Caller allocated/managed
 * @param json_out Result param for json result in case the post was successful. IF NULL, we do not try to parse result.
 * Allocated in this function. If functions succeeds you need to free this with 
 * `json_object_put(json_out)`
 * @return SEV_STEP_OK or SEV_STEP_ERROR 
 */
int curl_post_json(char* url, json_object* json_in, json_object** json_out);

/**
 * @brief Tries to parse key from json object as bool
 * 
 * @param in json object
 * @param key whose value we try to parse as bool
 * @param result on success, this contains the resulting bool value
 * @return int SEV_STEP_OK or SEV_STEP_ERROR
 */
int json_bool_helper(json_object* json_data,char* key, bool* result);

/**
 * @brief Tries to parse key from json object as string
 * 
 * @param in json object
 * @param key whose value we try to parse as bool
 * @param result on success, this contains the resulting string value. Caller must free
 * @return int SEV_STEP_OK or SEV_STEP_ERROR
 */
int json_string_helper(json_object* json_data,char* key, char** result);

/**
 * @brief Tries to parse key from json object as uint64_t
 * 
 * @param in json object
 * @param key whose value we try to parse as uint64_t
 * @param result on success, this contains the resulting uint64_t value
 * @return int SEV_STEP_OK or SEV_STEP_ERROR
 */
int json_uint64_helper(json_object* json_data,char* key, uint64_t* result);


/**
 * @brief Tries to parse key from json object as uint64_t array
 * 
 * @param json_data json object
 * @param key whose value we try to parse as uint64_t array
 * @param result on success, this is filled with callee allocated array containing the parsed uint64_t values
 * @param result_len on success, this contains the size of the callee allocated array in @result
 * @return int SEV_STEP_OK or SEV_STEP_ERROR
 */
int json_uint64_array_helper(json_object* json_data,char* key, uint64_t** result, uint64_t* result_len);


#endif