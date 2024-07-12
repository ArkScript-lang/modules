cmake_minimum_required(VERSION 3.8)

project(bitwise)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${ark_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ArkReactor)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${PROJECT_NAME}>" ${ark_SOURCE_DIR}/lib/${PROJECT_NAME}.arkm)
