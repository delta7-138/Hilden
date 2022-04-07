#ifndef __LOG_H_
#define __LOG_H_
#include "ast/expression.h"

std::unique_ptr<ExpressionAST> LogErr(const char *str); 
llvm::Value *LogErrV(const char *str); 
#endif