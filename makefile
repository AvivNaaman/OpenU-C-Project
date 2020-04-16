# final executable compilation & linkage
assembler: assembler.o processfile.o instructions.o table.o firstpass.o utils.o
	gcc -g assembler.o processfile.o instructions.o table.o firstpass.o utils.o -ansi -Wall -pedantic -o assembler


# modules (sorted by importancy):
## main module
assembler.o: assembler.c processfile.h
	gcc -c assembler.c -ansi -Wall -pedantic -o assembler.o

## single file process module
processfile.o: processfile.c processfile.h
	gcc -c processfile.c -ansi -Wall -pedantic -o processfile.o


## first pass module
firstpass.o: firstpass.c firstpass.h instructions.h
	gcc -c firstpass.c -ansi -Wall -pedantic -o firstpass.o

## symbol table module
table.o: table.c table.h
	gcc -c table.c -ansi -Wall -pedantic -o table.o

## instructions helper function module
instructions.o: instructions.c instructions.h utils.h
	gcc -c instructions.c -ansi -Wall -pedantic -o instructions.o

## common function & definitions module
utils.o: utils.c utils.h
	gcc -c utils.c -ansi -Wall -pedantic -o utils.o


