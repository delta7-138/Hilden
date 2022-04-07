#ifndef __PARSE_H_
#define __PARSE_H_

#include <lexer/token.h>
#include <lexer/lexer.h>
#include <stack>
#include <queue>

struct ParseTreeNumeric{
	ParseNumericNode node; 
	struct ParseTreeNumeric *left; 
	struct ParseTreeNumeric *mid; 
	struct ParseTreeNumeric *right; 

	int level; 
}

class ParseNumericNode{
	/*
	E = 1 ; T = 2; F = 3;
	E' = -1 ; T = -2; epsilon = 0 
	*/
	int val; 
	std::string name;//only for idenitifers; 
	bool is_terminal; 
	bool is_epsilon; 
public: 
	static std::vector<int> production(int start , int input);
	static ParseTreeNumeric *getNextNode(int start , int input);
}



struct ParseTreeNumeric  *build_parse_tree(std::vector<TokenType>tokenList);
//function to add a node as per production to tree is useful too