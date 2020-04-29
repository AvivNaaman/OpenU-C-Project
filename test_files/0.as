MAIN: add r3,LIST
LOOP: prn #48
lea W,r6
inc r6
mov r3,K
sub r1,r4
bne END
cmp K, #-6
bne &END
dec W
;Comment0
.entry MAIN
jmp &LOOP
add L3,L3
END: stop
STR: .string "abcd"
;Comment1
LIST: .data 6,-9
.data -100
K: .data 31
.entry LIST
.extern W
.extern L3