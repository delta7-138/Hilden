#ifndef __EXPR_H_
#define __EXPR_H_

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"

class ExpressionAST{
    public: 
    virtual ~ExpressionAST(){}//virtual function for AST's using this as an interface
    virtual llvm::Value *codegen() = 0; 
};
#endif 