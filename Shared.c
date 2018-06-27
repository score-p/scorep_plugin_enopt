#include "Shared.h"
#include <enoptC.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>




const char* ENV_VAR_DEBUG_OUTPUT = "ENOPT_ENABLE_DEBUG_OUTPUT";


void debug_printf(const char* format, ...) {
    if( getenv(ENV_VAR_DEBUG_OUTPUT) == NULL )
        return;

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
}
