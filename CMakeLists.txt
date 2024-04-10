cmake_minimum_required(VERSION 3.8)

project(Modules)

set(ARK_MOD_ALL Off CACHE BOOL "Build all the modules")
set(ARK_MOD_DRAFT Off CACHE BOOL "Build all draft modules")

if (ARK_MOD_ALL)
    add_subdirectory(${PROJECT_SOURCE_DIR}/src)
endif ()

if (ARK_MOD_DRAFT)
    add_subdirectory(${PROJECT_SOURCE_DIR}/draft)
endif ()
