.$$$$$$\...$$$$$$\...$$$$$$\..$$$$$$$\................$$\.......$$$$$$\..
$$..__$$\.$$..__$$\.$$..__$$\.$$..__$$\.............$$$$.|.....$$$.__$$\.
$$./..\__|$$./..\__|$$./..\__|$$.|..$$.|............\_$$.|.....$$$$\.$$.|
\$$$$$$\..\$$$$$$\..\$$$$$$\..$$$$$$$\.|..............$$.|.....$$\$$\$$.|
.\____$$\..\____$$\..\____$$\.$$..__$$\...............$$.|.....$$.\$$$$.|
$$\...$$.|$$\...$$.|$$\...$$.|$$.|..$$.|..............$$.|.....$$.|\$$$.|
\$$$$$$..|\$$$$$$..|\$$$$$$..|$$$$$$$..|............$$$$$$\.$$\\$$$$$$../
.\______/..\______/..\______/.\_______/.............\______|\__|\______/.
.........................................................................
.........................................................................
.........................................................................


   __________ ___________   _____   ________             _____   ___________
   \______   \\_   _____/  /  _  \  \______ \           /     \  \_   _____/
    |       _/ |    __)_  /  /_\  \  |    |  \         /  \ /  \  |    __)_ 
    |    |   \ |        \/    |    \ |    `   \       /    Y    \ |        \
    |____|_  //_______  /\____|__  //_______  /       \____|__  //_______  /
           \/         \/         \/         \/                \/         \/ 

Thanks for using SSSB SIC Assembler !!!!!!!!!!!!!

As per requirements , the program is written in two steps 

pass1.c ----> by Sourav Sarkar		(11CS30037)
pass2.c ----> by Shrihari A. Bhat	(11CS3008)

Description of Pass 1:

How to run :
	if inputfile = "input" and intermediate file = "INTFILE"---->
	"./pass1 input INTFILE"

	To compile with ALWAYSINCREMENT(discussed later) FLAG (0 or 1 say x)---->
	"./pass1 input INTFILE x"

	<any other format will simply dont run the program>

MODES:
	
ALWAYSINCREMENT = 1 // reserves 3 bytes even if lines are ERRORNEUS
ALWAYSINCREMENT = 0 // doesnot reservers 3 bytes if there are ERROR...so actually object code will be overwritten

ERRORS DETECTION :
 The following ERRORs are "FATAL ERRORS" ie whenever they are detected program will be terminated
 that means INT file and SYMTAB will be incomplete

	 1> Input file not found
	 2> START is written but no program name is given
	 3> Starting address is more that 2 bytes
	 4> If starting address is specified and not in HEX
	 5> OUT OF MEMORY
	 6> END does not exist in the program
	 7> The END operand does not exist or not given
	 8> '!' found in program

 The following are considered as ERROR or WARNINGs(ie program will not stop but in the intermediate flag will 
 contain certain flags such that pass2 can detect them exactly where they occured)

	ERRORCODE=1;// instruction not found

	ERRORCODE=2;// label already exists in SYMTAB

	ERRORCODE=3;// non decimal numerical or address reference

	ERRORCODE=4;// too few words

	ERRORCODE=5;// too many words

	ERRORCODE=6;// too large words

	ERRORCODE=7;// Missing or invalid(including 0) operand ... not an error 
				// reserving 1 byte/3 byte depending on keyword 

	ERRORCODE=8;// MISSING or INVALID OPERAND AFTER WORD

NOTE : The Errors are written in less to more dominant fashion and the intermediate file will give the actual idea
this flag will be written at the end of each line of the intermediate file

if no error is detected then ERRORCODE will be 0


Structue of Intermediate file:

<line number> <label> <instruction> <operand> <error code>

line number will always will be there . If any one of the fields like label or instruction or operand is not present 
then "!" sign will be there so that pass2's job is easy. So taking ! as a word each line in the intermediate file will
contain 6 fileds

NOTE: 	if there are comments like "LDA A # its a comment" then this comment will be ignored
		ie not written in the intermediate file

		if a comment is like "# The only thing in is line is a comment" then that will be written in the intermediate file
		like "# The only thing in is line is a comment" and no line number will be assigned to it and Location Counter will
		not be incremented.So comments can be done as desired.


Special Notes : 


0> '!' is an illegal character !!!
1> Comments can be placed as wished anywhere with a # tag
2> The lines are freeform !! ie each line must contain a fixed number of words that any number of tabs or spaces
between words are allowed,,,so while writting code any editor and any style can be used
3> Assumptions while extracting words --->  if <2 words are there then its ERRORCODE = 4
											if >3 words are there then ERRORCODE = 5
											if only one word is there that will be taken as instruction
											if two words are there then the format is <instruction> <operand>
											if three words are there then the format is standard
											<label> <instruction> <operand>
											as we have a function getContent to extract words from the lines
											so this conventions can be modified without changing the assembler
											design
4> Starting address(if mentioned) must be given in HEX

FAQ : 
	1> The line is "NINE WORD " ERROR code 8 must be shown but ERRORCODE = 1 is shown ?

	ANS : Refer to "Special Notes". As the line contains only 2 word so ins = 'NINE' and operand = 'WORD'.
	But nine is not a instruction so we get error 1. "WORD" was not treated as an assembler directive.So 
	consistency is maintained. "NINE WORD ERR" will give error 8.

	2> Can we use '!' in the program ?

	ANS : N0 !!'!'.As ! is indicated to show absence of field to pass2 , if we allow that pass2 may face
	problems.So that is prohibited. So whenever that is detected ,[probably in getContent to be precise]
	we will have a staraight exit(-1) from there !!.

	3> No support for "BYTE" and "RESB" keyword ?

	ANS : Sir told that we can skip that.

	4> How to disable the silly colors(if visible) ?

	ANS : Just in the starting of the file find a "const int ENABLECOLOR = 1" , Set it to 0.