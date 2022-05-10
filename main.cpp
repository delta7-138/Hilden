#include "parse/parser.h"
#include "lex/lexer.h"
#include "lex/token.h"
#include "semantics/environment.h"
#include <fstream>
#include <iostream>

int main(int argc , char *argv[]){
	if(argc <= 1){
		std::cout<<"No input file"<<std::endl;
		exit(0); 
	}

	std::ifstream infile(argv[1]);
	std::string data; 
	if(infile){
		std::ostringstream ss;   
		ss<<infile.rdbuf(); 
		data = ss.str(); 
	}
	Lexer *lexer = new Lexer(data); 
	std::vector<TokenType>tList = lexer->gettok(); 
	//lexer->print_token_list(); 
	//std::cout<<std::endl;

	Parser *parser = new Parser(tList , "E", 0); 
	parser->parse();
	if (argc == 3) {
		parser->print_ast(parser->root, 0);	
	}
	//parser->print_ast(parser->root , 0); 

	Environment *env = new Environment(parser->root , 0);  
	TypeObject *retval = env->eval(); 

	if(retval->type == 0){
		std::cout<<retval->val<<std::endl; 
		exit(0); 
	}
	// env->print_var_table(); 
	// env->print_func_table(); 
	return 0;
}
