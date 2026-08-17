cmake_minimum_required(VERSION 3.20)

project(mwframebuf LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

pybind11_add_module(
    mwframebuf
    src/mwframebuffer2.cpp
)

target_link_libraries(
    mwframebuf
    PRIVATE
    SDL3::SDL3
)

install(
    TARGETS mwframebuf
    LIBRARY DESTINATION .
    RUNTIME DESTINATION .
)
