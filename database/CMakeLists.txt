cmake_minimum_required(VERSION 3.8)

project(database)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(SQLite3)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${ark_SOURCE_DIR}/include
    ${SQLite3_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ArkReactor ${SQLite3_LIBRARIES})

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${PROJECT_NAME}>" ${PROJECT_SOURCE_DIR}/../../${PROJECT_NAME}.arkm)
