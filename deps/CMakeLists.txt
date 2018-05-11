add_subdirectory(
    cppzmq
    spdlog
)

if(TELES_TEST)
    add_subdirectory(googletest)
endif()
