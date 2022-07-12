/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

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

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 6 "parser.y"

    #include "parser.hh"
    #include <iostream>
    extern int yylex();


#line 56 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    HINT = 259,                    /* HINT  */
    HFLOAT = 260,                  /* HFLOAT  */
    HCHAR = 261,                   /* HCHAR  */
    HSTRING = 262,                 /* HSTRING  */
    SEMI_COL = 263,                /* ";"  */
    TOK_HFLOAT = 264,              /* "hfloat"  */
    TOK_HCHAR = 265,               /* "hchar"  */
    TOK_HSTRING = 266,             /* "hstring"  */
    TOK_HINT = 267,                /* "hint"  */
    TOK_HWHILE = 268,              /* "hwhile"  */
    TOK_HPRINT = 269,              /* "hprint"  */
    TOK_HIF = 270,                 /* "hif"  */
    EQUALS = 271,                  /* "="  */
    PLUS = 272,                    /* "+"  */
    MINUS = 273,                   /* "-"  */
    MULT = 274,                    /* "*"  */
    DIV = 275,                     /* "/"  */
    AND = 276,                     /* "&"  */
    OR = 277,                      /* "|"  */
    LESS_EQUAL = 278,              /* "<="  */
    GREAT_EQUAL = 279,             /* ">="  */
    DOUB_EQUAL = 280,              /* "=="  */
    NOT_EQUAL = 281,               /* "!="  */
    LESS_THAN = 282,               /* "<"  */
    GREATER_THAN = 283,            /* ">"  */
    NOT = 284,                     /* "!"  */
    OPEN_PAREN = 285,              /* "("  */
    CLOSE_PAREN = 286,             /* ")"  */
    OPEN_CURLY = 287,              /* "{"  */
    CLOSE_CURLY = 288,             /* "}"  */
    COMMA = 289                    /* ","  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 13 "parser.y"

    Number *num;

#line 111 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
