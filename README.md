# Enopt metric and tuning plugins

> *Please note:*
>
> These plugins are used to modify energy governors and frequencies of the CPU,
> and to provide socket, DRAM and node level counter measurements on SuperMUC.
>
> The plugins rely on the presence of the Enopt library, which is only available on
> the SuperMUC petascale system.
> It is not possible to compile and run these plugins on other systems than SuperMUC.

## Compilation and Installation

### Prerequisites

To compile this plugin, you need:

* C compiler

* Score-P (`1.4+`) for the metric plugin, Score-P with the tuning substrate for the tuning plugin.

* `libenopt` (including developement files)

### Building

1. Create a build directory

        mkdir build
        cd build

2. Invoke CMake

    Specify the path to the ENOPT headers with `-DENOPT_INC` if they're not in the default path.

    Afterwards, just call CMake, e.g.:

        cmake -DENOPT_INC=$ENOPT_BASE/include -DENOPT_STATIC=OFF ..

3. Invoke make

        make

4. Install the resulting plugins (`libEnoptMP.so` and `libEnoptTP.so`)

        make DESTDIR=$SCOREP_PATH install

5. List the directory where plugins are installed in `LD_LIBRARY_PATH`, e.g.,

        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SCOREP_PATH/lib/plugins

## Usage

To use the Enopt libray the user should be part of the vtune group and jobs should be submitted in the test queue.
To become part of the vtune group please contact LRZ.

`SCOREP_METRIC_ENOPT_PLUGIN` specifies the software events that shall be recorded when profiling or tracing an
application.

The events available on the system are:

1. ENOPT_ALL_CORES - Provides energy consumption of all cores on a node.
2. ENOPT_ALL_UNCORES - Provides energy consumption of all uncores on a node.
3. ENOPT_ALL_SOCKETS - Provides energy consumption of all sockets (cores + uncores) on a node.
4. ENOPT_ALL_DRAMS - Provides energy consumption of DRAM modules on a node.
5. ENOPT_NODE - Provides energy consumption for complete node.
6. ENOPT_PDU - Provides energy consumption for PDU (multiple nodes).
7. ENOPT_CORES_1 - Provides energy consumption of all cores from socket 1.
8. ENOPT_CORES_2 - Provides energy consumption of all cores from socket 2.
9. ENOPT_UNCORES_1 - Provides energy consumption of all uncores from socket 1.
10. ENOPT_UNCORES_2 - Provides energy consumption of all uncores from socket 2.
11. ENOPT_SOCKET_1 - Provides energy consumption of cores + uncores from socket 1.
12. ENOPT_SOCKET_2 - Provides energy consumption of cores + uncores from socket 2.
13. ENOPT_DRAM_1 - Provides energy consumption of DRAM modules from socket 1.
14. ENOPT_DRAM_2 - Provides energy consumption of DRAM modules from socket 2.
15. ENOPT_DC - Provides energy consumption measured on DC counter.
16. ENOPT_AC - Provides energy consumption measured on AC counter.
17. ENOPT_CYCLES - Provides number of cycles (PAPI).
18. ENOPT_INSTRUCTIONS - Provides number of instructions (PAPI).
19. ENOPT_CACHEL2 - Provides number of L2 cache misses (PAPI).
20. ENOPT_CACHEL3 - Provides number of L3 cache misses (PAPI).
21. ENOPT_TIME - Provides number of L2 cache misses (PAPI).
22. ENOPT_POWER_ALL_CORES - Provides power of all cores on a node.
23. ENOPT_POWER_ALL_UNCORES - Provides power of all uncores on a node.
24. ENOPT_POWER_ALL_SOCKETS - Provides power of all sockets (cores + uncores) on a node.
25. ENOPT_POWER_ALL_DRAMS - Provides power of all DRAM modules on a node.
26. ENOPT_POWER_NODE - Provides power for complete node.
27. ENOPT_POWER_PDU - Provides power for PDU (multiple nodes).
28. ENOPT_POWER_CORES_1 - Provides power of all cores from socket 1.
29. ENOPT_POWER_CORES_2 - Provides power of all cores from socket 2.
30. ENOPT_POWER_UNCORES_1 - Provides power of all uncores from socket 1.
31. ENOPT_POWER_UNCORES_2 - Provides power of all uncores from socket 2.
32. ENOPT_POWER_SOCKET_1 - Provides power of cores + uncores from socket 1.
33. ENOPT_POWER_SOCKET_2 - Provides power of cores + uncores from socket 2.
34. ENOPT_POWER_DRAM_1 - Provides power of DRAM modules from socket 1.
35. ENOPT_POWER_DRAM_2 - Provides power of DRAM modules from socket 2.
36. ENOPT_POWER_DC - Provides power measured on DC counter.
37. ENOPT_POWER_AC - Provides power measured on AC counter.

To add a kernel event counter to your profile or trace, you have to specify the environment variables
`SCOREP_METRIC_PLUGINS` and `SCOREP_METRIC_ENOPTMP` or use the Periscope Tuning Framework.

Load the metric plugin library:

    SCOREP_METRIC_PLUGINS="EnoptMP"

E.g.

    export SCOREP_METRIC_ENOPTMP="ENOPT_ALL_CORES"

Experienced `libEnoptMP` users may enable debug output by setting the environment
variable `ENOPT_ENABLE_DEBUG_OUTPUT` to 1.

### Environment variables

* `ENOPT_ENABLE_DEBUG_OUTPUT` (default=none)

    Enables debug output.

### If anything fails

1. Check whether the plugin library can be loaded from the `LD_LIBRARY_PATH`.

2. Write a mail to the author.

## Authors

See AUTHORS file.
