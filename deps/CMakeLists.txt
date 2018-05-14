set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "Enable cppzmq test" FORCE)
add_subdirectory(cppzmq)

add_subdirectory(spdlog)

# Enable draft api will cause zyre fail to compile
# This has to be set to NO instead of OFF
set(ENABLE_DRAFTS OFF CACHE BOOL "Enable zyre draft api" FORCE)
set(BUILD_EXAMPLE OFF CACHE BOOL "Enable zyre draft api" FORCE)
add_subdirectory(zyre)

if(TELES_TEST)
    add_subdirectory(googletest)
endif()
