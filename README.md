For lexing:

from the root directory run the following commands

lex lex/lexer.l
g++ lex.yy.c lex/scanner.cpp main.cpp
./a.out <a .hl file>

it should print out the correct token list

