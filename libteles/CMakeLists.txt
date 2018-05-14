add_subdirectory(proto)

file(GLOB TELES_SOURCES *.cpp)
add_library(teles SHARED ${TELES_SOURCES})

target_link_libraries(teles
    PUBLIC cppzmq
    PUBLIC Boost::boost
    PUBLIC Boost::program_options
    PUBLIC libuv
    PUBLIC proto
    PUBLIC spdlog
    PUBLIC zyre
)

target_include_directories(teles PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

configure_file(config.hpp.in config.hpp @ONLY)
