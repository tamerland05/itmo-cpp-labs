include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        my_ranges_tests
        my_ranges_tests.cpp
)

target_link_libraries(
        my_ranges_tests
        my_ranges
        GTest::gtest_main
)

target_include_directories(my_ranges PUBLIC ${PROJECT_SOURCE_DIR})
include(GoogleTest)

gtest_discover_tests(my_ranges_tests)

