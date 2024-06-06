cmake_minimum_required(VERSION 3.13)

project(
        labwork6
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Labwork6"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)