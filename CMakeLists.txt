cmake_minimum_required(VERSION 3.8)

project(Modules)

set(MODULE_EXTENSION "arkm")

add_subdirectory(${PROJECT_SOURCE_DIR}/random)
add_subdirectory(${PROJECT_SOURCE_DIR}/console)
add_subdirectory(${PROJECT_SOURCE_DIR}/msgpack)
add_subdirectory(${PROJECT_SOURCE_DIR}/http)
add_subdirectory(${PROJECT_SOURCE_DIR}/json)
add_subdirectory(${PROJECT_SOURCE_DIR}/hash)