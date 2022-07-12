%{
    #include "parser.tab.h"
    // any definitions go here

%}

%%

"hfloat"  return TOK_HFLOAT;//keywords
"hchar"  return TOK_HCHAR;//keywords
"hint"  return TOK_HINT;//keywords
"hstring" return TOK_HSTRING; // keyword
"hwhile" return TOK_HWHILE; // keyword
"hprint" return TOK_HPRINT; // keyword
"hif" return TOK_HIF; //keyword

"+" return PLUS;//operator
"-" return MINUS;//operator
"*" return MULT;//operator
"/" return DIV;//operator
"=" return EQUALS;//operator
"!" return NOT;//operator
"<" return LESS_THAN;//operator
">" return GREATER_THAN;//operator
"<=" return LESS_EQUAL; // operator
">=" return GREAT_EQUAL; // operator
"==" return DOUB_EQUAL; // operator
"!=" return NOT_EQUAL; // operator
";" return SEMI_COL; // ends
"," return COMMA;
"(" return OPEN_PAREN;
")" return CLOSE_PAREN;
"{" return OPEN_CURLY;
"}" return CLOSE_CURLY;
"\n" ;
" " ;

[0-9]* { 
    yylval.num = new Number(atoi(yytext));
    return HINT;
}//ints
[0-9]*"."[0-9]* { 
    yylval.num = new Number((float)atof(yytext));
    return HFLOAT;
}//floats
"'"."'"   {
        yylval.num = new Number(yytext[0]);
        return HCHAR;
    }//chars

\"(\\.|[^"\\])*\"    {
    yylval.num = new Number(std::string(yytext));
    return HSTRING;
} //string

[a-zA-Z_][a-zA-Z0-9_]* return IDENTIFIER;

. ;

%%

int yywrap() {
    return 0;
}
