cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 90)

project(UnitTests C)
add_executable(UnitTests assertions.c assertions.h UnitTests.c ../processfile.c ../processfile.h
		../table.c ../table.h ../first_pass_line.c ../first_pass_line.h
		../instructions_handler.c ../instructions_handler.h ../utils.c ../utils.h)
target_link_libraries(UnitTests m)