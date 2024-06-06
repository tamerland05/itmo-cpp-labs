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
        array_tests array_tests.cpp
)

target_link_libraries(
        array_tests
  GTest::gtest_main
)
target_link_libraries(${PROJECT_NAME} PRIVATE triple_array)
target_link_libraries(${PROJECT_NAME} PRIVATE wrap)

target_include_directories(array_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(array_tests)
