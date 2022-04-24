#include "op_parser.h"
#include "lexer.h"
#include "token.h"
#include <fstream>
#include <iostream>

int main(int argc , char *argv[]){
	if(argc<=1){
		std::cout<<"Provide a file!!!"<<std::endl;
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
	parse_expression(tList); 
	return 0;
}
