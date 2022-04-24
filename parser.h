#ifndef __PARSE_H_
#define __PARSE_H_

#include "token.h"
#include "lexer.h"
#include <stack>
#include <queue>
class ParseNumericNode{
	/*
	E = 1 ; T = 2; F = 3;
	E' = -1 ; T = -2; epsilon = 0 
	*/
	int val; 
	std::string name;//only for idenitifers; 
	bool is_terminal; 
	bool is_epsilon; 

};

class AST_Binary_node{
	char op; //operator node 
	AST_Binary_node *left; 
	AST_Binary_node *right; 
	float value; 
};

std::vector<int> production(int start , int input);

float build_parse_tree(std::vector<TokenType>tokenList);
//function to add a node as per production to tree is useful too

#endif