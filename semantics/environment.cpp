#include "environment.h"

Variable :: Variable(std::string name , int type , int level){
    this->name = name;  
    this->type = type; 
	this->val = ""; 
	this->env_level = level; 
}

TypeObject :: TypeObject(int type , std::string val){
    this->type = type;
    this->val = val;
}   

Environment::Environment(AST_Tree_Node *body , int level ){
	this->body = body;
	this->level = level; 
}

Environment::Environment(AST_Tree_Node *body , std::map<std::string , Variable*>vartable , std::map<std::string , Function*>ftable , int level){
	this->body = body; 
	//outer initialization
	this->variable_table = vartable; 
	this->function_table = ftable; 

	this->level = level; 
}

void Environment::print_var_table(){
	std::map<std::string, Variable *>::iterator it;

	for (it = variable_table.begin(); it != variable_table.end(); it++)
	{
		std::cout << it->first    // string (key)
				<< ':'
				<< it->second->val   // string's value 
				<< std::endl;
	}

	std::cout<<std::endl; 
}

template <typename T> T calc(T x , std::string op , T y){

	if(op == "+"){
		return x + y;
	}else if(op == "-"){
		return x-y; 
	}else if(op == "*"){
		return x*y; 
	}else if(op == "/"){
		return x/y;
	}else if(op == "<="){
		return x<=y; 
	}else if(op == ">="){
		return x>=y; 
	}else if(op == "=="){
		return x==y;
	}else if(op == "!="){
		return x!=y;
	}
	return -1;

}

TypeObject * Environment::get_val(TypeObject *lhs , std::string op , TypeObject *rhs){
	int lhs_type = lhs->type; 
	int rhs_type = rhs->type; 
	std::string val = ""; 
	int type = 0; 
	// std::cout<<lhs->val<<" "<<lhs->type<<std::endl; 
	// std::cout<<rhs->val<<" "<<rhs->type<<std::endl;

	if(lhs_type == tok_hfloat && rhs_type == tok_hint || lhs_type == tok_hint && rhs_type == tok_hfloat || lhs_type == tok_hfloat && rhs_type == tok_hfloat){
		
		//std::cout<<"in2"; 
		float lhs_val = std::stof(lhs->val);
		float rhs_val = std::stof(rhs->val); 
		float ans = calc<float>(lhs_val , op , rhs_val); 
		std::ostringstream ss;
		ss << ans;
		std::string s(ss.str());
		val = s; 
		type = tok_hfloat;

	}else if(lhs_type == tok_hint && rhs_type == tok_hint){

		//std::cout<<"in";
		int lhs_val = std::stoi(lhs->val);
		int rhs_val = std::stoi(rhs->val); 
		int ans = calc<int>(lhs_val , op , rhs_val); 
		//std::cout<<ans<<std::endl;
		std::ostringstream ss;
		ss << ans;
		std::string s(ss.str());
		val = s; 
		type = tok_hint;

	}else{
		type = 0; 
		val = "Type Error!"; 
	}

	return (new TypeObject(type , val));
}

TypeObject * Environment::get_var(std::string id_name){
	
	if(variable_table.find(id_name) == variable_table.end()){
		return new TypeObject(0 , "Varible Does not exist!"); 
	}

	Variable *var = variable_table.at(id_name); 
	return new TypeObject(var->type , var->val);
}

TypeObject * Environment::call_func(Function *func , AST_Tree_Node *fcallnode){
	return new TypeObject(tok_hint, "0"); 
}


TypeObject * Environment::typecheck_and_eval(AST_Tree_Node *root){
	
	if (root->node_type == "LIT"){

		TypeObject *literal = new TypeObject(root->tok->token_number , root->tok->token_val); 
		return literal; 

	}else if (root->node_type == "ID"){

		//std::cout<<root->tok->token_val<<std::endl; 
		TypeObject *variable_val = get_var(root->tok->token_val);
		if(variable_val->type == 0){
			perror(variable_val->val.c_str()); 
			exit(0); 
		} 
		//std::cout<<variable_val->type<<" "<<variable_val->val<<std::endl; 
		return variable_val; 

	}else if (root->node_type == "FCALL"){

		// Function *func_sig = get_func(root->tok->token_val); 
		// TypeObject *ret_val = call_func(func_sig , root); 
		// return ret_val; 
	}

	std::string op = root->tok->token_val; 
	AST_Tree_Node *lhs = root->childList[0]; 
	AST_Tree_Node *rhs = root->childList[1]; 

	TypeObject *lhs_val = typecheck_and_eval(lhs); 
	TypeObject *rhs_val = typecheck_and_eval(rhs);

	return get_val(lhs_val , op , rhs_val);  
	
}

TypeObject * Environment::eval(){

	for(int i = 0; i<body->childList.size() ; i++){
		AST_Tree_Node *root = body->childList[i]; 
		//std::cout<<root->node_type<<std::endl; 
		if(root->node_type == "B"){ //Block 

			Environment *blockenv = new Environment(root , variable_table , function_table , level + 1);
			blockenv->eval(); 
			blockenv->print_var_table(); 
		 
		}else if(root->node_type == "DEC"){ //Declaration
			
			int type = 0; 
			if(root->tok->token_val == "hint"){
				type = tok_hint; 
			}else if(root->tok->token_val == "hfloat"){
				type=  tok_hfloat; 
			}else if(root->tok->token_val == "hchar"){
				type = tok_hchar; 
			}
			
			if(variable_table.find(root->childList[0]->tok->token_val) != variable_table.end()){

				Variable *checkvar = variable_table.at(root->childList[0]->tok->token_val); 
				if(checkvar->env_level == level){
					return new TypeObject(0 , "Cannot declare variable with same identifier again!");
				}
			}

			Variable *newvar = new Variable(root->childList[0]->tok->token_val , type , level);
			variable_table.insert(std::pair<std::string , Variable *>(newvar->name , newvar)); 
			
		}else if(root->node_type == "U"){ //Updation 

			AST_Tree_Node *idnode = root->childList[0]; 
			AST_Tree_Node *expr = root->childList[1]; 

			if(variable_table.find(idnode->tok->token_val) == variable_table.end()){
				//print_var_table(); 
				return new TypeObject(0 , "Variable not declared!"); 
			}

			Variable *var = variable_table.at(idnode->tok->token_val); 
			TypeObject *eval_expr = typecheck_and_eval(expr); 
			if(eval_expr->type != var->type){
				return new TypeObject(0 , "Type Error! variable is declared with a different type"); 
			}

			var->val = eval_expr->val;

		}else if(root->node_type == "FC"){ //Function Call 

			

		}else if(root->node_type == "IF"){ //conditional 

		}

	}
	return new TypeObject(222 , "success"); 
}