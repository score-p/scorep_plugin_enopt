project(EnoptMP)

cmake_minimum_required(VERSION 2.8)

#Input Options
option(ENOPT_STATIC "Link libenopt statically." ON)
option(ENOPT_DIR  "Path to libenopt directory" "")
option(ENOPT_INC  "Path to libenopt headers" "")
option(ENOPT_LIB  "Path to libenopt" "")

#additional c flags
set(CMAKE_C_FLAGS "-D_GNU_SOURCE -g -std=gnu99 -fopenmp -pthread")

#debugging c flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wstrict-prototypes -Wall -Wundef -Wno-long-long -Wsign-compare -Wcomment -pedantic -finline-functions -fno-strict-aliasing")
set(CMAKE_C_FLAGS_RELEASE "-Os")

include(../scorep_plugin_common/FindScorep.cmake)
if( SCOREP_FOUND )
    include_directories(${SCOREP_INCLUDE_DIRS})
    link_directories(${SCOREP_LIBRARY_DIRS})
    SET(CMAKE_INSTALL_PREFIX ${SCOREP_ROOT_PATH} CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
else()
    message(SEND_ERROR "Scorep was not found but is required!")
endif()

include(../scorep_plugin_common/FindTuningSubstrate.cmake)
if( TUNING_SUBSTRATE_FOUND )
    include_directories(${TUNING_SUBSTRATE_INCLUDE_DIRS})
    link_directories(${TUNING_SUBSTRATE_LIBRARY_DIRS})
    SET(CMAKE_INSTALL_PREFIX ${TUNING_SUBSTRATE_ROOT_PATH} CACHE PATCH "Install path prefix, prepended onto install directories." FORCE)
else()
    message(SEND_ERROR "Tuning substrate plugin was not found but is required!")
endif()

# find libenopt
find_path(ENOPT_INC_DIR NAMES enoptC.h defines.h HINTS ${ENOPT_INC} ${ENOPT_DIR}/include DOC "Path to enoptC.h")
if( ENOPT_INC_DIR )
	if( ENOPT_STATIC )
		message("Using static libenopt")
		message("Linking might fail if you use the static version of libenopt and your library is not compiled with -fPIC. In this case use the cmake flag -DENOPT_STATIC=OFF")
		set(LIBENOPT_NAME libenopt.a)
	else()
		set(LIBENOPT_NAME libenopt.so)
	endif()
	find_path(ENOPT_LIB_DIR ${LIBENOPT_NAME} HINTS ${ENOPT_LIB} ${ENOPT_DIR}/lib ENV LD_LIBRARY_PATH DOC "Path to ${LIBENOPT_NAME}")
	if ( ENOPT_LIB_DIR )
		
	else ()
    	message(SEND_ERROR "${LIBENOPT_NAME} not found, but required, use -DENOPT_DIR or -DENOPT_LIB!")
	endif ()
else()
    message(SEND_ERROR "enoptC.h not found, but required, use -DENOPT_DIR or -DENOPT_INC!")
endif()

include_directories(${ENOPT_INC_DIR})
link_directories(${ENOPT_LIB_DIR})

add_library(EnoptMP SHARED EnoptMP.c Shared.c)
add_library(EnoptTP SHARED EnoptTP.c Shared.c)

target_link_libraries(EnoptMP pthread ${LIBENOPT_NAME})
target_link_libraries(EnoptTP pthread ${LIBENOPT_NAME})

install(TARGETS EnoptMP EnoptTP LIBRARY DESTINATION ${TUNING_SUBSTRATE_ROOT_PATH}/lib/plugins)

message ("SCOREP_FOUND                  =${SCOREP_FOUND}")
message ("SCOREP_ROOT_PATH              =${SCOREP_ROOT_PATH}")
message ("SCOREP_INCLUDE_DIRS           =${SCOREP_INCLUDE_DIRS}")
message ("SCOREP_LIBRARY_DIRS           =${SCOREP_LIBRARY_DIRS}")
message ("TUNING_SUBSTRATE_FOUND        =${TUNING_SUBSTRATE_FOUND}")
message ("TUNING_SUBSTRATE_ROOT_PATH    =${TUNING_SUBSTRATE_ROOT_PATH}")
message ("TUNING_SUBSTRATE_INCLUDE_DIRS =${TUNING_SUBSTRATE_INCLUDE_DIRS}")
message ("TUNING_SUBSTRATE_LIBRARY_DIRS =${TUNING_SUBSTRATE_LIBRARY_DIRS}")
message ("ENOPT_STATIC                  =${ENOPT_STATIC}")
message ("ENOPT_DIR                     =${ENOPT_DIR}")
message ("ENOPT_INC                     =${ENOPT_INC}")
message ("ENOPT_INC_DIR                 =${ENOPT_INC_DIR}")
message ("ENOPT_LIB                     =${ENOPT_LIB}")
message ("ENOPT_LIB_DIR                 =${ENOPT_LIB_DIR}")
message ("LIBENOPT_NAME                 =${LIBENOPT_NAME}")
message ("CMAKE_SOURCE_DIR              =${CMAKE_SOURCE_DIR}")
