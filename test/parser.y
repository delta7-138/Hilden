%{
    /* definitions go here */
    int yyerror(char* s);
%}

%code requires {
    #include "parser.hh"
    #include <iostream>
    extern int yylex();

}

%union {
    Number *num;
}


// %code {
//     #include "scanner.hh"
// }


// %define api.value.type variant
// %define api.token.constructor
// %define parse.assert

%token IDENTIFIER 
%token <num> HINT
%token <num> HFLOAT
%token <num> HCHAR
%token <num> HSTRING
%token 
    SEMI_COL ";"
    TOK_HFLOAT "hfloat"
    TOK_HCHAR "hchar"
    TOK_HSTRING "hstring"
    TOK_HINT "hint"
    TOK_HWHILE "hwhile"
    TOK_HPRINT "hprint"
    TOK_HIF "hif"


%token 
    EQUALS "="
    PLUS "+"
    MINUS "-"
    MULT "*"
    DIV "/"
    AND "&"
    OR "|"
    LESS_EQUAL "<="
    GREAT_EQUAL ">="
    DOUB_EQUAL "=="
    NOT_EQUAL "!="
    LESS_THAN "<"
    GREATER_THAN ">"
    NOT "!"

%token 
    OPEN_PAREN "(" 
    CLOSE_PAREN ")"
    OPEN_CURLY "{"
    CLOSE_CURLY "}"
%token COMMA ","


%%
input: 
|stmt ";" input
// |"{" input "}" input
// |fun_def input
// |if input
// |while input
;

// fun_def: type ident "(" arg ")" "{" input "}"
// ;

// arg: type ident "," arg | type ident
// ;

// if: "hif" expr "{" input "}" "helse" "{" input "}"
// | "hif" expr "{" input "}"
// ;

stmt: dec | def | expr;

// while: "hwhile" expr "{" input "}";

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

lit: HINT {printf("HINT LITERAL: %d\n", $1->get_int());}
| HFLOAT { printf("HFLOAT LITERAL: %f\n", $1->get_float()); }
| HCHAR { printf("HCHAR LITERAL: %c\n", $1->get_char()); }
| HSTRING { std::cout << $1->get_string() << std::endl; }
;

type: "hfloat" { printf("hfloat\n"); }
| "hchar" { printf("hchar\n"); }
| "hstring" { printf("hstring\n"); }
| "hint" { printf("hint\n"); }
;

%%

int yyerror(char* s) {
    printf("ERROR: %s\n", s);
    return 0;
}
