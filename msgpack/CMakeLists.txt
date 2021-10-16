cmake_minimum_required(VERSION 3.8)

project(msgpack)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${PROJECT_SOURCE_DIR}/include
    ${Ark_SOURCE_DIR}/thirdparty
    ${Ark_SOURCE_DIR}/include
    ${Modules_SOURCE_DIR}/submodules/msgpack-cpp/include)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${Modules_SOURCE_DIR}/submodules/msgpack-cpp/src/*.c)

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
        "$<TARGET_FILE:${PROJECT_NAME}>" ${PROJECT_SOURCE_DIR}/../../${PROJECT_NAME}.${MODULE_EXTENSION})
