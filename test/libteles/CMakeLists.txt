file(GLOB TELES_TEST_SOURCES "*.cpp")
add_executable(teles-test ${TELES_TEST_SOURCES})

target_link_libraries(teles-test
    gtest
    gtest_main
    teles
)

add_test(teles-test teles-test)
