add_executable(
        search_engine_tests
        search_engine_tests.cpp
)

target_link_libraries(
        search_engine_tests
        search_engine
        GTest::gtest_main
)

target_include_directories(search_engine PUBLIC ${PROJECT_SOURCE_DIR})
include(GoogleTest)

gtest_discover_tests(search_engine_tests)

