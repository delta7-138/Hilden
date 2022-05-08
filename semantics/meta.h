#ifndef __META_H_
#define __META_H_
#include "parse/parser.h"

class Variable{
	std::string name; 
	std::string type; 
	std::val; //on declaration memory will also be allocated
}; 

class Function{
	std::string name; 
	std::vector<std::string> typeList; 
	std::string ret_type; 
	//name of function, type list for arguments and return type is the function signature

	AST_Tree_Node * body; //body of a function after definiton 
	TypeObject * eval(); // if it returns a val;
}; 

class TypeObject{
	std::string val; 
	std::string type; //"error" for error
};


#endif