/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2015,
 *    Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 *
 */

/**
 * @file       EnoptTP.c
 *
 * @brief Metric plugin implementation.
 */

#include "Shared.h"
#include <enoptC.h>
#include <scorep/SCOREP_TuningPlugins.h>
#include <scorep/SCOREP_TuningTypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




static void scorep_enopt_set_frequency( int new_setting, int* old_setting )
{
    debug_printf( "EnoptTP: %s: Changing frequency from %d to %d\n", __func__, *old_setting, new_setting );
    static int last_frequency = 7;

    *old_setting   = last_frequency;
    int error      = CPUFreq_setFrequency( new_setting );
    last_frequency = new_setting;
}


static void scorep_enopt_set_governor( int new_setting,int* old_setting )
{
    debug_printf( "EnoptTP: %s: Changing governor from %d to %d\n", __func__, *old_setting, new_setting );
    static int last_governor = 2;

    *old_setting  = last_governor;
    int error     = CPUFreq_setGovernor( new_setting );
    last_governor = new_setting;
}


static SCOREP_Tuning_Action_Info return_values[] =
{
    {
        .name                           = "FREQ",
        .kind                           = SCOREP_TUNING_FUNCTION,
        .enterRegionVariablePtr         = NULL,
        .exitRegionVariablePtr          = NULL,
        .enterRegionFunctionPtr         = &scorep_enopt_set_frequency,
        .exitRegionFunctionPtr          = &scorep_enopt_set_frequency,
        .validationEndRegionFunctionPtr = NULL,
        .restoreValueFlag               = 1,
        .languageType                   = 0
    },
    {
        .name                           = "GOV",
        .kind                           = SCOREP_TUNING_FUNCTION,
        .enterRegionVariablePtr         = NULL,
        .exitRegionVariablePtr          = NULL,
        .enterRegionFunctionPtr         = &scorep_enopt_set_governor,
        .exitRegionFunctionPtr          = &scorep_enopt_set_governor,
        .validationEndRegionFunctionPtr = NULL,
        .restoreValueFlag               = 1,
        .languageType                   = 0
    },
    {
        .name                           = "FREQCORE",
        .kind                           = SCOREP_TUNING_FUNCTION,
        .enterRegionVariablePtr         = NULL,
        .exitRegionVariablePtr          = NULL,
        .enterRegionFunctionPtr         = &CPUFreq_setFreqCore,
        .exitRegionFunctionPtr          = &CPUFreq_setFreqCore,
        .validationEndRegionFunctionPtr = NULL,
        .restoreValueFlag               = 1,
        .languageType                   = 0
    },
    {
        .name                           = "GOVCORE",
        .kind                           = SCOREP_TUNING_FUNCTION,
        .enterRegionVariablePtr         = NULL,
        .exitRegionVariablePtr          = NULL,
        .enterRegionFunctionPtr         = &CPUFreq_setGovCore,
        .exitRegionFunctionPtr          = &CPUFreq_setGovCore,
        .validationEndRegionFunctionPtr = NULL,
        .restoreValueFlag               = 1,
        .languageType                   = 0
    },
    {
        .name                           = "FREQNODE",
        .kind                           = SCOREP_TUNING_FUNCTION,
        .enterRegionVariablePtr         = NULL,
        .exitRegionVariablePtr          = NULL,
        .enterRegionFunctionPtr         = &CPUFreq_setFreqNode,
        .exitRegionFunctionPtr          = &CPUFreq_setFreqNode,
        .validationEndRegionFunctionPtr = NULL,
        .restoreValueFlag               = 1,
        .languageType                   = 0
    },
    {
        .name                           = "GOVNODE",
        .kind                           = SCOREP_TUNING_FUNCTION,
        .enterRegionVariablePtr         = NULL,
        .exitRegionVariablePtr          = NULL,
        .enterRegionFunctionPtr         = &CPUFreq_setGovCore,
        .exitRegionFunctionPtr          = &CPUFreq_setGovCore,
        .validationEndRegionFunctionPtr = NULL,
        .restoreValueFlag               = 1,
        .languageType                   = 0
    },
    {
        .name                           = NULL,
        .kind                           = SCOREP_TUNING_UNDEFINED,
        .enterRegionVariablePtr         = NULL,
        .exitRegionVariablePtr          = NULL,
        .enterRegionFunctionPtr         = NULL,
        .exitRegionFunctionPtr          = NULL,
        .validationEndRegionFunctionPtr = NULL,
        .restoreValueFlag               = 0,
        .languageType                   = 0
    }
};


int32_t init( void )
{
    debug_printf( "EnoptTP: %s: Entering\n", __func__ );

    if( init_enopt_wrapper() != 0 )
    {
        printf( "EnoptTP: %s: Unable to initialize enopt metric source\n", __func__ );
        return -1;
    }

    debug_printf( "EnoptTP: %s: Exiting!\n", __func__ );
    return 0;
}


SCOREP_Tuning_Action_Info* get_tuning_info( void )
{
    return return_values;
}


void fini( void )
{
    debug_printf( "EnoptTP: %s: Entering\n", __func__ );

    if( finalize_enopt_wrapper() != 0 )
    {
        printf( "EnoptTP: %s: Unable to finalize enopt metric source\n", __func__ );
        abort();
    }

    debug_printf( "EnoptTP: %s: Exiting!\n", __func__ );
}


SCOREP_TUNING_PLUGIN_ENTRY( EnoptTP )
{
    /* Initialize info data (with zero) */
    SCOREP_Tuning_Plugin_Info info;
    memset( &info, 0, sizeof( SCOREP_Tuning_Plugin_Info ) );

    /* Set up */
    info.plugin_version  = SCOREP_TUNING_PLUGIN_VERSION;
    info.initialize      = init;
    info.get_tuning_info = get_tuning_info;
    info.finalize        = fini;
    return info;
}
