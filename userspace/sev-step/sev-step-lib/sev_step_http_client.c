#include "sev_step_http_client.h"
#include "sev_step_error_codes.h"


#include <errno.h>
#include <string.h>



/* callback for curl fetch */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;                             /* calculate buffer size */
    struct curl_fetch_st *p = (struct curl_fetch_st *) userp;   /* cast pointer to fetch struct */

    /* expand buffer using a temporary pointer to avoid memory leaks */
    char * temp = realloc(p->payload, p->size + realsize + 1);

    /* check allocation */
    if (temp == NULL) {
      /* this isn't good */
      fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
      /* free buffer */
      free(p->payload);
      /* return */
      return 1;
    }

    /* assign payload */
    p->payload = temp;

    /* copy contents to buffer */
    memcpy(&(p->payload[p->size]), contents, realsize);

    /* set new buffer size */
    p->size += realsize;

    /* ensure null termination */
    p->payload[p->size] = 0;

    /* return size */
    return realsize;
}

/* fetch and return url body via curl */
CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch) {
    CURLcode rcode;                   /* curl result code */

    /* init payload */
    fetch->payload = (char *) calloc(1, sizeof(fetch->payload));

    /* check payload */
    if (fetch->payload == NULL) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
        /* return error */
        return CURLE_FAILED_INIT;
    }

    /* init size */
    fetch->size = 0;

    /* set url to fetch */
    curl_easy_setopt(ch, CURLOPT_URL, url);

    /* set calback function */
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);

    /* pass fetch struct pointer */
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *) fetch);

    /* set default user agent */
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");


    /* enable location redirects */
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);

    /* set maximum allowed redirects */
    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);

    /* fetch the url */
    rcode = curl_easy_perform(ch);

    /* return */
    return rcode;
}


int curl_post_json(char* url, json_object* json_in, json_object** json_out) {
    //based on https://gist.github.com/leprechau/e6b8fef41a153218e1f4
    CURL *ch;                                               /* curl handle */
    CURLcode rcode;                                         /* curl result code */

    enum json_tokener_error jerr = json_tokener_success;    /* json parse error */

    struct curl_fetch_st curl_fetch;                        /* curl fetch struct */
    struct curl_fetch_st *cf = &curl_fetch;                 /* pointer to fetch struct */
    struct curl_slist *headers = NULL;                      /* http headers to send with request */

    /* url to test site */

    /* init curl handle */
    if ((ch = curl_easy_init()) == NULL) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to create curl handle in fetch_session");
        /* return error */
        return SEV_STEP_ERR;
    }

    /* set content type */
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    

    

     /* set curl options */
    curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(ch, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(ch, CURLOPT_POSTFIELDS, json_object_to_json_string(json_in));

     /* fetch page and capture return code */
    rcode = curl_fetch_url(ch, url, cf);

     /* cleanup curl handle */
    curl_easy_cleanup(ch);

    /* free headers */
    curl_slist_free_all(headers);

   

    /* check return code */
    if (rcode != CURLE_OK || (json_out != NULL && cf->size < 1) ) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to fetch url (%s) - curl said: %s",
            url, curl_easy_strerror(rcode));
        if(cf->payload != NULL ) {
            free(cf->payload);
        }
        /* return error */
        return SEV_STEP_ERR;
    }

    long http_resp_code;
    curl_easy_getinfo(ch,CURLINFO_RESPONSE_CODE,&http_resp_code);
    if ( http_resp_code != 200 ) {
        fprintf(stderr, "ERROR: request returned error code %ld\n",http_resp_code);
        if(cf->payload != NULL ) {
            free(cf->payload);
        }
        return SEV_STEP_ERR;
    }

    //caller does not want/expect json result
    if( json_out == NULL ) {
        if(cf->payload != NULL ) {
            free(cf->payload);
        }
        return SEV_STEP_OK;
    }
    //otherwise, parse body to json 

    /* check payload */
    if (cf->payload != NULL) {
        /* print result */
        //printf("CURL Returned: \n%s\n", cf->payload);
        /* parse return */
        *json_out = json_tokener_parse_verbose(cf->payload, &jerr);
        /* free payload */
        free(cf->payload);
    } else {
        /* error */
        fprintf(stderr, "ERROR: Failed to populate payload");
        /* free payload */
        free(cf->payload);
        /* return */
        return SEV_STEP_ERR;
    }

    /* check error */
    if (jerr != json_tokener_success) {
        /* error */
        fprintf(stderr, "ERROR: Failed to parse json string");
        /* free json object */
        json_object_put(*json_out);
        /* return */
        return SEV_STEP_ERR;
    }

   

   
    return SEV_STEP_OK;
}


