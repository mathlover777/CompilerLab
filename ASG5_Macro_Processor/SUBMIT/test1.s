TEST 	START 		2000
SWAP 	MACRO 		&I,&J
		LDA 		&I
		STA 		TEMP
		LDA 		&J
		STA 		&I
		LDA 		TEMP
		STA 		&J
		MEND
FIRST	LDA 		ONE
		STA 		I
		LDA 		FIVE
		STA 		J
		SWAP 		I,J
I 		RESW 		1
J 		RESW 		1
TEMP 	RESW 		1
ONE 	WORD 		1
FIVE 	WORD 		5
END 	FIRST
