add_subdirectory(proto)

file(GLOB TELES_SOURCES *.cpp)
add_library(teles SHARED ${TELES_SOURCES})

target_link_libraries(teles
    PUBLIC cppzmq
    PUBLIC Boost::boost
    PUBLIC Boost::program_options
    PUBLIC libuv
    PUBLIC proto
)

target_include_directories(teles PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/deps/spdlog/include>
    $<INSTALL_INTERFACE:include>
)
