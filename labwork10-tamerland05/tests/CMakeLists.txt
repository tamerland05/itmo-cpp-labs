add_executable(
        my_scheduler_tests
        scheduler_tests.cpp
)

target_link_libraries(
        my_scheduler_tests
        my_scheduler
        GTest::gtest_main
)

target_include_directories(my_scheduler PUBLIC ${PROJECT_SOURCE_DIR})
include(GoogleTest)

gtest_discover_tests(my_scheduler_tests)

