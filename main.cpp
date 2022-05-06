#include "parse/parser.h"
#include "lex/lexer.h"
#include "lex/token.h"
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
	lexer->print_token_list(); 
	std::cout<<std::endl;

	Parser *parser = new Parser(tList); 
	parser->parse(); 
	parser->print_ast(parser->root , 0); 
	return 0;
}
