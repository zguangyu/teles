cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(teles VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(MySQLCPP REQUIRED)
find_package(Libuv REQUIRED)

# Initialize dependencies
if(EXISTS "${PROJECT_SOURCE_DIR}/deps/cppzmq")
    message(STATUS "Found deps")
    execute_process(COMMAND git submodule update)
else()
    message(STATUS "Cloning deps")
    execute_process(COMMAND git submodule update --init)
endif()

option(TELES_TEST "Enable teles test" ON)
option(TELES_UDP_PORT "The UDP port for zeromq discovery" "5670")
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "Enable cppzmq test")


add_subdirectory(deps)

if(CMAKE_INSTALL_PREFIX STREQUAL "/usr")
    set(TELES_ETC_PATH "/etc")
else()
    set(TELES_ETC_PATH "${CMAKE_INSTALL_PREFIX}/etc")
endif()

add_subdirectory(libteles)
add_subdirectory(src)

if(TELES_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
