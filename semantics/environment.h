#ifndef __ENV_H_
#define __ENV_H_
#define void_type 37

#include "../parse/parser.h"

#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
#include <iterator>

class TypeObject{
	public: 
	std::string val; 
	int type; //"error" for error

	TypeObject(int , std::string); 
};



class Variable{
	public: 
	std::string name; 
	int type; 
	std::string val; //on declaration memory will also be allocated
	int env_level; //env level 

	Variable(std::string , int , int); 
}; 

class Function{
	public: 
	std::string name; 
	std::map<std::string , Variable *> argList; 
	int ret_type; 
	//name of function, type list for arguments and return type is the function signature

	AST_Tree_Node * body; //body of a function after definiton 
	TypeObject * eval(int , std::map<std::string , Function *>); // if it returns a val;
	Function(std::string , int); 
	int level; 
}; 

class Environment{
public: 
	AST_Tree_Node *body; 
	int level; 
	std::map<std::string , Variable *> variable_table; //use for lookup 
	std::map<std::string , Function *> function_table; //use for calls; lookup is just complicated

	Environment(AST_Tree_Node * , int); //constructor 
	Environment(AST_Tree_Node * , std::map<std::string , Variable *> , std::map<std::string , Function *> , int);
	Environment(AST_Tree_Node * , std::map<std::string , Variable *> , int); //
	//Environment(AST_Tree_Node * , std::vector<Variable *> , std::vector<Function *>); //for blocks
	TypeObject * typecheck_and_eval(AST_Tree_Node *); //takes AST_Tree_Node and evaluates
	TypeObject * eval(); //evaluates body and returns error in case 
	TypeObject * get_var(std::string); //variable lookup
	Function * get_func(std::string); //function lookup
	TypeObject * call_func(Function * , AST_Tree_Node *); //calling function and returning the val if any
	TypeObject * get_val(TypeObject *lhs , std::string op , TypeObject *rhs); //lhs , rhs and op to get the answer
	int child_counter; 


	// void add_var_list(std::vector<Variable*>); //add variables from outer scope
	// void add_func_list(std::vector<Function *>); //outside functions inside and inside too

	// void add_var_scope(std::string , std::string); //add variable to current scope
	// void add_func_scope(std::string name , std::string); 

	//printing functions
	void print_var_table();
	void print_func_table();
}; 

class Function_Frame{
	public: 
	Environment *env; 
	TypeObject *ans;

	void call();  
}; 

#endif 