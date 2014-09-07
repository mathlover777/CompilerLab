 ____  _____    _    ____  __  __ _____ 
 |  _ \| ____|  / \  |  _ \|  \/  | ____|
 | |_) |  _|   / _ \ | | | | |\/| |  _|  
 |  _ <| |___ / ___ \| |_| | |  | | |___ 
 |_| \_\_____/_/   \_\____/|_|  |_|_____|
                                         

 Assignment 9:
 Group 18
 Sourav Sarkar
 11CS30037
 Shrihari A Bhat
 11CS30008

souravmathlover@gmail.com

 To Build:
 "make"
 To Run:
 "./tac <[inputfile]"

 output will be generated in the file "output.txt"

 NOTES:
 	>I have done explicit type checking in expression blocks for int-float clashes only [as mentioned in assignment]
 	Other non-trivial type clashes will give error [also float can hold an int by explicit typeconversion
 	but the reverse will give error]
 	
 	I have made some changes in the given grammer [functionality is all the same]:
 	1>
 	stmt :	
	assignmentStmt SEMICOLON
	|readStmt SEMICOLON
	|printStmt SEMICOLON

	stmtList:
		stmtList M1 stmt
		|M2 stmt
	These are some semicolon issues. Kindly note the  given input sets will have a slight syntax error in my parser due to semicolon at the end of the declararion

	GIVEN VERSION : 
	def
		a:int;
		b:int;
		sum:float;
		xpos:float;
		ypos:float;
		temp: float     [notice no semicolon so will give syntax error]
	end
	However,
	def
		a:int;
		b:int;
		sum:float;
		xpos:float;
		ypos:float;
		temp: float;     [will work perfectly]
	end
	2> I have introduced proper Markers for inheritted attributes
	3> 	bExp ---> LEFT_PAREN M16 bExp RIGHT_PAREN was giving conflicts. So parenthesis in bExps have to in {}.This is not part of the grammer ,but I have introduced them to save some coding
	So The Modified Rule : 
	bExp --> LEFT_BRACES M16 bExp RIGHT_BRACES