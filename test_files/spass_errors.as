; Second pass errors

DATALABEL: .string "My String.$#"
MYSTRING: .string "My other string"
MYDATA: .data 5,4,3,2,1
X: .string "XXXXX"

.extern EXTERNLABEL
.entry DATALABEL
.entry CODELABEL
CODELABEL: mov r0, r1

; external label cannot be defined as an entry one as well
.entry EXTERNLABEL

; jumping to data label is invalid
jmp &DATALABEL

prn MYSTRINGS
prn MYSTRIN
prn YSTRING
prn MYSTRING123
prn MYSTRING0

prn XX
prn Y
prn X0
prn X

.entry SOMEUNKNOWNLABEL
.entry MYSTRING
.entry MYDATA
.entry MYDATA0
.entry 0MYDATA
.entry 0MYSTRING
.entry 0
.entry NULL

; Assembler will ignore
EMPTYLABEL:

jmp &EMPTYLABEL
