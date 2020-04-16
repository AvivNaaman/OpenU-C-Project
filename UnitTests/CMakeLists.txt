cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 90)

project(UnitTests C)
add_executable(UnitTests assertions.c assertions.h)
target_link_libraries(UnitTests m)