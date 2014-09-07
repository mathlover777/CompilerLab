README

to compile : 	make
to run     : 	"./macroASM"  <default parameters>
				"./macroASM input.s" <input = "input.s"
				output = "output.out"(default)>
				"./macroASM input.s output.out"
				input = "input.s"
				output = "output.out"


Assumptions:

There shouldn't be any space in the argument list
&a, &b - will not work
&a,&b - will work

*********NOTE : in the test files given in question last line is END FIRST but FIRST Label is never mentioned ,, so kindly make the required changes while testing ,, the input files are also given in test1.s and test2.s and their corresponding expand files and intermediate files are also maintained.

its case sensitive

if the same macro (may be different in inside) defined more than once
the last one will be considered!!



SOURAV SARKAR
11CS30037
SHRIHARI A BHAT
11CS30008