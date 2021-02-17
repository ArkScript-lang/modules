cmake_minimum_required(VERSION 3.8)

project(Modules)

set(MODULE_EXTENSION "arkm")

set(ARK_MOD_RAND    Off CACHE BOOL "Build the random module")
set(ARK_MOD_CONSOLE On  CACHE BOOL "Build the console module")
set(ARK_MOD_MSGPACK Off CACHE BOOL "Build the msgpack module")
set(ARK_MOD_HTTP    Off CACHE BOOL "Build the http module")
set(ARK_MOD_JSON    Off CACHE BOOL "Build the json module")
set(ARK_MOD_HASH    Off CACHE BOOL "Build the hash module")

if (ARK_MOD_RAND)
    add_subdirectory(${PROJECT_SOURCE_DIR}/random)
endif()

if (ARK_MOD_CONSOLE)
    add_subdirectory(${PROJECT_SOURCE_DIR}/console)
endif()

if (ARK_MOD_MSGPACK)
    add_subdirectory(${PROJECT_SOURCE_DIR}/msgpack)
endif()

if (ARK_MOD_HTTP)
    add_subdirectory(${PROJECT_SOURCE_DIR}/http)
endif()

if (ARK_MOD_JSON)
    add_subdirectory(${PROJECT_SOURCE_DIR}/json)
endif()

if (ARK_MOD_HASH)
    add_subdirectory(${PROJECT_SOURCE_DIR}/hash)
endif()