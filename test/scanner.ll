/* Scanner for calc++.   -*- C++ -*-

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

%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hh"
# include "parser.hh"
%}

%{
#if defined __clang__
# define CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
#endif

// Clang and ICC like to pretend they are GCC.
#if defined __GNUC__ && !defined __clang__ && !defined __ICC
# define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#endif

// Pacify warnings in yy_init_buffer (observed with Flex 2.6.4)
// and GCC 6.4.0, 7.3.0 with -O3.
#if defined GCC_VERSION && 600 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wnull-dereference"
#endif

// This example uses Flex's C back end, yet compiles it as C++.
// So expect warnings about C style casts and NULL.
#if defined CLANG_VERSION && 500 <= CLANG_VERSION
# pragma clang diagnostic ignored "-Wold-style-cast"
# pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#elif defined GCC_VERSION && 407 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wold-style-cast"
# pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#define FLEX_VERSION (YY_FLEX_MAJOR_VERSION * 100 + YY_FLEX_MINOR_VERSION)

// Old versions of Flex (2.5.35) generate an incomplete documentation comment.
//
//  In file included from src/scan-code-c.c:3:
//  src/scan-code.c:2198:21: error: empty paragraph passed to '@param' command
//        [-Werror,-Wdocumentation]
//   * @param line_number
//     ~~~~~~~~~~~~~~~~~^
//  1 error generated.
#if FLEX_VERSION < 206 && defined CLANG_VERSION
# pragma clang diagnostic ignored "-Wdocumentation"
#endif

// Old versions of Flex (2.5.35) use 'register'.  Warnings introduced in
// GCC 7 and Clang 6.
#if FLEX_VERSION < 206
# if defined CLANG_VERSION && 600 <= CLANG_VERSION
#  pragma clang diagnostic ignored "-Wdeprecated-register"
# elif defined GCC_VERSION && 700 <= GCC_VERSION
#  pragma GCC diagnostic ignored "-Wregister"
# endif
#endif

#if FLEX_VERSION < 206
# if defined CLANG_VERSION
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wdocumentation"
#  pragma clang diagnostic ignored "-Wshorten-64-to-32"
#  pragma clang diagnostic ignored "-Wsign-conversion"
# elif defined GCC_VERSION
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wsign-conversion"
# endif
#endif

// Flex 2.6.4, GCC 9
// warning: useless cast to type 'int' [-Wuseless-cast]
// 1361 |   YY_CURRENT_BUFFER_LVALUE->yy_buf_size = (int) (new_size - 2);
//      |                                                 ^
#if defined GCC_VERSION && 900 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
%}

%option noyywrap nounput noinput batch debug

%{
  // A number symbol corresponding to the value in S.
%}

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
float [0-9]+"."[0-9]+
char "'"."'"
string \"(\\.|[^"\\])*\"
ident [a-zA-Z_][a-zA-Z0-9_]*
blank [ \t\r]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}

{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();

"hfloat" return yy::parser::make_KEY_HFLOAT(loc);
"hchar" return yy::parser::make_KEY_HCHAR(loc);
"hint" return yy::parser::make_KEY_HINT(loc);
"hstring" return yy::parser::make_KEY_HSTRING(loc);
"hif" return yy::parser::make_KEY_HIF(loc);
"helse" return yy::parser::make_KEY_HELSE(loc);

"+" return yy::parser::make_OP_PLUS(loc);//operator
"-" return yy::parser::make_OP_MINUS(loc);//operator
"*" return yy::parser::make_OP_MULT(loc);//operator
"/" return yy::parser::make_OP_DIV(loc);//operator
"=" return yy::parser::make_OP_EQUALS(loc);//operator
"!" return yy::parser::make_OP_NOT(loc);//operator
"<" return yy::parser::make_OP_LESS_THAN(loc);//operator
">" return yy::parser::make_OP_GREATER_THAN(loc);//operator
"<=" return yy::parser::make_OP_LESS_EQUAL(loc); // operator
">=" return yy::parser::make_OP_GREAT_EQUAL(loc); // operator
"==" return yy::parser::make_OP_DOUB_EQUAL(loc); // operator
"!=" return yy::parser::make_OP_NOT_EQUAL(loc); // operator
";" return yy::parser::make_OP_SEMI_COL(loc); // ends
"," return yy::parser::make_OP_COMMA(loc);
"(" return yy::parser::make_OP_OPEN_PAREN(loc);
")" return yy::parser::make_OP_CLOSE_PAREN(loc);
"{" return yy::parser::make_OP_OPEN_CURLY(loc);
"}" return yy::parser::make_OP_CLOSE_CURLY(loc);

{int} return yy::parser::make_VAL_HINT(new LiteralAST(std::make_unique<Number>(std::stoi(yytext))), loc);
{float} return yy::parser::make_VAL_HFLOAT(new LiteralAST(std::make_unique<Number> (std::stof(yytext))), loc);
{char} return yy::parser::make_VAL_HCHAR(new LiteralAST(std::make_unique<Number> (yytext[1])), loc);
{string} return yy::parser::make_VAL_HSTRING(new LiteralAST(std::make_unique<Number> (yytext)), loc);

{ident} return yy::parser::make_IDENTIFIER(loc);


<<EOF>>    return yy::parser::make_YYEOF (loc);
%%
// {int}      return make_NUMBER (yytext, loc);
// {id}       return yy::parser::make_IDENTIFIER (yytext, loc);
// .          {
//              throw yy::parser::syntax_error
//                (loc, "invalid character: " + std::string(yytext));
// }


void
driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void
driver::scan_end ()
{
  fclose (yyin);
}
