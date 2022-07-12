#! /usr/bin/bash

bison -d -t parser.y && flex lexer.l && g++ -g parser.tab.c lex.yy.c main.c parser.cc
