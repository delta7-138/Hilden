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
#include "../lex/lexer.h"
#include "../lex/token.h"

#define EQ_PREC 1
#define ADD_PREC 2
#define MUL_PREC 3

enum AST_Node_Type {
	n_binary = 0, // Default, binary statements
	n_unary = 1, // Unary statements, return, break etc
	n_ternary = 2, // Ternery statements, conditionals, ternery operator
	n_multi = 3
}; 

class AST_Tree_Node
{
public:
	TokenType* tok; 
	TokenType* eval_tok;
	int node_type;
	std::vector<AST_Tree_Node *>childList;
 	AST_Tree_Node(TokenType tok);
 	AST_Tree_Node(TokenType tok, int node_type);
	AST_Tree_Node();
	int add_child(AST_Tree_Node* node);
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
AST_Tree_Node *parse_expression(AST_Tree_Node * , int);
AST_Tree_Node* parse_binary();
void init_variable(TokenType , std::string val); 
AST_Tree_Node *init_parse(std::vector<TokenType>tokenList);
void print_ast(AST_Tree_Node* node, int);

#endif