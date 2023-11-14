# Tiny Language Compiler

This is a minimalistic compiler for the TINY language, focusing on the scanner part implemented in C++. The scanner is responsible for lexical analysis, breaking down the TINY program into tokens. Below is a guide on how to use the scanner along with the sample TINY program.

## Getting Started

1. **Installation:**
   - Clone this repository to your local machine.

2. **Usage:**
   - Open a terminal and navigate to the compiler directory.
   - Compile the C++ scanner file:

     ```bash
     g++ scanner.cpp -o scanner
     ```

   - Run the scanner with the TINY program file as an argument:

     ```bash
     ./scanner 
     ```

     Replace the text inside `input.txt` with your TINY program.

3. **Output:**
   - The scanner will print the identified tokens in the following format:

     ```txt
     [Line Number] [Token String] (Token Type)
     ```

## TINY Language Scanner

The scanner, implemented in C++, recognizes the following token types:

- **Keywords:**
  - `IF`, `THEN`, `ELSE`, `END`, `REPEAT`, `UNTIL`, `READ`, `WRITE`

- **Operators:**
  - `:=` (ASSIGN), `+` (PLUS), `-` (MINUS), `*` (TIMES), `/` (DIVIDE), `^` (POWER)

- **Relational Operators:**
  - `<` (LESS_THAN), `=` (EQUAL)

- **Special Symbols:**
  - `;` (SEMI_COLON), `(` (LEFT_PAREN), `)` (RIGHT_PAREN), `{` (LEFT_BRACE), `}` (RIGHT_BRACE)

- **Identifiers:**
  - Alphabetic characters (a:z or A:Z) and underscores (`ID`).

- **Integers:**
  - Sequences of digits (`NUM`).

- **End of File:**
  - `ENDFILE`

- **Error:**
  - `ERROR`

## Sample TINY Program

```txt
{ Sample program
  in TINY language
  compute factorial
}

read x; {input an integer}
if 0<x then {compute only if x>=1}
  fact:=1;
  repeat
    fact := fact * x;
    x:=x-1
  until x=0;
  write fact {output factorial}
end
```
