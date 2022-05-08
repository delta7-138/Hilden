#ifndef __ENV_H_
#define __ENV_H_

#include "parse/parser.h"
#include "semantics/meta.h"
#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
#include <iterator>

class Environment{
public: 
	AST_Tree_Node *body; 
	std::map<std::string , Variable *> variable_table; //use for lookup 
	std::map<std::string , Function *> function_table; //use for calls; lookup is just complicated

	Environment(AST_Tree_Node *); //constructor 
	//Environment(AST_Tree_Node * , std::vector<Variable *> , std::vector<Function *>); //for blocks
	TypeObject * typecheck_and_eval(AST_Tree_Node *); //takes AST_Tree_Node and evaluates
	TypeObject * eval(); //evaluates body and returns error in case 
	void add_var_list(std::vector<Variable*>); //add variables from outer scope
	void add_func_list(std::vector<Function *>); //outside functions inside and inside too

	void add_var_scope(std::string , std::string); //add variable to current scope
	void add_func_scope(std::string name , std::string); 
}
#endif 