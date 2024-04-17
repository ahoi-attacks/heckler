#ifndef CACHE_ATTACK_HELPERS_H
#define CACHE_ATTACK_HELPERS_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../sev-step-lib/ansi-color-codes.h"

#include "my-error-codes.h"

void print_cache_timings(char *fp, uint64_t *timings, uint64_t *perf_values, uint64_t timings_len,
                         int way_count, uint64_t table_paddr, uint64_t expected_offset)
{

    printf("%sResults of cache attack on table at hpa 0x%lx. Got %ju probes. Want high at offset offset 0x%lx\n",
           fp, table_paddr, timings_len,expected_offset);

    uint64_t set_timings_len = (timings_len / way_count);
    printf("set_timings_len = %ju\n", set_timings_len);
    uint64_t *set_timings = malloc(sizeof(uint64_t) * set_timings_len);
    memset(set_timings, 0, sizeof(uint64_t) * set_timings_len);

    uint64_t set_l2_misses_len = set_timings_len;
    uint64_t *set_l2_misses = malloc(sizeof(uint64_t) * set_l2_misses_len);
    memset(set_l2_misses, 0, sizeof(uint64_t) * set_l2_misses_len);

    printf("\n%sindividual timings\n", fp);
    for (uint64_t i = 0; i < timings_len; i++)
    {
        set_timings[i / way_count] += timings[i];
        uint64_t offset = i / way_count * 64;
        char *color = reset;
        if (offset == (expected_offset & ~0xf))
        {
            color = BHYEL;
        }
        printf("%s\t%s"
               "Offset 0x%03lx : timing %03lu , expected high? %d perf diff?: %lu\n" reset,
               fp, color, offset, timings[i], offset == (expected_offset & ~0xf), perf_values[i]);

        if ((timings[i] >= 100) && (perf_values[i] == 0))
        {
            set_l2_misses[i / way_count] += 1;
        }
    }
    uint64_t max_value = 0;
    for (uint64_t i = 0; i < set_timings_len; i++)
    {
        if (set_timings[i] > max_value)
        {
            max_value = set_timings[i];
        }
    }
    printf("\n%sset stats\n", fp);
    for (uint64_t i = 0; i < set_timings_len; i++)
    {
        uint64_t offset = i * 64;
        char *color;
        // expected offset and max timing
        if (((offset & ~0xf) == (expected_offset & ~0xf)) && set_timings[i] == max_value)
        {
            color = BHGRN;
        }
        else if (set_timings[i] == max_value)
        { // unexpected offset with max timing
            color = BHRED;
        }
        else
        {
            color = reset;
        }
        printf("%s\t%s set_timings[%02ju] (offset 0x%03jx)= %03ju want high? %d, l2 misses = %ju\n" reset,
               fp, color, i, offset, set_timings[i], (offset & ~0xf) == (expected_offset & ~0xf), set_l2_misses[i]);
    }
    free(set_timings);
    free(set_l2_misses);
}

/**
 * @brief Check that cache set containing expected_offset has the
 * (unique) max timing value
 *
 * @return int HOST_CLIENT_SUCCESS or HOST_CLIENT_ERROR
 */
int can_distinguish_expected_offset(char *fp,
                                    uint64_t *timings, uint64_t *perf_diffs, uint64_t len,
                                    uint64_t way_count, uint64_t expected_offset)
{

    (void)perf_diffs;
    uint64_t res = HOST_CLIENT_SUCCESS;

    // batch individual timings to cache set timings
    uint64_t set_timings_len = (len / way_count);
    uint64_t *set_timings = malloc(sizeof(uint64_t) * set_timings_len);
    memset(set_timings, 0, sizeof(uint64_t) * set_timings_len);

    for (uint64_t i = 0; i < len; i++)
    {
        set_timings[i / way_count] += timings[i];
    }
    uint64_t max_value = set_timings[0];
    bool unique_max_value = true;
    for (uint64_t i = 1; i < set_timings_len; i++)
    {
        if (set_timings[i] > max_value)
        {
            max_value = set_timings[i];
            unique_max_value = true;
        }
        else if (set_timings[i] == max_value)
        {
            unique_max_value = false;
        }
    }

    if (!unique_max_value)
    {
        printf("%s : %s : max value not unique, cannot distinguish"
               "expected_offset from others\n",
               fp, __FUNCTION__);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

    uint64_t set_idx_for_expected_offset = expected_offset / 64;
    if (set_timings[set_idx_for_expected_offset] != max_value)
    {
        printf("%s : %s : expected_offset is 0x%jx -> set_timings[%ju] "
               "should be max value but is not\n",
               fp, __FUNCTION__, expected_offset, set_idx_for_expected_offset);
        res = HOST_CLIENT_ERROR;
        goto cleanup;
    }

cleanup:
    free(set_timings);
    return res;
}

#endif