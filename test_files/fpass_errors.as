;; First pass errors - mostly syntax validation, but also double definitions!

; label errors
LABEL: .data -0,5,6,7,2,6
CODELABEL: mov r0,r1
LOCALCODE: add r0,r1
LOCALDATA: .string "Some local data"
LABEL: .string "Same label twice!"
CODELABEL: .data "Again!"
.extern LOCALCODE
.extern LOCALDATA
.extern WHATEVEREXT
.extern LONGLONGLONGLONGLONGLONGLABEL
.extern 1nvalidLabel
.extern Label@With4t
.extern
WHATEVEREXT: .string "Another definition!"
VERY_VERY_VERY_VERY_VERY_LONG_LABEL_DEFINITION_SO_ITS_ILLEGAL: .data 0
SOME*INVALID&CHARS%HERE: .data 5
2tartWithNumISILLEGAL: .data -6
ALLVALIDBUTLASTI$: .string "more invalid!"

; data and string errors
.string "No closing
.string No opening"
.string Not at all
.data ,5,6,-7
.data 5,,6,-7
.data 5,6,,-7
.data 5,6,-7,
.data 5 6 -7
.data 5 6 , -7
.data 5 , 6 -7,
.data 0.5
.data 5.9
.data -0.1
.data .5
; 2^23+1
.data 8388609
.data -8388609
.data -5,8388609,-8388609

; Operand count errors
mov
mov r0
mov r0,LABEL,#5
cmp
cmp LABEL
cmp #-0,MYLABEL,HERE
add
add r7
add #5,#6,#7
sub
sub LABEL
sub #4,LABEL,r0
lea
lea operand
lea firstoperand,secondoperand
clr
clr ope,rand
not
not r4, label
inc
inc r6,r5,r4
dec
dec lambdaX,XequalsY
jmp
jmp somewhere,around
bne
bne &realtivelyjump,notToday
jsr
jsr WeLikeJumpingAround,ButShouldBeError
red
red alpha,beta
prn
prn #48,#56
prn #4,#0,#9
rts hello
rts hello,goodbye
stop myprog
stop myprog,yourprog

; Addressing type errors (by booklet p.34)

mov &source, #0
mov &source, dest
mov &source, r0
mov &source, &dest
mov #5, &dest
mov source, &dest
mov r0, &dest
mov &source, #0
mov #5, #0
mov source, #0
mov r0, #0
mov #5,#0

cmp &source, #5
cmp &source, dest
cmp &source, &dest
cmp &source, r0
cmp #5, &dest
cmp source, &dest
cmp r0, &dest

add &source, #0
add &source, dest
add &source, r0
add &source, &dest
add #5, &dest
add source, &dest
add r0, &dest
add &source, #0
add #5, #0
add source, #0
add r0, #0
add #5,#0

sub &source, #0
sub &source, dest
sub &source, r0
sub &source, &dest
sub #5, &dest
sub source, &dest
sub r0, &dest
sub &source, #0
sub #5, #0
sub source, #0
sub r0, #0
sub #5,#0

lea #5, #0
lea #5, dest
lea #5, &dest
lea #5, r0
lea &source, #0
lea &source, dest
lea &source, &dest
lea &source, r0
lea r0, #0
lea r0, dest
lea r0, &dest
lea r0, r0
lea source, #0
lea &source, #0
lea r0, #0
lea #0, &dest
lea source, &dest
lea r0, &dest
lea #0, r0
lea source,r0
lea &source, r0

clr #5
clr &source
not #9
not &source
inc #-1
inc &source
dec #0
dec &source
red #9
red &source

jmp source
jmp #4
bne source
bne #9
jsr source
jsr #-5

prn &source

; More General syntax errors
mov $,#5
mov 6.7
mov 123
mov VERYVERYVERYVERYVERYLONGLABELWITH36C
mov $SomeLabel
.entry LONGLONGLONGLONGLONGLONGLONGLONGLABEL1234
.extern REALLYREALLYREALLYREALLYREALLYREALLYLONG
op operand,operand
label : illegal
lable!:illegal
label: mov .string "Hello World!"
labell: mov .data "Hello World!"
label .data "Hello World!" mov r0,#5

string: .string "mystr"
entry: .data 12
extern: .data +5

TOOOOOOOOOOOOOOOOOOOOOOLOOOOOOOOOOOOOOOOOOONGGGGGGGGGGGGGGGGGGTOOOOOOPROCESSSSSSS
.extern HELLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLOO