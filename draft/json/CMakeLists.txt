cmake_minimum_required(VERSION 3.16)

project(json)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${Modules_SOURCE_DIR}/submodules/json/single_include)

file(GLOB_RECURSE SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${ark_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ArkReactor)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${PROJECT_NAME}>" ${PROJECT_SOURCE_DIR}/../../../${PROJECT_NAME}.arkm)
