To Build
    "make"
To RUN
	1> Scanner : ./scanner <test.txt
	[if we want to compile test.txt]

	2> Parsing : ./llparser

	[Output will be displayed on Console]

Format of Grammar.txt
As attached ,,, fist all the productions are written like ,,
E->TR  --->  E T R
.......
......
PRODEND #end of productions
Non Terminal List
Terminal List
Start Symol

The same order of productions will be used in fst-fol.txt
The same order of Terminals will be used in fst-fol.txt

Format of Fst-Fol.txt

At fist Fist info for all the productions in the same order as they appeared in the Grammar.txt file

after FIRSTPRODEND

fisrt for each of the non terminals in the same order

after FIRSTEND

follow for each of the non terminals in the same order

end at FOLLOWEND