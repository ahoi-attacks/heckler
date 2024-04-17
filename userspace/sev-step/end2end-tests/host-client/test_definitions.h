/**
 * @file test_definitions.h
 * @author Luca Wilke (l.wilke@its.uni-luebeck.de)Luca Wilke (l.wilke@its.uni-luebeck.de)
 * @brief Common code used by multiple tests
 * @version 0.1
 * @date 2023-01-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef TEST_DEFINITIONS_H
#define TEST_DEFINITIONS_H

#include <stdio.h>

typedef struct {
    char* name;
    int (*test_function)(char* format_prefix, void* args);
    void* args;
    bool skip;
} end2end_test_t;

typedef struct {
    int want_runs;
    int timer_value; 
    int total_zero_step_count;
    int total_single_step_count;
    int total_multi_step_count;
    int total_step_count;
} print_step_histogram_args_t;
void print_step_histogram(char* format_prefix,print_step_histogram_args_t args ) {
    double zero_step_fraction = (double)args.total_zero_step_count/args.total_step_count;
    double multi_step_fraction = (double)args.total_multi_step_count/args.total_step_count;

    printf("%sStep Histogram (%d events over %d runs) \n",format_prefix,args.total_step_count,args.want_runs);
    printf("%s\t Timer value    : 0x%x\n",format_prefix,args.timer_value);
    printf("%s\t Zero   Steps   : %02.2f%% (%06d events)\n",format_prefix, zero_step_fraction*100,args.total_zero_step_count);
    printf("%s\t Single Steps   : %02.2f%% (%06d events)\n",
        format_prefix,(double)args.total_single_step_count/args.total_step_count*100,args.total_single_step_count);
    printf("%s\t Multi  Steps   : %02.2f%% (%06d events)\n",
        format_prefix,multi_step_fraction*100,args.total_multi_step_count);
}


#endif