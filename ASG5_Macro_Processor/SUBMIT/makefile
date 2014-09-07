all:macroProcessor macroASM pass1 pass2
macroProcessor:macroDriver.c macro.c fileio.c string.c
	g++ macroDriver.c -o macroProcessor

macroASM:macroAssembler.c macro.c macroDriver.c fileio.c string.c pass1.c pass2.c
	g++ macroAssembler.c -g -o macroASM

pass1:pass1.c
	g++ pass1.c -g -o pass1

pass2:pass2.c
	g++ pass2.c -g -o pass2


	
