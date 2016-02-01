#include "Shared.h"
#include <enoptC.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>




const char* ENV_VAR_DEBUG_OUTPUT = "ENOPT_ENABLE_DEBUG_OUTPUT";
const char* ENV_VAR_INIT_COUNTER = "ENOPT_INIT_COUNT";


void debug_printf(const char* format, ...) {
    if( getenv(ENV_VAR_DEBUG_OUTPUT) == NULL )
        return;

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
}


int read_count() {
    const char* countStr = getenv(ENV_VAR_INIT_COUNTER);
    return countStr  ?  strtol(countStr, NULL, 10) : 0;
}


void write_count(int count) {
    char str[16];
    sprintf(str, "%d", count);
    setenv(ENV_VAR_INIT_COUNTER, str, 1);
}


int init_enopt_wrapper() {
    int count = read_count();
    int result = (count == 0)  ?  enopt_init() : 0;
    write_count(++count);
    debug_printf("NOTE: After increasing, the new init count is: %i\n", count);

    return result;
}


int finalize_enopt_wrapper() {
    int count = read_count();
    if( count <= 0 ) {
        printf("ERROR: Finalize called more often than initialize!\n");
        abort();
    }

    int result = 0;
    if( count == 1 ) {
        debug_printf("NOTE: Init count is going to 0, finalizing Enopt...\n");
        result = enopt_finalize();
    }

    write_count(--count);
    debug_printf("NOTE: After decreasing, the new init count is: %i\n", count);

    return result;
}
