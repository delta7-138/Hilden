/* Parser for calc++.   -*- C++ -*-

   Copyright (C) 2005-2015, 2018-2021 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.2"
%header


%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  #include "AST.hh"
  class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
#include "driver.hh"
}

%token IDENTIFIER
%token 
    <LiteralAST*> VAL_HINT
    <LiteralAST*> VAL_HFLOAT
    <LiteralAST*> VAL_HCHAR
    <LiteralAST*> VAL_HSTRING

%token
    KEY_HINT "hint"
    KEY_HFLOAT "hfloat"
    KEY_HCHAR "hchar"
    KEY_HSTRING "hstring"
    KEY_HIF "hif"
    KEY_HELSE "helse"

%token
    OP_SEMI_COL ";"
    OP_EQUALS "="
    OP_PLUS "+"
    OP_MINUS "-"
    OP_MULT "*"
    OP_DIV "/"
    OP_AND "&"
    OP_OR "|"
    OP_LESS_EQUAL "<="
    OP_GREAT_EQUAL ">="
    OP_DOUB_EQUAL "=="
    OP_NOT_EQUAL "!="
    OP_LESS_THAN "<"
    OP_GREATER_THAN ">"
    OP_NOT "!"
    OP_OPEN_PAREN "(" 
    OP_CLOSE_PAREN ")"
    OP_OPEN_CURLY "{"
    OP_CLOSE_CURLY "}"
    OP_COMMA ","


%%
input: 
| stmt ";" input
| if input
;

if: "hif" expr "{" input "}" "helse" "{" input "}"
| "hif" expr "{" input "}"
;

stmt: dec | def | expr;

dec: type ident;

def: dec "=" expr 
| ident "=" expr 
;

expr: expr "&" log
| expr "|" log
| log 
;

log: log "<=" sum
| log ">=" sum
| log "==" sum
| log "!=" sum
| sum
;

sum: sum "+" term
| sum "-" term
| term
;

term: term "*" factor
| term "/" factor
| factor
;

factor: "(" expr ")"
| ident
| fcall
| lit
;

fcall: ident "(" idarg ")";

idarg: expr ',' idarg | expr;

ident: IDENTIFIER{printf("identifier\n");};

lit: VAL_HINT {printf("HINT LITERAL: %d\n", $1->get_value()->get_int());}
| VAL_HFLOAT {printf("FLOAT LITERAL: %f\n", $1->get_value()->get_float());}
| VAL_HCHAR { printf("HCHAR LITERAL: %c\n", $1->get_value()->get_char()); }
| VAL_HSTRING { std::cout << $1->get_value()->get_string() << std::endl; }
;

type: "hfloat" { printf("hfloat\n"); }
| "hchar" { printf("hchar\n"); }
| "hstring" { printf("hstring\n"); }
| "hint" { printf("hint\n"); }
;
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
