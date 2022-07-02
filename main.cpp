#include "parse/op_parser.h"
#include "lex/lexer.h"
#include "lex/scanner.h"
#include "lex/token.h"
#include <fstream>
#include <iostream>

int main(int argc , char *argv[]){
	if(argc<=1){
		std::cout<<"Provide a file!!!"<<std::endl;
		exit(0); 
	}
	// std::ifstream infile(argv[1]);
	// std::string data; 
	// if(infile){
	// 	std::ostringstream ss; 
	// 	ss<<infile.rdbuf(); 
	// 	data = ss.str(); 
	// }
	// std::vector<TokenType>tList = gettok(data);
	std::vector<TokenType>tList = gettokenlist(argv[1]);
	for(int i = 0; i<tList.size(); i++){
	     tList[i].print();
	}
	// parse_expression(tList); 
	return 0;
}
