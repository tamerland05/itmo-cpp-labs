cmake_minimum_required(VERSION 3.13)

project(
        labwork10
        VERSION 1.0
        DESCRIPTION "C++ Year 2024 Labwork10"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)
