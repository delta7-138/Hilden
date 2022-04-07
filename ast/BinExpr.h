#ifndef __BIN_EXPR_H_
#define __BIN_EXPR_H_

#include "logger/logger.h"
#include "ast/expression.h"

class BinExpr : public ExpressionAST{
    char Op; //operator 
    std::unique_ptr<ExpressionAST> LHS , RHS; //LHS AND RHS of Expression
    public: 
    BinExpr(char Op , std::unique_ptr<ExpressionAST>LHS , std::unique_ptr<ExpressionAST>RHS) : Op(Op) , LHS(std::move(LHS)) , RHS(std::move(RHS)){}
    llvm::Value *codegen() override; 
};
#endif