int json_uint64_helper(json_object* json_data,char* key, uint64_t* result) {
    json_object* json_gpa;
    if( !json_object_object_get_ex(json_data,key,&json_gpa) ) {
        fprintf(stderr, "ERROR: result did not contain \"%s\" key\n",key);
        return SEV_STEP_ERR;
    }
    errno = 0;
    *result = json_object_get_uint64(json_gpa);
    if( errno != 0 ) {
        fprintf(stderr, "ERROR: failed to parse value for \"%s\" :"
            " failed to parse %s  to uint64_t\n",key,json_object_to_json_string(json_gpa));
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}


int json_bool_helper(json_object* json_data,char* key, bool* result) {
    json_object* json_bool;
    if( !json_object_object_get_ex(json_data,key,&json_bool) ) {
        fprintf(stderr, "ERROR: result did not contain \"%s\" key\n",key);
        return SEV_STEP_ERR;
    }
    errno = 0;
    *result = json_object_get_boolean(json_bool);
    if( errno != 0 ) {
        fprintf(stderr, "ERROR: failed to parse value for \"%s\" :"
            " failed to parse %s  to bool\n",key,json_object_to_json_string(json_bool));
        return SEV_STEP_ERR;
    }
    return SEV_STEP_OK;
}

int json_string_helper(json_object* json_data,char* key, char** result) {
    json_object* json_str;
    if( !json_object_object_get_ex(json_data,key,&json_str) ) {
        fprintf(stderr, "ERROR: result did not contain \"%s\" key\n",key);
        return SEV_STEP_ERR;
    }
    errno = 0;
    const char* tmp = json_object_get_string(json_str);
    if( errno != 0 || tmp == NULL) {
        fprintf(stderr, "ERROR: failed to parse value for \"%s\" :"
            " failed to parse %s  to bool\n",key,json_object_to_json_string(json_str));
        return SEV_STEP_ERR;
    }

    *result = strdup(tmp);
    if(*result == NULL ) {
        fprintf(stderr, "ERROR: failed copy tmp str\n");
        return SEV_STEP_ERR;
    }

    return SEV_STEP_OK;
}

int json_uint64_array_helper(json_object* json_data,char* key, uint64_t** result, uint64_t* result_len) {
    json_object* json_offset_array;
    if( !json_object_object_get_ex(json_data,key,&json_offset_array) ) {
        fprintf(stderr, "ERROR: result did not contain \"%s\" key\n",key);
        return SEV_STEP_ERR;
    }

    size_t offset_array_len = json_object_array_length( json_offset_array);
    *result = malloc(offset_array_len * sizeof(uint64_t));
    *result_len = offset_array_len;

    for( uint64_t i = 0; i < offset_array_len; i++ ) {
        json_object* json_offset_entry = json_object_array_get_idx(json_offset_array, i);
        if (json_offset_entry == NULL ) {
            fprintf(stderr, "ERROR: failed to get json_offset_entry for idx %ju\n",i);
            return SEV_STEP_ERR;
        }
        errno = 0;
        (*result)[i] = json_object_get_uint64(json_offset_entry);
        if( errno != 0 ) {
            fprintf(stderr, "ERROR: failed to parse value for \"%s\" :"
                " failed to parse %s  to uint64_t\n",key,json_object_to_json_string(json_offset_entry));
            *result_len = 0;
            free(result);
            return SEV_STEP_ERR;
        }
    }

    return SEV_STEP_OK;
}