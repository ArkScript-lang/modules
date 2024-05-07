cmake_minimum_required(VERSION 3.8)

project(msgpack)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${Ark_SOURCE_DIR}/include
    ${Modules_SOURCE_DIR}/submodules/msgpack-cpp/include)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${Modules_SOURCE_DIR}/submodules/msgpack-cpp/src/*.c)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ArkReactor)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${PROJECT_NAME}>" ${PROJECT_SOURCE_DIR}/../../../${PROJECT_NAME}.arkm)
