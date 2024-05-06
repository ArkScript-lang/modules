cmake_minimum_required(VERSION 3.8)

project(Modules)

set(ARK_MOD_ALL Off CACHE BOOL "Build all the modules")
set(ARK_MOD_DRAFT Off CACHE BOOL "Build all draft modules")

if (ARK_MOD_ALL)
    message(STATUS "Building modules in modules/src")
    add_subdirectory(src)
endif ()

if (ARK_MOD_DRAFT)
    message(STATUS "Building modules in modules/draft")
    add_subdirectory(draft)
endif ()
