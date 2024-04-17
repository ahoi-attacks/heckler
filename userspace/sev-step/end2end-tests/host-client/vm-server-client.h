#ifndef VM_SERVER_CLIENT_H
#define VM_SERVER_CLIENT_H

#include <stdint.h>

typedef struct {
    uint64_t gpa1;
    uint64_t gpa2;
} pagetrack_gpas_t;


typedef struct {
    uint64_t* offsets_with_mem_access;
    uint64_t* mem_access_target_offset;
    /// @brief  Length of both mem_access_target_offsets and offsets_with_mem_access
    uint64_t len;
    uint64_t lookup_table_gpa;
    uint64_t lookup_table_vaddr;
    uint64_t lookup_table_bytes;
} cache_attack_data_t;

typedef struct  {
    uint64_t gpa;
    uint64_t vaddr;
    uint64_t* expected_offsets;
    uint64_t expected_offsets_len;
    /// @brief May be null
    cache_attack_data_t* cache_attack_data;
} victim_program_data_t;

typedef enum  {
    VICTIM_PROGRAM_NOP_SLIDE,
    VICTIM_PROGRAM_ADD_SLIDE,
    VICTIM_PROGRAM_RCL_SLIDE,
    VICTIM_PROGRAM_LAR_SLIDE,
    VICTIM_PROGRAM_RDRAND64_SLIDE,
    VICTIM_PROGRAM_RDRAND32_SLIDE,
    VICTIM_PROGRAM_DIVR16_SLIDE,
    VICTIM_PROGRAM_DIVR32_SLIDE,
    VICTIM_PROGRAM_DIVR64_SLIDE0,
    VICTIM_PROGRAM_DIVR64_SLIDE1,
    VICTIM_PROGRAM_DIVR64_SLIDE2,
    VICTIM_PROGRAM_DIVR64_SLIDE3,
    VICTIM_PROGRAM_DIVR64_SLIDE4,
    VICTIM_PROGRAM_MULR64_SLIDE,

    VICTIM_PROGRAM_SIMPLE_CACHE_VICTIM,
    VICTIM_PROGRAM_SIMPLE_CACHE_VICTIM_LFENCE,
    VICTIM_PROGRAM_EVAL_CACHE_VICTIM,
    VICTIM_PROGRAM_EVAL_CACHE_VICTIM_LFENCE,
} victim_program_t;

/**
 * @brief Translate program to string that can be send to the vm-server
 * 
 * @param program 
 * @param res Result Param. String for program that can be used with the vm-server 
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int victim_program_to_str(victim_program_t program,char** res);

int vm_server_pagetrack_init(char* tracking_type, pagetrack_gpas_t* result);
int vm_server_pagetrack_start();
int vm_server_pagetrack_teardown();

int vm_server_single_step_victim_init(victim_program_t program,victim_program_data_t* response);
int vm_server_single_step_victim_start(victim_program_t program);

void free_victim_program_data_t(victim_program_data_t* v);
#endif
