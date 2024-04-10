cmake_minimum_required(VERSION 3.8)

project(http)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${Modules_SOURCE_DIR}/submodules/cpp-httplib
    ${ark_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ArkReactor OpenSSL::Crypto OpenSSL::SSL)

target_compile_features(ArkReactor PRIVATE cxx_std_20)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${PROJECT_NAME}>" ${ark_SOURCE_DIR}/lib/${PROJECT_NAME}.arkm)
