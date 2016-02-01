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

#ifndef ENOPT_H_
#define ENOPT_H_

#include <stdbool.h>
#include <stdlib.h>




typedef struct SCOREP_Enopt_MetricInfo
{
    char*    name;
    int32_t  enopt_id;
    bool     used;
    uint64_t value;
} SCOREP_Enopt_MetricInfo;

typedef struct SCOREP_Enopt_NameId
{
    const char* name;
    int32_t     id;
    char*       metric;
} SCOREP_Enopt_NameId;

SCOREP_Enopt_MetricInfo scorep_enopt_metric[ MAX_COUNTERS ] =
{
    {
        .name = NULL,
        .enopt_id = 0,
        .used = false,
        .value = 0
    }
};

//RM: Could be sorted and then searched with the binary search instead of the linear search.
SCOREP_Enopt_NameId table[] =
{
    {
        "ENOPT_ALL_CORES",
        ENOPT_ALL_CORES,
        "JOULE"
    },
    {
        "ENOPT_ALL_UNCORES",
        ENOPT_ALL_UNCORES,
        "JOULE"
    },
    {
        "ENOPT_ALL_SOCKETS",
        ENOPT_ALL_SOCKETS,
        "JOULE"
    },
    {
        "ENOPT_ALL_DRAMS",
        ENOPT_ALL_DRAMS,
        "JOULE"
    },
    {
        "ENOPT_NODE",
        ENOPT_NODE,
        "JOULE"
    },
    {
        "ENOPT_PDU",
        ENOPT_PDU,
        "JOULE"
    },
    {
        "ENOPT_CORES_1",
        ENOPT_CORES_1,
        "JOULE"
    },
    {
        "ENOPT_CORES_2",
        ENOPT_CORES_2,
        "JOULE"
    },
    {
        "ENOPT_UNCORES_1",
        ENOPT_UNCORES_1,
        "JOULE"
    },
    {
        "ENOPT_UNCORES_2",
        ENOPT_UNCORES_2,
        "JOULE"
    },
    {
        "ENOPT_SOCKET_1",
        ENOPT_SOCKET_1,
        "JOULE"
    },
    {
        "ENOPT_SOCKET_2",
        ENOPT_SOCKET_2,
        "JOULE"
    },
    {
        "ENOPT_DRAM_1",
        ENOPT_DRAM_1,
        "JOULE"
    },
    {
        "ENOPT_DRAM_2",
        ENOPT_DRAM_2,
        "JOULE"
    },
    {
        "ENOPT_DC",
        ENOPT_DC,
        "JOULE"
    },
    {
        "ENOPT_AC",
        ENOPT_AC,
        "JOULE"
    },
    {
        "ENOPT_CYCLES",
        ENOPT_CYCLES,
        NULL
    },
    {
        "ENOPT_INSTRUCTIONS",
        ENOPT_INSTRUCTIONS,
        NULL
    },
    {
        "ENOPT_CACHEL2",
        ENOPT_CACHEL2,
        NULL
    },
    {
        "ENOPT_CACHEL3",
        ENOPT_CACHEL3,
        NULL
    },
    {
        "ENOPT_TIME",
        ENOPT_TIME,
        "SEC"
    },
    {
        "ENOPT_POWER_ALL_CORES",
        ENOPT_POWER_ALL_CORES,
        "WATT"
    },
    {
        "ENOPT_POWER_ALL_UNCORES",
        ENOPT_POWER_ALL_UNCORES,
        "WATT"
    },
    {
        "ENOPT_POWER_ALL_SOCKETS",
        ENOPT_POWER_ALL_SOCKETS,
        "WATT"
    },
    {
        "ENOPT_POWER_ALL_DRAMS",
        ENOPT_POWER_ALL_DRAMS,
        "WATT"
    },
    {
        "ENOPT_POWER_NODE",
        ENOPT_POWER_NODE,
        "WATT"
    },
    {
        "ENOPT_POWER_UNCORES_1",
        ENOPT_POWER_UNCORES_1,
        "WATT"
    },
    {
        "ENOPT_POWER_UNCORES_2",
        ENOPT_POWER_UNCORES_2,
        "WATT"
    },
    {
        "ENOPT_POWER_SOCKET_1",
        ENOPT_POWER_SOCKET_1,
        "WATT"
    },
    {
        "ENOPT_POWER_SOCKET_2",
        ENOPT_POWER_SOCKET_2,
        "WATT"
    },
    {
        "ENOPT_POWER_DRAM_1",
        ENOPT_POWER_DRAM_1,
        "WATT"
    },
    {
        "ENOPT_POWER_DRAM_2",
        ENOPT_POWER_DRAM_2,
        "WATT"
    },
    {
        "ENOPT_POWER_DC",
        ENOPT_POWER_DC,
        "WATT"
    },
    {
        "ENOPT_POWER_AC",
        ENOPT_POWER_AC,
        "WATT"
    },
    {
        NULL,
        0,
	NULL
    }
};

void scorep_enopt_metric_init( void );

void scorep_enopt_add_counter( char* event_name, int32_t id );

void scorep_enopt_get_event_info( void );

int32_t scorep_enopt_get_id( char* event_name );

char* scorep_enopt_get_metric( char* event_name );

#endif /* ENOPT_H_ */
