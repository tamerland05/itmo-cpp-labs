
include(..//)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

enable_testing()

add_executable(
        argparser_tests
        argparser_tests.cpp
)

target_link_libraries(
        argparser_tests
        argparser
        GTest::gtest_main
)

target_include_directories(argparser_tests PUBLIC ${PROJECT_SOURCE_DIR})
gtest_discover_tests(argparser_tests)

add_executable(
        hamming_tests
        hamming_tests.cpp
)

target_link_libraries(
        hamming_tests
        hamming_code
        GTest::gtest_main
)

target_include_directories(hamming_tests PUBLIC ${PROJECT_SOURCE_DIR})
gtest_discover_tests(hamming_tests)