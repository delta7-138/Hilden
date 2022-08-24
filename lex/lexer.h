
#ifndef __LEXER_H_
#define __LEXER_H_
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "token.h"
/* We have the following token classes
    1. Separator(;)
    2. Keyword (Float)
    3. OpenParan
    4. CloseParan
    5. Identifier (has to start with alphabet followed by alphanumeric characters)
    6. String Literal (has to be enclosed in double quotes)
    7. Character Literal (has to be enclosed in single quotes)
    8. Numeric Literal (has only decimal and numbers)
    9. Operators (+ - * / =)
*/
extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE* yyin;

class TokenType{
    public: 
    int token_number; 
    std::string token_val; 

    void print(){
        std::cout<<"<"<<token_number<<" : "<<token_val<<">"<<std::endl; 
    }
}; 

class Lexer{
public: 
    std::string source; 
    std::vector<TokenType> tokenList;
    std::vector<TokenType> gettokenlist(char *); 

    //functions to validate and find token class implements NFA's
    // bool isDigit(char);
    // bool isAlphabet(char);
    // bool isAlphanumeric(char);
    // bool isSeparator(std::string);
    // bool isKeyword(std::string);
    // bool isOpenParan(std::string);
    // bool isCloseParan(std::string);
    // bool isOperator(std::string);
    // bool isStringLiteral(std::string);
    // bool isCharLiteral(std::string);
    // bool isNumeric(std::string);
    // bool isIdentifier(std::string);
    // bool isIntegerLiteral(std::string);
    // bool isFloatingLiteral(std::string);
    // TokenType getNextToken(std::string);
    void print_token_list(); 
}; 
#endif