cmake_minimum_required(VERSION 3.12)

set(CMAKE_EXE_LINKER_FLAGS "-static")

project(
        Sandpile
        VERSION 1.0
        DESCRIPTION "C++ Year 2022 Lab3"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/converting_to_bmp)
add_subdirectory(lib/sandpile)
add_subdirectory(bin)
