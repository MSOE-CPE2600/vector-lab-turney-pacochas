# Mini MATLAB - Vector Calculator

A simplified vector calculator that is used from the command line, written in C.
This program allows the user to  create, manipulate, save, and load 3D vectors in similar style to MATLAB syntax. The vector operations this program supports is addition, subtraction, scalar multiplication, dot product, and cross product.

## Version 1.0 Features

- Create and store up to 10 named vectors into an array (ex. a, b, c, d).
- Perform common vector operations:
- 'A = 1 2 3' -> Create a vector
- 'c = a + b', 'c = a - b', ' c = 2 * a' -> arithmetic
- 'c = a x b' -> cross product
- 'c = a . b' -> dot product
- list vectors
- clear the list of vectors
- input validation with helpful error messages

## Version 2.0 Updated Features

- Create and store unlimited vectors
- Load vectors from a .csv file
- Save vectors to a .csv file
- Handle File IO errors such as loading a file that does not exist
- clean memory management - no leaks

## Files

- mini_matlab.c # Main program (user input, parsing, logic)
- vector.c # Vector operations (math, load/save, memory)
- vector.h # Header for Vector struct and function prototypes
- README.md # Project documentation
- Makefile # makefile of the program to cleanly build the files

## Compilation

compile with 'gcc' (requires 'math.h')
BASH: gcc -o mini_matlab mini_matlab.c vector.c -lm

## Using Makefile

BASH:
make clean
make

## Executable

./lab5

## Author

Sebastian Pacocha
