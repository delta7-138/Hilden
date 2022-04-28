#ifndef __OP_PARSE_H_
#define __OP_PARSE_H_
#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
#include <iterator>
#include "lexer.h"
#include "token.h"

#define EQ_PREC 1
#define ADD_PREC 2
#define MUL_PREC 3
class AST_Tree_Node{
	TokenType tok; 
	std::vector<AST_Tree_Node *>childList;
}; 

class Variable{
public: 
	std::string val; 
	std::string type;
	std::string name; 
}; 

class Environment{
public: 
	int level; 
	std::map<std::string , Variable *>var_map; 
};

class Function{
public: 
	std::vector<TokenType>prototype;
	std::vector<TokenType>body; 
	std::string return_type; 
}; 



int get_precedence(std::string);
float parse_primary(std::vector<TokenType>);
void parse_expression(std::vector<TokenType>);
float parse_binary(std::vector<TokenType>tList);
void init_variable(TokenType , std::string val); 

#endif