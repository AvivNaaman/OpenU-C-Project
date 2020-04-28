assembler: assembler.o code.o fpass.o spass.o instructions.o table.o utils.o writefiles.o globals.h 
	gcc -g assembler.o code.o fpass.o spass.o instructions.o table.o utils.o writefiles.o -ansi -Wall -pedantic -o assembler

## Main:
assembler.o: assembler.c assembler.h globals.h
	gcc -c assembler.c -ansi -Wall -pedantic -o assembler.o

## Code helper functions:
code.o: code.c code.h globals.h
	gcc -c code.c -ansi -Wall -pedantic -o code.o

## First Pass:
fpass.o: first_pass.c first_pass.h globals.h
	gcc -c first_pass.c -ansi -Wall -pedantic -o fpass.o

## Second Pass:
spass.o: second_pass.c second_pass.h globals.h
	gcc -c second_pass.c -ansi -Wall -pedantic -o spass.o

## Instructions helper functions:
instructions.o: instructions.c instructions.h globals.h
	gcc -c instructions.c -ansi -Wall -pedantic -o instructions.o

## Table:
table.o: table.c table.h globals.h
	gcc -c table.c -ansi -Wall -pedantic -o table.o

## Useful functions:
utils.o: utils.c instructions.h globals.h
	gcc -c utils.c -ansi -Wall -pedantic -o utils.o

## Output Files:
writefiles.o: writefiles.c writefiles.h globals.h
	gcc -c writefiles.c -ansi -Wall -pedantic -o writefiles.o
