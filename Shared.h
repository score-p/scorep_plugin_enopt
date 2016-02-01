#ifndef ENOPT_STRICT_SYNC_SHARED_H_
#define ENOPT_STRICT_SYNC_SHARED_H_


/** Defines the name of the environment variable to set for debug output. */
extern const char* ENV_VAR_DEBUG_OUTPUT;

/** Prints debug output, if enabled. */
void debug_printf(const char* format, ...);

/** Initializes Enopt, and makes sure that this happens only once. */
int init_enopt_wrapper();

/** Finalizes Enopt, when called as often as initialize. */
int finalize_enopt_wrapper();


#endif
