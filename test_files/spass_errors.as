; Second pass errors

DATALABEL: .string "My String.$#"
.extern EXTERNLABEL
.entry DATALABEL
.entry CODELABEL
CODELABEL: mov r0, r1

; external label cannot be defined as an entry one as well
.entry EXTERNLABEL

; jumping to data label is invalid
jmp &DATALABEL


