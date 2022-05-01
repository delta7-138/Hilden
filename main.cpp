#include "parse/op_parser.h"
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
	std::vector<TokenType>tList = gettok(data);
	// for(int i = 0; i<tList.size(); i++){
	//      tList[i].print();
	// }
	
	print_ast(parse_expression(init_parse(tList) , tList.size()) , 0); 
	return 0;
}
