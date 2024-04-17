
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include "../../sev-step-lib/ansi-color-codes.h"
#include "../../sev-step-lib/sev_step_api.h"
#include "my-error-codes.h"
#include "vm-server-client.h"

#include "test_definitions.h"
#include "pagetracking_tests.h"
#include "single_stepping_tests.h"
#include "cache_attack_tests.h"






/**
 * @brief This will setup an API connection followed by immediate teardown
 * 
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int test_setup_teardown_ctx(char* format_prefix,void* _args) {
    (void)_args;

    for( int run_idx = 0; run_idx < 10; run_idx++ ) {
        usp_poll_api_ctx_t ctx;

        printf("%siteration %d\n",format_prefix,run_idx);
        printf("%sInitializing API...\n",format_prefix);
        if(SEV_STEP_ERR == usp_new_ctx(&ctx,false)) {
            printf("%s" BRED "usp_new_ctx failed." reset " Check dmesg for more information\n",format_prefix);
            return HOST_CLIENT_ERROR;
        }
        printf("%sAPI initialization done!\n",format_prefix);
        printf("%sClosing API...\n",format_prefix);
        if(SEV_STEP_ERR == usp_close_ctx(&ctx)) {
            printf("%s" BRED "usp_close_ctx failed." reset " Check dmesg for more information\n",format_prefix);
            return HOST_CLIENT_ERROR;
        }
        printf("%sAPI closed!\n",format_prefix);
    }
    return HOST_CLIENT_SUCCESS;
}

/**
 * @brief Wrapper with error handling around strtoul
 *
 * @param str string to parse as number
 * @param base as described in strtoul doc
 * @param result result param
 * @return int SEV_STEP_OK or SEV_STEP_ERR
 */
int do_stroul(char *str, int base, uint64_t *result)
{
    (*result) = strtoul(str, NULL, base);
    // if commented in, we cannot enter zero, as uses zero as an error case. it's just stupid
    /*if ((*result) == 0) {
      printf("line %d: failed to convert %s to uint64_t\n", __LINE__, str);
      return 0;
    }*/
    if ((*result) == ULLONG_MAX && errno == ERANGE)
    {
        flf_printf("failed to convert %s to uint64_t. errno was ERANGE\n", str);
        return HOST_CLIENT_ERROR;
    }
    return HOST_CLIENT_SUCCESS;
}

