#include "semantics/environment.h"

Environment::Environment(AST_Tree_Node *body){
	this->body = body;
}

Environment::TypeObject * eval(){

	if(body->type == "E"){ //Block 

	}else if(body->type == "Dec"){ //Declaration

	}else if(body->type == "U"){ //Updation 

	}else if(body->type == "FC"){ //Function Call 

	}else if(body->type == "A"){ //Arith expr which returns a value 

	}else if(body->type == "IF"){ //conditional 

	}
}