cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 90)

# Default CLion generated
project(mmn14 C)
add_executable(mmn14 assembler.c processfile.c processfile.h table.c table.h)
## math library, gcc option -lm
target_link_libraries(mmn14 m)

# Using makefile for compilation
project(assembler C)
add_custom_target(assembler COMMAND make -C ${assembler_SOURCE_DIR} CLION_EXE_DIR=${PROJECT_BINARY_DIR})