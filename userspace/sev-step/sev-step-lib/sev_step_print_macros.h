#ifndef SEV_STEP_PRINT_MACROS
#define SEV_STEP_PRINT_MACROS

#include <stdio.h>
#include <time.h>

#define flf_printf(fmt, ...) printf("%ju - %s:%d: %s: " fmt, time(NULL), __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#endif 