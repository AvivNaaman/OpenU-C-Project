# final executable
assembler: assembler.o processfile.o first_pass_line.o utils.o table.o instructions_handler.o
	gcc -g assembler.o processfile.o first_pass_ln.o instructions_handler.o table.o  -ansi -Wall -pedantic -o assembler

assembler.o: assembler.c processfile.h
	gcc -c assembler.c -ansi -Wall -pedantic -o assembler.o

processfile.o: processfile.c processfile.h globals.h
	gcc -c processfile.c -ansi -Wall -pedantic -o processfile.o

first_pass_ln.o: first_pass_line.c first_pass_line.h
	gcc -c first_pass_line.c -ansi -Wall -pedantic -o first_pass_ln.o

instructions_handler.o: instructions_handler.c instructions_handler.h
	gcc -c instructions_handler.c -ansi -Wall -pedantic -o instructions_handler.o

table.o: table.c table.h
	gcc -c table.c -ansi -Wall -pedantic -o table.o

utils.o: utils.c utils.h
	gcc -c utils.c -ansi -Wall -pedantic -o utils.o

