
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include "../sev-step-lib/ansi-color-codes.h"
#include "../sev-step-lib/sev_step_api.h"
#include "../sev-step-lib/sev_step_print_macros.h"
#include "../end2end-tests/host-client/my-error-codes.h"
#include "../end2end-tests/host-client/vm-server-client.h"

#include "../end2end-tests/host-client/test_definitions.h"
#include "../end2end-tests/host-client/single_stepping_tests.h"



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


/**
 * @brief Convert instruction slide number to instruction slide enum
 *
 * @param int instruction slide number
 * @return int HOST_CLIENT_ERROR or HOST_CLIENT_SUCCESS
 */
int strtinstr(int instr, victim_program_t* SLIDE)
{
    switch (instr) {
	    case 0:
		*SLIDE = VICTIM_PROGRAM_NOP_SLIDE;
		return HOST_CLIENT_SUCCESS;
	    case 1:
		*SLIDE = VICTIM_PROGRAM_ADD_SLIDE;
		return HOST_CLIENT_SUCCESS;
	    case 2:
		*SLIDE = VICTIM_PROGRAM_RDRAND64_SLIDE;
		return HOST_CLIENT_SUCCESS;
	    case 3:
		*SLIDE = VICTIM_PROGRAM_RDRAND32_SLIDE;
		return HOST_CLIENT_SUCCESS;
	    case 4:
		*SLIDE = VICTIM_PROGRAM_DIVR16_SLIDE;
		return HOST_CLIENT_SUCCESS;
	    case 5:
		*SLIDE = VICTIM_PROGRAM_DIVR32_SLIDE;
		return HOST_CLIENT_SUCCESS;
	    case 6:
		*SLIDE = VICTIM_PROGRAM_DIVR64_SLIDE0;
		return HOST_CLIENT_SUCCESS;
	    case 7:
		*SLIDE = VICTIM_PROGRAM_DIVR64_SLIDE1;
		return HOST_CLIENT_SUCCESS;
	    case 8:
		*SLIDE = VICTIM_PROGRAM_DIVR64_SLIDE2;
		return HOST_CLIENT_SUCCESS;
	    case 9:
		*SLIDE = VICTIM_PROGRAM_DIVR64_SLIDE3;
		return HOST_CLIENT_SUCCESS;
            case 10:
		*SLIDE = VICTIM_PROGRAM_DIVR64_SLIDE4;
		return HOST_CLIENT_SUCCESS;
	    case 11:
		*SLIDE = VICTIM_PROGRAM_MULR64_SLIDE;
		return HOST_CLIENT_SUCCESS;
	    case 12:
		*SLIDE = VICTIM_PROGRAM_RCL_SLIDE;
		return HOST_CLIENT_SUCCESS;
	    case 13:
		*SLIDE = VICTIM_PROGRAM_LAR_SLIDE;
		return HOST_CLIENT_SUCCESS;
	    default:
		flf_printf("ERROR: invalid program value\n");
		return HOST_CLIENT_ERROR;
    }
}

int main(int argc, char** argv) {

    uint64_t timer_value = 0;
    victim_program_t VICTIM_PROGRAM_SLIDE = 0; // NOP is default
    if (argc != 3) {
        flf_printf("Usage: nemesis-main <timer value> <victim slide number>");
        return 1;
    }
    if( HOST_CLIENT_SUCCESS != do_stroul(argv[1],0,&timer_value)) {
        flf_printf("failed to parse timer_value\n");
        return HOST_CLIENT_ERROR;
    }

    int instr = atoi(argv[2]);

	if( HOST_CLIENT_SUCCESS != strtinstr(instr, &VICTIM_PROGRAM_SLIDE)) {
	    flf_printf("failed to parse instruction slide instruction\n");
	    return HOST_CLIENT_ERROR;
	}
    flf_printf("Parsed timer value 0x%jx\n",timer_value);
	flf_printf("Using instruction slide: %d\n", VICTIM_PROGRAM_SLIDE); 



    //
    // nemesis_slide
    //
    test_single_step_nop_slide_args_t nemesis_slide_args = {
        .timer_value = timer_value,
        .check_debug_rip = false,
        .zero_step_abort_fraction = 0.8,
        .multi_step_abort_fraction = 0.2,
        .save_logs_to_file = true,
        .filename = "out.json",
        .instruction_slide = VICTIM_PROGRAM_SLIDE
    };
    end2end_test_t nemesis_slide = {
        .name = "nemesis_slide",
        .test_function = test_single_step_nop_slide,
        .args = (void*)&nemesis_slide_args,
	    .skip = false
    };

    flf_printf("Running nemesis slide...\n");
    if (HOST_CLIENT_ERROR == nemesis_slide.test_function(nemesis_slide.name, nemesis_slide.args) ) {
            printf("\nnemesis slides" BRED "FAILED\n" reset);
            return 1;
    }
    flf_printf("SUCCESS\n");

    return 0;
}
