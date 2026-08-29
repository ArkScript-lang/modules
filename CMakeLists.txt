cmake_minimum_required(VERSION 3.16)

project(Modules)

set(ARK_REQUESTED_MODULES "" CACHE STRING "List of module names to build")

add_subdirectory(src)
add_subdirectory(draft)
