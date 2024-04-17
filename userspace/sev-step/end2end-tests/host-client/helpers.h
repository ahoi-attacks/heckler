#ifndef HELPERS_H
#define HELPERS_H

#include "../../sev-step-lib/ansi-color-codes.h"
#include "../../sev-step-lib/sev_step_api.h"
#include "../../sev-step-lib/sev_step_print_macros.h"
#include "my-error-codes.h"

bool elem_is_in_array(uint64_t elem, uint64_t *array, uint64_t len)
{
    for (uint64_t i = 0; i < len; i++)
    {
        if (array[i] == elem)
        {
            return true;
        }
    }
    return false;
}

int track_pages(usp_poll_api_ctx_t *ctx, uint64_t *gpas, uint64_t len, enum kvm_page_track_mode mode)
{
    for (uint64_t i = 0; i < len; i++)
    {
        if (SEV_STEP_OK != track_page(ctx, gpas[i], mode))
        {
            flf_printf("track_page for gpa 0x%jx failed\n", gpas[i]);
            return SEV_STEP_ERR;
        }
    }
    return SEV_STEP_OK;
}

int untrack_pages(usp_poll_api_ctx_t *ctx, uint64_t *gpas, uint64_t len, enum kvm_page_track_mode mode)
{
    for (uint64_t i = 0; i < len; i++)
    {
        if (SEV_STEP_OK != untrack_page(ctx, gpas[i], mode))
        {
            flf_printf("track_page for gpa 0x%jx failed\n", gpas[i]);
            return SEV_STEP_ERR;
        }
    }
    return SEV_STEP_OK;
}

#endif