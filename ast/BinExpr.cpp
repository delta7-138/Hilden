#include "BinExpr.h"

/*Grammar for binary expression is
E -> E + E
| -> E * E
| -> E - E
| -> E / E (what about '=' we will deal later with mutability)
*/

llvm::Value *BinExpr::codegen(){
    llvm::Value *L = LHS->codegen(); 
    llvm::Value *R = RHS->codegen(); 
    if(!L || !R){
        return nullptr;
    }
    switch(Op){
        case '+':
        return Builder.CreateFAdd(L , R , "addtmp");
        case '-': 
        return Builder.CreateFSub(L , R , "subtmp"); 
        case '*': 
        return Builder.CreateFMul(L , R , "multmp");
        case '/':
        return Builder.CreateFDiv(L , R , "divtmp");  
        default: 
        return LogErr("Binary operator does not exist");
    }
}