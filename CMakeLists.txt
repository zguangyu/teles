cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(teles VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

# Initialize dependencies
if(EXISTS "${PROJECT_SOURCE_DIR}/deps/cppzmq")
    message(STATUS "Found deps")
else()
    message(STATUS "Cloning deps")
    execute_process(COMMAND git submodule update --init)
endif()

set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "Enable cppzmq test")
add_subdirectory(deps/cppzmq)
