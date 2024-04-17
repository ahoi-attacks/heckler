
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include "../../sev-step-lib/ansi-color-codes.h"
#include "../../sev-step-lib/sev_step_api.h"
#include "../../end2end-tests/host-client/my-error-codes.h"
#include "../../end2end-tests/host-client/vm-server-client.h"

#include "../../end2end-tests/host-client/test_definitions.h"
#include "../../end2end-tests/host-client/cache_attack_helpers.h"
#include "paper_experiments.h"

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

int main(int argc, char **argv)
{
    // check cli args
    char *selected_experiment_name;
    if (argc < 2)
    {
        flf_printf("Usage: paper-experiments <experiment name> [apic timer value]");
        return 1;
    }
    selected_experiment_name = argv[1];
    uint64_t timer_value = 0;
    // if true, timer_value is valid
    bool have_timer_value = false;
    if (argc == 3)
    {
        if (HOST_CLIENT_SUCCESS != do_stroul(argv[2], 0, &timer_value))
        {
            flf_printf("failed to parse timer_value\n");
            return HOST_CLIENT_ERROR;
        }
        have_timer_value = true;
        flf_printf("Parsed timer value 0x%jx\n", timer_value);
    }

    //
    // define all know experiments
    //

    //single step evaluation
    paper_experiments_nop_slide_args_t single_step_no_tlb_no_a_bit_args = {
        .timer_value = timer_value,
        .check_debug_rip = true,
        .flush_tlb = false,
        .reset_access_bit = false,
    };
    end2end_test_t single_step_no_tlb_no_a_bit;
    single_step_no_tlb_no_a_bit.args = (void *)&single_step_no_tlb_no_a_bit_args;
    single_step_no_tlb_no_a_bit.name = "single_step_no_tlb_no_a_bit",
    single_step_no_tlb_no_a_bit.test_function = paper_experiments_nop_slide;

    paper_experiments_nop_slide_args_t single_step_no_tlb_reset_a_bit_args = {
        .timer_value = timer_value,
        .check_debug_rip = true,
        .flush_tlb = false,
        .reset_access_bit = true,
    };
    end2end_test_t single_step_no_tlb_reset_a_bit;
    single_step_no_tlb_reset_a_bit.args = (void *)&single_step_no_tlb_reset_a_bit_args;
    single_step_no_tlb_reset_a_bit.name = "single_step_no_tlb_reset_a_bit",
    single_step_no_tlb_reset_a_bit.test_function = paper_experiments_nop_slide;

    paper_experiments_nop_slide_args_t single_step_flush_tlb_no_a_bit_args = {
        .timer_value = timer_value,
        .check_debug_rip = true,
        .flush_tlb = true,
        .reset_access_bit = false,
    };
    end2end_test_t single_step_flush_tlb_no_a_bit;
    single_step_flush_tlb_no_a_bit.args = (void *)&single_step_flush_tlb_no_a_bit_args;
    single_step_flush_tlb_no_a_bit.name = "single_step_flush_tlb_no_a_bit",
    single_step_flush_tlb_no_a_bit.test_function = paper_experiments_nop_slide;

    paper_experiments_nop_slide_args_t single_step_flush_tlb_reset_a_bit_args = {
        .timer_value = timer_value,
        .check_debug_rip = true,
        .flush_tlb = true,
        .reset_access_bit = true,
    };
    end2end_test_t single_step_flush_tlb_reset_a_bit;
    single_step_flush_tlb_reset_a_bit.args = (void *)&single_step_flush_tlb_reset_a_bit_args;
    single_step_flush_tlb_reset_a_bit.name = "single_step_flush_tlb_reset_a_bit",
    single_step_flush_tlb_reset_a_bit.test_function = paper_experiments_nop_slide;

    //cache attack evaluation
    paper_experiment_prime_probe_args_t prime_probe_lfence_args = {
        .timer_value = timer_value,
        .zero_step_timer_value = -1,
        .forced_zero_steps_per_mem_access = 0,
        .check_debug_rip = true,
        .cache_attack_target = VICTIM_PROGRAM_EVAL_CACHE_VICTIM_LFENCE,
    };
    end2end_test_t prime_probe_lfence = {
        .args = (void *)&prime_probe_lfence_args,
        .name = "prime_probe_lfence",
        .test_function = paper_experiment_prime_probe,
    };

    paper_experiment_prime_probe_args_t prime_probe_no_lfence_args = {
        .timer_value = timer_value,
        .zero_step_timer_value = -1,
        .forced_zero_steps_per_mem_access = 0,
        .check_debug_rip = true,
        .cache_attack_target = VICTIM_PROGRAM_EVAL_CACHE_VICTIM,
    };
    end2end_test_t prime_probe_no_lfence = {
        .args = (void *)&prime_probe_no_lfence_args,
        .name = "prime_probe_no_lfence",
        .test_function = paper_experiment_prime_probe,
    };

    // match string arg to experiment
    end2end_test_t selected_experiment;
    if (strcmp(selected_experiment_name, single_step_no_tlb_no_a_bit.name) == 0)
    {
        if (!have_timer_value)
        {
            flf_printf("Provide timer value as additional cli args\n");
            return 1;
        }
        selected_experiment = single_step_no_tlb_no_a_bit;
    }
    else if (strcmp(selected_experiment_name, single_step_no_tlb_reset_a_bit.name) == 0)
    {
        if (!have_timer_value)
        {
            flf_printf("Provide timer value as additional cli args\n");
            return 1;
        }
        selected_experiment = single_step_no_tlb_reset_a_bit;
    }
    else if (strcmp(selected_experiment_name, single_step_flush_tlb_no_a_bit.name) == 0)
    {
        if (!have_timer_value)
        {
            flf_printf("Provide timer value as additional cli args\n");
            return 1;
        }
        selected_experiment = single_step_flush_tlb_no_a_bit;
    }
    else if (strcmp(selected_experiment_name, single_step_flush_tlb_reset_a_bit.name) == 0)
    {
        if (!have_timer_value)
        {
            flf_printf("Provide timer value as additional cli args\n");
            return 1;
        }
        selected_experiment = single_step_flush_tlb_reset_a_bit;
    }
    else if (strcmp(selected_experiment_name, prime_probe_lfence.name) == 0)
    {
        if (!have_timer_value)
        {
            flf_printf("Provide timer value as additional cli args\n");
            return 1;
        }
        selected_experiment = prime_probe_lfence;
    }
    else if (strcmp(selected_experiment_name, prime_probe_no_lfence.name) == 0)
    {
        if (!have_timer_value)
        {
            flf_printf("Provide timer value as additional cli args\n");
            return 1;
        }
        selected_experiment = prime_probe_no_lfence;
    }
    else
    {
        flf_printf("Unknown experiment %s\n", selected_experiment_name);
        return 1;
    }

    // Run selected experiment
    printf("Running experiment %s...\n", selected_experiment.name);
    if (HOST_CLIENT_ERROR == selected_experiment.test_function("\t", selected_experiment.args))
    {
        printf("\n%s" BRED "FAILED\n" reset, selected_experiment.name);
        return 1;
    }
    printf("%s" BHGRN " SUCCEEDED\n" reset, selected_experiment.name);

    return 0;
}