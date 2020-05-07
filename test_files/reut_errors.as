; Error tester: v1 by Reut Cohen @ openu
; v2 by @AvivNaaman
NUM: .entry NUM
add r3,   #4
cmp #5, #-29
He45: jsr r3
prn NUM
LOOP.2: .string "Hello"
LABEL: .extern LABEL
LABEL: .data 5  , -25,  -5, 14, 1
    .data , 5,   12 ,  -110
    .data 5 ,, 22 , 45
    .data 34  , -100 ,
    LABEL:
Main: .string "abc"
Main: .data  5
    .entry Main
lea r2 , r5
string: mov 5 , r8
stop r5
add Main#5
sub # 5, *r6
    .string "hffonhof  543 ^&*"
    .string "ggregtrhgthc
clr r11
r12: dec *r6
label234: cmp #45,#3 r5
bne &r5
jmp123: jmp r2 , *r6
jmp jmp123
jmp: red r7
laBel12djewoijdweoijewodjedjejdojdeoijoe: clr *r4
    .entry  Main
    .extern Main