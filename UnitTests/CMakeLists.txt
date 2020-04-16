cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_STANDARD 90)

project(UnitTests C)
add_executable(UnitTests assertions.c assertions.h UnitTests.c ../processfile.c ../processfile.h
		../table.c ../table.h ../firstpass.c ../firstpass.h
		../instructions.c ../instructions.h ../utils.c ../utils.h testTable.c UnitTests.h testUtils.c testInstructions.c)
target_link_libraries(UnitTests m)