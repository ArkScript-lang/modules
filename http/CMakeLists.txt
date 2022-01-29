cmake_minimum_required(VERSION 3.8)

project(http)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/../include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../submodules/cpp-httplib
    ${ark_SOURCE_DIR}/thirdparty
    ${ark_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ArkReactor OpenSSL::Crypto OpenSSL::SSL)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${PROJECT_NAME}>" ${PROJECT_SOURCE_DIR}/../../${PROJECT_NAME}.${MODULE_EXTENSION})