int main(int argc, char** argv) {

    uint64_t timer_value = 0;
    // if true, timer_value is valid
    bool have_timer_value = false;
    if (argc == 2)
    {
        if( HOST_CLIENT_SUCCESS != do_stroul(argv[1],0,&timer_value)) {
            flf_printf("failed to parse timer_value\n");
            return HOST_CLIENT_ERROR;
        }
        have_timer_value = true;
        flf_printf("Parsed timer value 0x%jx\n",timer_value);
    }
    if(! have_timer_value ) {
        flf_printf("No timer value provided, some tests will be skipped")
    }


    //
    // test_single_step_simple_long
    //

    // TODO: this test freezes the VM at the end when we explicitly send a timer interrupt in the kernel
    // after disabling single stepping. However, for the test_single_step_nop_slide_simple test it is exactly the other way around. I guess there are some contexts in which we cannot send the timer interrupt. When single stepping only userspace code we properly never encounter these
    test_single_step_simple_args_t test_single_step_simple_long_args_t = {
        .timer_value = timer_value,
        .want_steps = 5000,
        .zero_step_abort_fraction = 0.8,
        .multi_step_abort_fraction = 0.2,
    };
    end2end_test_t test_single_step_simple_long = {
        .name = "test_single_step_simple_long",
        .test_function = test_single_step_simple,
        .args = (void*)&test_single_step_simple_long_args_t,
        .skip = !have_timer_value
    };

    //
    // test_single_step_simple_long_rep
    //

    //same freezing behaviour as in test_single_step_simple_long
    /*test_single_step_simple_rep_args_t test_single_step_simple_long_rep_args = {
        .timer_value = timer_value,
        .want_runs = 100,
        .want_steps_per_run = 500,
        .zero_step_abort_fraction = 0.8,
        .multi_step_abort_fraction = 0.2,
    };
    end2end_test_t test_single_step_simple_long_rep = {
        .name = "test_single_step_simple_long_rep",
        .test_function = test_single_step_simple_rep,
        .args = (void*)&test_single_step_simple_long_rep_args,
    };*/

    //
    // test_single_step_simple_short_rep
    //

    /*
    test_single_step_simple_rep_args_t test_single_step_simple_short_rep_args = {
        .timer_value = timer_value,
        .want_runs = 200,
        .want_steps_per_run = 60,
        .zero_step_abort_fraction = 0.8,
        .multi_step_abort_fraction = 0.2,
    };
    end2end_test_t test_single_step_simple_short_rep = {
        .name = "test_single_step_simple_short_rep",
        .test_function = test_single_step_simple_rep,
        .args = (void*)&test_single_step_simple_short_rep_args,
    };*/



    test_do_cache_attack_l2_args_t test_do_cache_attack_l2_args = {
        .timer_value = timer_value,
        .zero_step_timer_value = timer_value - 4,
        .check_debug_rip = true,
        .cache_attack_target = VICTIM_PROGRAM_EVAL_CACHE_VICTIM,
    };
    end2end_test_t testdef_do_cache_attack_l2 = {
        .name = "testdef_do_cache_attack_l2",
        .test_function = test_do_cache_attack_l2,
        .args = (void*)&test_do_cache_attack_l2_args,
        .skip = !have_timer_value
    };

    /*test_do_cache_attack_kernel_aliasing_args_t test_do_cache_attack_kernel_aliasing_args = {
        .timer_value = timer_value,
        .check_debug_rip = true,
    };
    end2end_test_t testdef_do_cache_attack_kernel_aliasing = {
        .name = "do_cache_attack_kernel_aliasing",
        .test_function = test_do_cache_attack_kernel_aliasing,
        .args = (void*)&test_do_cache_attack_kernel_aliasing_args,
    };*/

    end2end_test_t tests[] = {
        {.name = "mixed page track bug",
         .test_function = test_track_all_mixed_with_single,
         .args = NULL,
         .skip = false
        },
        {
            .name = "setup->teardown",
            .test_function = test_setup_teardown_ctx,
            .args = NULL,
            .skip = false,
        },
        {
            .name = "pagetrack write",
            .test_function = test_pagetrack_write,
            .args = NULL,
            .skip = false,
        },
        {
            .name = "pagetrack access",
            .test_function = test_pagetrack_access,
            .args = NULL,
            .skip = false,
        },
        {
            .name = "pagetrack exec",
            .test_function = test_pagetrack_exec,
            .args = NULL,
            .skip = false,
        },
        {
            .name = "test_track_all_rep",
            .test_function = test_track_all_rep,
            .args = NULL,
            .skip = false,
        },
        test_single_step_simple_long,
        /*TODO: Still having some freeze issues with  test_single_step_simple_short_rep and test_single_step_simple_long_rep.
         Not hard freezes though (can still kill qemu and restart vm)
        */
        // test_single_step_simple_short_rep,
        // test_single_step_simple_long_rep,
        /*{
            .name = "test_import_free_user_evs",
            .test_function = test_import_free_user_evs,
            .args = NULL,
        },*/
        testdef_do_cache_attack_l2,
    };

    int tests_len = sizeof(tests)/sizeof(end2end_test_t);
    int skipped_tests = 0;
    for( int test_idx = 0; test_idx < tests_len; test_idx++) {
        end2end_test_t test = tests[test_idx];
        if( test.skip ) {
            printf("[%d/%d] - "BHYEL"Skipping test " reset"%s...\n", test_idx + 1, tests_len, test.name);
            skipped_tests += 1;
            continue;
        }
        printf("[%d/%d] - Running test %s...\n",test_idx+1,tests_len,test.name);
        if( HOST_CLIENT_ERROR == test.test_function("\t",test.args)) {
            printf("\nTest %s" BRED "FAILED\n" reset,test.name);
            return 1;
        }
        printf("\n[%d/%d] - Test %s" BHGRN " SUCCEEDED\n" reset,test_idx+1,tests_len,test.name);

    }
    if( skipped_tests == 0 ) {
        printf(BHGRN "\n All Tests SUCCEEDED\n Happy Pwning !\n" reset);
    } else {
        printf(BHGRN "\n[%d/%d] Tests SUCCEEDED,"BHYEL" %d skipped\n" reset, tests_len-skipped_tests,tests_len,skipped_tests);
        printf("To enable tests that require single stepping, provide an APIC timer value as first param. If you want hex encoding, add prefix 0x.");
    }

    return 0;
}