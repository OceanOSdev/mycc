# Com S 440 Compiler Project

## How to build
To build the project and documentation, simply run `make`. 
For speedy build times, run `make -jN` where N is some number.
To remove generated files, run `make clean`.
For convenience, to build just the latex documentation, you can run `make docs`.

## Part 0 Features
- [x] README entry for part 0
- [x] Switch handling in `mycc`
- [x] Updated LaTeX doc

## Part 1 Features
- [x] Can read and print tokens in files
- [x] Now documentation gets built when running `make`
- [x] Updated LaTeX doc

## Part 2 Features
- [x] Can now parse:
    - Global Variables
    - Function Prototypes / Parameter Lists
    - Function Local Variables and Body
    - For, While, Do Loops
    - If Then Else
    - Break / Continue / Return / Expression Stmts
    - Expressions with Unary / Binary/ Ternary Operators
    - Assignment Operators; Increment and Decrement
    - Identifiers and Arrays
    - Function Calls and Parameters
- [x] Extra credit functionality:
    - Variable Initialization
    - Constants
    - User-Defined Structs
    - Struct Member Selection 
- [x] Updated LaTeX doc

## Part 3 Features
- [x] Can now perform type checking on all features listed above in Part 2
- [ ] Updated LaTeX doc