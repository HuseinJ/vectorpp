cmake_minimum_required(VERSION 3.14)
project(vectorpp-add LANGUAGES CXX)

include("../../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(vectorpp REQUIRED)
endif()

set(sources main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(vectorpp-add)
target_sources(vectorpp-add PRIVATE ${sources})
target_link_libraries(vectorpp-add PRIVATE vectorpp::vectorpp)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(vectorpp-add vectorpp::vectorpp)
endif()
