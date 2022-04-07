#include "logger/logger.h"
std::unique_str<ExpressionAST> LogErr(const char *str){
	fprintf(stderr, "LogErr : %s\n", str);
	return nullptr; 
}

llvm::Value *LogErrV(const char *str){
	LogErr(str); 
	return nullptr; 
}