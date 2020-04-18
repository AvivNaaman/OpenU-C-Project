![Build](https://github.com/avivnaaman/20465-project/workflows/Build/badge.svg?branch=master)
## About
This project is an assignment in a C programming course. The purpose of this project is to build an assembler for 15-instruction asssembly language, for an imaginary 24-bit CPU and 21 bits of memory size.
This project was done by @AvivNaaman and @YotamGranot.
For any issues, questions and ideas turn to the Issues tab.
### Directory structure
1. assembler.c - main function definition, argv & argc processing
2. processfile.c - processing of single file
3. firstpass.c - first pass methods for processing single line
4. utils.c - contains common functions & tools
5. table.c - contains definition of a basic table data structure
6. instructions.c - contains methods for processing an instruction (.data, .string, etc.)
7. code.c - contains methods for processing a code line (one that contains operation such as mov,jmp,etc.)
