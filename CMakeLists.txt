cmake_minimum_required(VERSION 3.8)

project(Modules)

set(ARK_MOD_RAND    Off CACHE BOOL "Build the random module")
set(ARK_MOD_CONSOLE On  CACHE BOOL "Build the console module")
set(ARK_MOD_MSGPACK Off CACHE BOOL "Build the msgpack module")
set(ARK_MOD_HTTP    Off CACHE BOOL "Build the http module")
set(ARK_MOD_JSON    Off CACHE BOOL "Build the json module")
set(ARK_MOD_HASH    Off CACHE BOOL "Build the hash module")
set(ARK_MOD_BITWISE    Off CACHE BOOL "Build the bitwise module")
set(ARK_MOD_DATABASE   Off CACHE BOOL "Build the database module")
set(ARK_MOD_ALL     Off CACHE BOOL "Build all the modules")

if (ARK_MOD_RAND OR ARK_MOD_ALL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/random)
endif()

if (ARK_MOD_CONSOLE OR ARK_MOD_ALL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/console)
endif()

if (ARK_MOD_MSGPACK OR ARK_MOD_ALL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/msgpack)
endif()

if (ARK_MOD_HTTP OR ARK_MOD_ALL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/http)
endif()

if (ARK_MOD_JSON OR ARK_MOD_ALL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/json)
endif()

if (ARK_MOD_HASH OR ARK_MOD_ALL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/hash)
endif()

if (ARK_MOD_BITWISE OR ARK_MOD_ALL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/bitwise)
endif()

if (ARK_MOD_DATABASE OR ARK_MOD_ALL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/database)
endif()