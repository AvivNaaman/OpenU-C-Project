![Build](https://github.com/avivnaaman/20465-project/workflows/Build/badge.svg?branch=master)
## About
This project is an assignment in a C programming course at the open unvieristy (20465). The purpose of this project is to build an assembler for 15-instruction asssembly language, for an imaginary 24-bit CPU and 21 bits of memory size.
This project was done by @AvivNaaman and @YotamGranot.
For any issues, questions and ideas turn to the Issues tab.

License: MIT License, Affect from September 1st, 2020
### Directory structure (Modules)
1. `assembler` - main function definition, argv & argc processing, single file processing.
2. `first_pass` - contains functions for processing a single line in the first pass and a code line in the first pass.
3. `second_pass` - contains function for processing a single line in second pass and replacing symbol by it's address.
4. `code` - contains useful function for processing code.
5. `instructions` - contains useful function for processing instruction.
6. `globals.h` - contains type and constant definitions.
7. `table` - contains definition of table-like linked list data structure.
8. `utils` - contains general-purposed, project-wide functions.
9. `writefiles` - contains methods for writing output files: `*.ob`, `*.ext` and `*.ent` 
