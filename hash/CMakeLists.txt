cmake_minimum_required(VERSION 3.8)

project(hash)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${ark_SOURCE_DIR}/thirdparty
    ${ark_SOURCE_DIR}/include
    ${ark_SOURCE_DIR}/lib/picosha2/)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ArkReactor)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${PROJECT_NAME}>" ${PROJECT_SOURCE_DIR}/../../${PROJECT_NAME}.arkm)
