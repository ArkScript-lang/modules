cmake_minimum_required(VERSION 3.16)

project(re)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(ABSL_ENABLE_INSTALL ON)
set(ABSL_PROPAGATE_CXX_STD ON)
add_subdirectory(${Modules_SOURCE_DIR}/submodules/abseil abseil EXCLUDE_FROM_ALL SYSTEM)
add_subdirectory(${Modules_SOURCE_DIR}/submodules/re2 re2 EXCLUDE_FROM_ALL SYSTEM)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${ark_SOURCE_DIR}/thirdparties/fmt/src/format.cc)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${ark_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ArkReactor re2::re2)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${PROJECT_NAME}>" ${ark_SOURCE_DIR}/lib/${PROJECT_NAME}.arkm)
