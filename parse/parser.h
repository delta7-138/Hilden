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
#define LOG_PREC 0

enum AST_Node_Type {
	n_binary = 0, // Default, binary statements
	n_unary = 1, // Unary statements, return, break etc
	n_ternary = 2, // Ternery statements, conditionals, ternery operator
	n_multi = 3, 
	n_empty = -1
}; 

class AST_Tree_Node
{
public:
	TokenType* tok; 
	TokenType* eval_tok;
	std::string node_type;
	std::vector<AST_Tree_Node *>childList;
 	AST_Tree_Node(TokenType tok);
 	AST_Tree_Node(TokenType tok, std::string node_type);
	AST_Tree_Node();
	int add_child(AST_Tree_Node* node);
};

class Parser{
public: 
	std::queue<TokenType>fresh_tokens; // To store current tokens to get parsed
	std::stack<TokenType>parsed_tokens; //To store the already parsed tokens in case we need them
	AST_Tree_Node *root; //root of the AST

	Parser(std::vector<TokenType> , std::string, int check); 

	//primary functions
	TokenType get_next_tok(); //read queue front dequeue and push into parsed_tokens stack
	TokenType get_cur_tok(); //read queue front and return do nothing else
	void parse(); //recursively parse and return head

	//Accessory functions
	int get_precedence(std::string); 
	AST_Tree_Node *parse_function(std::vector<TokenType>tList);
	AST_Tree_Node *parse_binary(std::vector<TokenType>); 
	std::vector<TokenType>dequeue_and_return(std::string , std::string , bool); 
	void print_ast(AST_Tree_Node* node, int depth); 
	void init_check();
	void check_paranthesis(int, int);
}; 


#endif