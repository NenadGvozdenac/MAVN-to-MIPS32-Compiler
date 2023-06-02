# MAVN to MIPS32 compiler

Here is a demonstration of a simple MAVN to MIPS32 assembly compiler.

The compiler can handle basic instructions that are used in MAVN language.

The compiler consists of a few phases:
  1. Lexical Analysis
  2. Syntax Analysis
  3. Instructions, Variables and Labels gathering
  4. Liveness Analysis
  5. Resource Allocation:
    * Finding an interference graph
    * Simplification of register variables
    * Resource allocation to all register variables
  6. Output to .S file

The output of this simple compiler is a .S file. 

Lexical Analysis: 
  - Reads through the input file of the .mavn file
  - Finds all tokens (words) in the mavn file
  - Stores them in a container

Syntax Analysis:
  - Reads through the container where lexicals are stored
  - Goes token by token, finding if the variables are in order
  - Makes sure the labels & variables are unique

Instructions, Variables and Labels gathering:
  - While doing the Syntax Analysis, it reads token by token
  - If the instruction consists of multiple variables, it stores them when it comes to that variable
  - Instructions are lines of code, until the ; 
  - Labels are specifically read and stored

Liveness Analysis:
  - Phase consists of finding the connections of all register variables
  - Before liveness analysis, DEF and SRC variables are the same, DST exists
  - After liveness analysis, OUT and IN are generated

Resource Allocation:
  - This phase is for painting an intereference graph so it has the few variables used possible
  - I implemented a simple solution which finds an interference graph, goes through the interference matrix and checks for neighbours
  - If number of neighbours is less than the register number, it's okay; else, it spills (error is thrown)
  - Resource allocation is the final step before the writing, so it maps all variables to syntax analysis's variables

Output to .S file:
  - FileWritter is used to generate a stringstream object while is written into a .S file


Project provided by the subject OPPiSA (Osnovi Paralelnog Programiranja i Softverski Alati), RT-RK.
