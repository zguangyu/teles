cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(teles VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS program_options)

# Initialize dependencies
if(EXISTS "${PROJECT_SOURCE_DIR}/deps/cppzmq")
    message(STATUS "Found deps")
else()
    message(STATUS "Cloning deps")
    execute_process(COMMAND git submodule update --init)
endif()

option(TELES_TEST "Enable teles test" ON)

set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "Enable cppzmq test")

add_subdirectory(deps)

add_subdirectory(libteles)

if(TELES_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
