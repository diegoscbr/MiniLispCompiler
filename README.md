# MiniLispCompiler
A miniLisp interpreter

This C++ program serves as an interpreter for a simple Lisp-like programming language. It provides functionality for defining functions, evaluating expressions, and interacting with the user through a read-eval-print loop. 
namespace std`: to simplify the use of standard namespace.

## Declarations
- Type definitions for `NUMBER` and `NAME`.
- Constants for the maximum length of names, maximum number of different names, and maximum input length.
- Prompt messages defined as `PROMPT` and `PROMPT2`.
- A comment character defined as `COMMENTCHAR`.
- ASCII code for tab character defined as `TABCODE`.
- Definitions of various data structures and enums.

## Data Structure Operations

Several functions are defined to create and manipulate data structures used in the interpreter:

- `mkVALEXP`: Create an expression of type VALEXP with a given number.
- `mkVAREXP`: Create an expression of type VAREXP with a given variable name.
- `mkAPEXP`: Create an expression of type APEXP with an operator and arguments.
- `mkExplist`: Create an expression list with a head expression and a tail expression list.
- `mkNamelist`: Create a name list with a head name and a tail name list.
- `mkValuelist`: Create a value list with a head value and a tail value list.
- `mkEnv`: Create an environment with variables and corresponding values.
- `lengthVL`: Return the length of a value list.
- `lengthNL`: Return the length of a name list.

## Name Management

Functions for managing and working with names include:

- `fetchFun`: Get the function definition of a given name from the list of function definitions.
- `newFunDef`: Add a new function definition with a name, formal parameters, and a body.
- `initNames`: Initialize pre-defined names.
- `install`: Insert a new name into the list of print names.
- `prName`: Print the name corresponding to a given name index.

## Input

Functions related to input processing and parsing include:

- `isDelim`: Check if a character is a delimiter.
- `skipblanks`: Find the next non-blank position in the input.
- `matches`: Check if a string matches the input at a specific position.
- `nextchar`: Read the next character from input, filtering tabs and comments.
- `readParens`: Read characters, ignoring newlines, until a matching ')' is found.
- `readInput`: Read characters into the `userinput` array.
- `reader`: Read input into `userinput`, ensuring it's not blank.
- `parseName`: Parse and return a name from the input.
- `isDigits`: Check if a sequence of digits begins at a position.
- `isNumber`: Check if a number begins at a position.
- `parseVal`: Parse and return a number from the input.
- `parseExp`: Parse and return an expression from the input.
- `parseEL`: Parse and return an expression list.
- `parseNL`: Parse and return a name list.
- `parseDef`: Parse and return a function definition.

## Environments

Functions for managing environments, variables, and values include:

- `emptyEnv`: Return an environment with no bindings.
- `bindVar`: Bind a variable to a value in an environment.
- `findVar`: Look up a name in an environment and return its value.
- `assign`: Assign a value to a variable in an environment.
- `fetch`: Return the value bound to a name in an environment.
- `isBound`: Check if a name is bound in an environment.

## Numbers

Functions related to numbers, value operations, and control operators include:

- `prValue`: Print a number.
- `isTrueVal`: Check if a number is a true (non-zero) value.
- `arity`: Return the number of arguments expected by an operator.
- `applyValueOp`: Apply a value operator to arguments in a value list.

## Evaluation

The core evaluation function `eval` is defined to evaluate expressions in a given environment. It handles value expressions, variable expressions, and applied expressions with various operators. Depending on the expression type, it either returns the value or performs a corresponding operation.

## Read-Eval-Print Loop

The `main` function serves as the entry point for the interpreter. It initializes pre-defined names, sets up a global environment, and enters a read-eval-print loop. Within the loop, the program reads user input, checks for exit commands, defines functions, or evaluates expressions based on user input.

Overall, this program provides a basic Lisp-like interpreter with support for defining functions, variables, and evaluating expressions in a simple programming language.
