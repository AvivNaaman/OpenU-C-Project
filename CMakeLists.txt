cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_STANDARD 90)


# Default CLion generated
project(mmn14 C)
add_executable(mmn14 assembler.c
		table.c table.h
		instructions.c instructions.h utils.c utils.h code.c code.h writefiles.c writefiles.h globals.h
		first_pass.c first_pass.h second_pass.c second_pass.h)
## math library, gcc option -lm
target_link_libraries(mmn14 m)
## add warning flags -pedantic -Wall
set (CMAKE_CXX_FLAGS "-ansi -pedantic -Wall")

# Using makefile for compilation
project(assembler C)
add_custom_target(assembler COMMAND make -C ${assembler_SOURCE_DIR} CLION_EXE_DIR=${PROJECT_BINARY_DIR})