/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2015,
 *    Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */


#include <scorep/SCOREP_MetricPlugins.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <omp.h>

#include <defines.h>
#include <enoptC.h>

#include "EnoptMP.h"
#include "Shared.h"


/* Maximum number of metrics */
#define NUMBER_RANDOM_METRICS 5

/* Guard for metric ID generation */
static pthread_mutex_t add_metric_mutex;

/* Number of individual metrics */
static int32_t num_metrics = 0;


int32_t init( void )
{
    debug_printf( "EnoptMP: %s: Entering\n", __func__ );

    if( init_enopt_wrapper() != 0 )
    {
        printf( "EnoptMP: %s: enopt_init: Unable to initialize enopt metric source\n", __func__ );
        return -1;
    }
    enopt_start();

    debug_printf( "EnoptMP: %s: Exiting\n", __func__ );
    return 0;
}


int32_t add_counter( char* event_name )
{
    /* ID generation has to be thread save */
    pthread_mutex_lock( &add_metric_mutex );
    int id = num_metrics;
    num_metrics++;
    pthread_mutex_unlock( &add_metric_mutex );

    debug_printf( "EnoptMP: %s(%s) = %d!\n", __func__, event_name, id );
    scorep_enopt_add_counter( event_name, id );

    return id;
}


SCOREP_Metric_Plugin_MetricProperties* get_event_info( char* event_name )
{
    SCOREP_Metric_Plugin_MetricProperties* return_values;
    char                                   name_buffer[ 255 ];
    int                                    i;

    /* If wildcard, add some random counters */
    if( strcmp( event_name, "*" ) == 0 )
    {
        return_values = malloc( ( NUMBER_RANDOM_METRICS + 1 ) * sizeof( SCOREP_Metric_Plugin_MetricProperties ) );
        for( i = 0; i < NUMBER_RANDOM_METRICS; i++ )
        {
            sprintf( name_buffer, "random strictly sync counter #%i", i );
            return_values[ i ].name = strdup( name_buffer );
            return_values[ i ].description = NULL;
            return_values[ i ].unit = NULL;
            return_values[ i ].mode = SCOREP_METRIC_MODE_ABSOLUTE_LAST;
            return_values[ i ].value_type = SCOREP_METRIC_VALUE_UINT64;
            return_values[ i ].base = SCOREP_METRIC_BASE_DECIMAL;
            return_values[ i ].exponent = 0;
        }
        return_values[ NUMBER_RANDOM_METRICS ].name = NULL;
    }
    else
    {
        debug_printf( "EnoptMP: %s(%s)\n", __func__, event_name );
        /* If no wildcard is given create one random counter with the passed name */
        return_values = malloc( 2 * sizeof( SCOREP_Metric_Plugin_MetricProperties ) );
        sprintf( name_buffer, "random strictly sync counter #%s", event_name );
        return_values[ 0 ].name = strdup( event_name /*name_buffer*/ );
        return_values[ 0 ].description = NULL;
        return_values[ 0 ].unit = scorep_enopt_get_metric( event_name );
        return_values[ 0 ].mode = SCOREP_METRIC_MODE_ABSOLUTE_LAST;
        return_values[ 0 ].value_type = SCOREP_METRIC_VALUE_UINT64;
        return_values[ 0 ].base = SCOREP_METRIC_BASE_DECIMAL;
        return_values[ 0 ].exponent = -9; //nJ
        return_values[ 1 ].name = NULL;
    }

    return return_values;
}


