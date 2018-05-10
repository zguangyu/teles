include(${PROJECT_SOURCE_DIR}/cmake/googletest.cmake)

fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
)

add_subdirectory(libteles)