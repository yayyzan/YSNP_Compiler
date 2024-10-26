# RISCV C Compiler 2023
*This is our log of time-tracking/project management*
## Overview
*This is an outline of our general plan along with a provisional timeline*

1. Understand the content covered in lectures and labs ~~**_by 25/02/2023_**~~
2. Research and evaluate tools available to us ~~**_by 26/02/2023_**~~
3. Decide on a structure for the compiler (including which tools to use) ~~**_by 27/02/2023_**~~
4. Implement and test the basic features: ~~**_by 6/03/2023_**~~ Very optimistic actually ~~**_by 10/03/2023_**~~
   * a file containing just a single function with no arguments ~~**_by 5/03/2023_**~~
   * variables of `int` type ~~**_by 5/03/2023_**~~
   * local variables ~~**_by 5/03/2023_**~~
   * arithmetic and logical expressions ~~**_by 6/03/2023_**~~
   * if-then-else statements ~~**_by 9/03/2023_**~~
   * while loops ~~**_by 8/03/2023_**~~
5. Implement and test intermediate features: ~~**_by 13/03/2023_**~~:super optimistic ~~**_by 23/03/2023_**~~
    * files containing multiple functions that call each other ~~**_by 15/03/2023_**~~
    * functions that take up to four parameters ~~**_by 14/03/2023_**~~
    * for loops ~~**_by 14/03/2023_**~~
    * arrays declared globally (i.e. outside of any function in your file)~~**_by 20/03/2023_**~~
    * arrays declared locally (i.e. inside a function)~~**_by 20/03/2023_**~~
    * reading and writing elements of an array~~**_by 20/03/2023_**~~
    * recursive function calls~~**_by 22/03/2023_**~~
    * the `enum` keyword~~**_by 23/03/2023_**~~
    * `switch` statements~~**_by 23/03/2023_**~~
    * the `break` and `continue` keywords~~**_by 22/03/2023_**~~