uint64_t get_value( int32_t counterIndex )
{
    debug_printf( "EnoptMP: %s: Entering\n", __func__ );

    if( omp_get_thread_num() == 0 )
    {
        switch( enopt_stop() )
        {
            case 0:
                debug_printf( "EnoptMP: %s: enopt_stop: Successfully stopped ENOPT measurements!\n", __func__ );
                break;
            case -1:
                printf( "EnoptMP: %s: enopt_stop: Could not stop ENOPT measurements!\n", __func__ );
                abort();
            case -2:
                printf( "EnoptMP: %s: enopt_stop Not allowed to stop ENOPT measurements!\n", __func__ );
                abort();
            default:
                printf( "EnoptMP: %s: enopt_stop: Returned invalid code!\n", __func__ );
                abort();
        }

        double energy;
        switch( enopt_get( scorep_enopt_metric[ counterIndex ].enopt_id, &energy ) )
        {
            case 0:
                debug_printf( "EnoptMP: %s: enopt_get: Successfully accessed to ENOPT %s measurement!\n",
                              __func__, scorep_enopt_metric[ counterIndex ].name );
                break;
            case -1:
                printf( "EnoptMP: %s: enopt_get: Could not get ENOPT %s measurement!\n",
                         __func__, scorep_enopt_metric[ counterIndex ].name );
                abort();
            case -2:
                printf( "EnoptMP: %s: enopt_get: Not allowed to access ENOPT %s measurement!\n",
                        __func__, scorep_enopt_metric[ counterIndex ].name );
                abort();
            default:
                printf( "EnoptMP: %s: enopt_get: Returned invalid code for ENOPT %s measurement!\n",
                        __func__, scorep_enopt_metric[ counterIndex ].name );
                abort();
        }

        energy *= 1e09;
        scorep_enopt_metric[ counterIndex ].value += ( uint64_t ) energy; 
        debug_printf( "EnoptMP: %s(%d) accumulated with enopt_id(%d) = %llu, read %.0f (0x%x)!\n",
                      __func__, counterIndex, scorep_enopt_metric[ counterIndex ].enopt_id,
                      scorep_enopt_metric[ counterIndex ].value, energy, energy );

        enopt_start();
        debug_printf( "EnoptMP: %s: Exiting\n", __func__ );
        return scorep_enopt_metric[ counterIndex ].value;
    }

    debug_printf( "EnoptMP: %s: Exiting\n", __func__ );
    return 0;
}


void fini( void )
{
    debug_printf( "EnoptMP: %s: Entering\n", __func__ );
    enopt_stop();

    if( finalize_enopt_wrapper() != 0 )
    {
       printf( "EnoptMP: %s: Unable to finalize enopt metric source\n", __func__ );
       abort();
    }

    pthread_mutex_destroy( &add_metric_mutex );
    debug_printf( "EnoptMP: %s: Exiting\n", __func__ );
}


SCOREP_METRIC_PLUGIN_ENTRY( EnoptMP )
{
    /* Initialize info data (with zero) */
    SCOREP_Metric_Plugin_Info info;
    memset( &info, 0, sizeof( SCOREP_Metric_Plugin_Info ) );

    /* Set up */
    info.plugin_version    = SCOREP_METRIC_PLUGIN_VERSION;
    info.run_per           = SCOREP_METRIC_PER_THREAD;
    info.sync              = SCOREP_METRIC_STRICTLY_SYNC;
    info.initialize        = init;
    info.finalize          = fini;
    info.get_event_info    = get_event_info;
    info.add_counter       = add_counter;
    info.get_current_value = get_value;
    return info;
}


void scorep_enopt_metric_init( void )
{
}


void scorep_enopt_add_counter( char* event_name, int32_t id )
{
    scorep_enopt_metric[id].name     = strdup( event_name );
    scorep_enopt_metric[id].used     = true;
    scorep_enopt_metric[id].enopt_id = scorep_enopt_get_id( event_name );
}


void scorep_enopt_get_event_info( void )
{
}


int32_t scorep_enopt_get_id( char * event_name )
{
    SCOREP_Enopt_NameId *p;

    debug_printf( "EnoptMP: %s: Searching for %s starting with %s\n",
                  __func__, event_name, table->name );
    for( p = table; p->name != NULL; ++p )
    {
        debug_printf( "EnoptMP: %s: Comparing %s with %s\n",
                      __func__, p->name, event_name);
        if( strcmp(p->name, event_name ) == 0 )
        {
            debug_printf( "EnoptMP: %s: Found %s with id %d\n",
                    __func__, event_name, p->id);
            return p->id;
        }
    }

    abort();
}


char* scorep_enopt_get_metric( char* event_name )
{
    SCOREP_Enopt_NameId *p;

    for( p = table; p->name != NULL; ++p )
        if( strcmp(p->name, event_name) == 0 )
            return p->metric;

    abort();
}
