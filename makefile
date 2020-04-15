# final executable
assembler: assembler.o processfile.o
	gcc -g assembler.o processfile.o -ansi -Wall -pedantic -o assembler

assembler.o: assembler.c processfile.h
	gcc -c assembler.c -ansi -Wall -pedantic -o assembler.o

processfile.o: processfile.c processfile.h
	gcc -c processfile.c -ansi -Wall -pedantic -o processfile.o