6. Implement and test advanced features: **_by 20/03/2023_**
    * variables of `double`, `float`, `char`, `unsigned`, structs, and pointer types
   * calling externally-defined functions (i.e. the file being compiled declares a function, but its definition is provided in a different file that is linked in later on)
   * functions that take more than four parameters ~~**_by 14/03/2023_**~~
   * mutually recursive function calls
   * locally scoped variable declarations (e.g. a variable that is declared inside the body of a while loop, such as `while(...) { int x = ...; ... }`. ~~**_by 6/03/2023_**~~
   * the `typedef` keyword
   * the `sizeof(...)` function (which takes either a type or a variable) ~~**_by 24/03/2023_**~~
   * taking the address of a variable using the `&` operator
   * dereferencing a pointer-variable using the `*` operator
   * pointer arithmetic
   * character literals, including escape sequences like `\n`
   * strings (as NULL-terminated character arrays)
   * declaration and use of structs
7. Testing and evaluation of our overall implementation **_by 24/03/2023_**
# Deciding YACC vs ANTLR
YACC    |     ANTLR
:----   |    :----
Familiarity (covered in labs)    |    New concept
Left Recursive (can use pre-made `ANSI C90 grammar`)    |     Right recursive (need to rewrite grammar or at least adapt example of `C11 grammar` which is right recursive
Table driven parser (typically slower)     |    Recursive descent parser (typically faster)
Need to write classes    |    ANTLR writes the all of the classes
No GUI IDE to visualize/debug    |    ANTLRworks GUI draws grammar rule tree



# Meetings
Date | Objective | Achieved |TBC| Meeting num
:--- |:----------|:---------|:---|:------
25/02/2023| Discuss YACC vs ANTLR| Compared the relative merit and drawback of each method|Make a final decision|1
2/03/2023 | Discuss and understand the grammar, ast classes and structure required to parse basic features| Made sure that both members of the team are clear on a common ast structure and what is to be implemented| Implement the discussed material|2
3/03/2023 | Discuss what has been implemented in terms of ast and parser| Understood what has been implemented and decided what is the next step| Symbol table/context|3
3/03/2023 | Discuss context function: how to keep track of registers, possible structures for symbol tables (hierarchy of tables, stack of tables, array of structs etc)| Implemented register tracker as bitset (see notes)| Symbol table structure|4
5/03/2023 | Decide on symbol table structure and implement| Decided and implemented stack of symbol table structures| Maybe modify to a vector that is implemented as a stack-like structure for ease of access| 5
10/03/2023| Progress meeting + implement and test basic section| All basic features implemented and tested see notes|Plan and implement advanced features|6
11/03/2023| Implement function calls and functions with multiple parameters| Function calls and multiple parameters achieved as well as more than four| Implement arrays and look at storing offset from frame pointer instead of stack pointer|7
14/03/2023| Implement for loops| For loops succesfully implemented|Implement multiple function calls to each other|8
15/03/2023|Implement multiple function calls to each other| Implemented multiple function calls to each other|GLOBALS|9
17/03/2023|Discuss a structure for GLOBALs| Decided and started implementation|Implement and test globals|10
19/03/2023|Implement GLOBAL declarations| Global declarations implemented for non array types|ARRAYS|11
20/03/2023|Implement arrays (read/write)| Arrays implemented both global and local|recursion next|12
21/03/2023|Discuss recursive function calls| Started implementation of recursive function calls (saved registers???)| Finish recursion|13
22/03/2023|Finish recursion and start break/continue| break/continue implemented as well as recursion works|ENUM and SWITCH|14
23/03/2023|Implement ENUM and SWITCH (possibly start some types if time allows)| ENUM and SWITCH finished |but need to do types|15
24/03/2023|Implement UNSIGNED, CHAR and POINTERs + Sizeof anything else easy|DONE|SUBMIT|16





# Meeting notes
Often used teams for meetings and co-development environments like replit/vs code liveshare to work together on the code as we went along (consistency in code) and reduce chances of git conflicts.
Meeting num: notes
1. Used sections of premade ANSI C90 grammar to lex and parse basic C code. Implemented AST and objects to lex and parse the "basic" features.

4. Discussed and decided upon a structure for keeping track of register allocation- using a bitset where 0 = empty reg, 1 = occupied reg.
Implemented this with `set` and `check` member functions as part of a `registers` struct.
Also discussed the use of a symbol table to keep track of important information about each symbol in the program (variables, functions... -scope, registers etc). However we need to consider how these will need to behave as the program becomes more complex with nested functions and such.
5. Symbol table implemented as a stack (push a new table to stack everytime a new scope is entered and pop when exited)
6. Implemented all basic functions - codegen done in ast files, classes inherit from node base class, context contains symbol table definitions, also inadvertently implemented {} counting as local scope due to method of entering/exiting new scope everytime curly braces encountered
7. Implemented functions of multiple parameters and function calls (added new register tracking bitset to allocate function registers as per RISCV documentation x10-x17)
8. For loops implemented with three/four child nodes for (declaration), condition, statement and process. Codegen using BNE
9. Parameters are communicated through function argument registers a0-a7 if parameter size is greater than 8 arguments are stored in the stack of the 'caller' function and the 'callee' will access the arguments through a reference which is above the frame pointer. This also means that the current stack offset of the 'caller' function will be influenced by the number of arguments.
10. Globals are stored on the heap and not on local stack frame.
12. Since arrays can have variable indexing the actual offset should be calculated through code generation and not an interpreter. However, knowing that an array cannot be initialised with a variable, an interpreter is used to calculate the size.
13. In the case a function is called inside an expression on the RHS the LHS of the expression needs to be stored in a saved register in order for its value not to be lost.
14. By adding labels to for, while and switch statements and storing them in a vector in context break/continue isn't too hard.
15. Enums are ints;) done in context
16. Implementing new types proved to be tedious and time consuming, only implemented unsigned and dodgy char implemented. Size of ran a check on type and would return size of that type. Pointers involved making classes for dereferencing and assigning address which respectively dereference and address a pointer into a destination register.
