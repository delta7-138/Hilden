#include "NumExpr.h"

/*
Grammar : 
n -> [n] constant evaluates to constant
This is basically what happens in llvm code generation 
*/

llvm::Value  *NumExpr::codegen(){
    return llvm::ConstantFP::get(TheContext , llvm::APFloat(val));
}