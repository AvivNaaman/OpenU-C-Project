; print_characters.as (c) Nadav Kahlon


MAIN: 			red r5
				sub char0, r5
				
				red startChar
				mov startChar, endChar
				add r5, endChar
				.extern startChar
endChar:		.data 1, -78
				jsr &PrintChars
				
				clr startChar
				clr endChar
				stop
.entry MAIN
	


PrintChars:		mov startChar, r0
CharLoop:		prn r0
				inc r0
				cmp r0, endChar
				bne EndCharLoop
				jmp &CharLoop
EndCharLoop:	rts
.entry PrintChars



char0: .string "0"
