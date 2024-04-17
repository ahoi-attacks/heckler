/**
 * @file sev_step_routines.h
 * @author Luca Wilke (l.wilke@its.uni-luebeck.de)
 * @brief This file contains functions that build on the basic sev_step api to achieve
 * more complex tasks.
 * @date 2023-01-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SEV_STEP_ROUTINES_H
#define SEV_STEP_ROUTINES_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 
 * 
 * @param asm_exc_nmi_offset offset of asm_exc_nmi relative to the start of the kernels text section 
 * @param kernel_text_base_gpa Result param. Filled with the de-randomized gpa of the kernel's text section
 * @param debug_mode If true, we expect vm to run in debug, allowing us to calculate kernel_text_base_vaddr
 * @param kernel_text_base_vaddr Result param. If debug_mode=false this param should be set to NULL. If
 * debug_mode=true, this param is filled with the de-randomized vaddr of the kernel's text section
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int break_kaslr_via_nmi(uint64_t asm_exc_nmi_offset ,uint64_t* kernel_text_base_gpa, bool debug_mode,
    uint64_t* kernel_text_base_vaddr);

#endif