#ifndef __NUM_EXPR_H_
#define __NUM_EXPR_H_

#include "expression.h" //for inherting the expression class and using the virtual AST function

class NumExpr : public ExpressionAST{
    float val; //for hfloat 
    public: 
        NumExpr(double Val){
            val = Val;
        }
        llvm::Value *codegen() override; 
}; 

#endif

