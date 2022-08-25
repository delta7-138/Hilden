#include "environment.h"

std::stack<Function_Frame>fstack; //please work 
void print_map(std::map<std::string , Variable *>vlist){
	std::map<std::string, Variable *>::iterator it;

	for (it = vlist.begin(); it != vlist.end(); it++)
	{
		std::cout << it->first    // string (key)
				<< ':'
				<< it->second->val   // string's value 
				<< std::endl;
	}

	std::cout<<std::endl; 
}

int gettype(std::string s){
	int type = 0; 
	if(s == "hint"){
		type = tok_hint; 
	}else if(s == "hfloat"){
		type=  tok_hfloat; 
	}else if(s == "hchar"){
		type = tok_hchar; 
	}else if(s == "hvoid"){
		type = void_type; 
	}else if(s == "hstring"){
		type = tok_hstring;
	}
	return type; 
}

Function :: Function(std::string name , int type){
	this->name = name; 
	this->ret_type = type; 
}

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

Environment::Environment(AST_Tree_Node *body , std::map<std::string , Variable *>vmap , int level){
	this->body = body; 
	this->variable_table = vmap; 
	this->level = level;
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

void Environment::print_func_table(){
	std::map<std::string, Function *>::iterator it;

	for (it = function_table.begin(); it != function_table.end(); it++)
	{
		std::cout << it->first    // string (key)
				<< ':'; 
		print_map(it->second->argList);   // string's value
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
	}else if(op == "&"){
		return x&&y; 
	}else if(op == "|"){
		return x||y; 
	}
	return -1;

}

std::string delim_string(std::string x){
	return x.substr(1 , x.length()-2); 
}

std::string create_string_two(std::string x , int ct , std::string y){
	for(int i = 0; i<ct; i++){
		x = x + y; 
	}
	return x; 
}

std::string comp(std::string x , std::string y , std::string op){
	if(op == "<="){
		if(x<=y){
			return "1";
		}

	}else if(op == ">="){
		if(x>=y){
			return "1"; 
		}

	}else if(op == "<"){
		if(x<y){
			return "1";
		}

	}else if(op == ">"){
		if(x>y){
			return "1"; 
		}

	}else if(op == "=="){
		if(x == y){
			return "1"; 
		} 
	}else if(op == "!="){
		if(x != y){
			return "1"; 
		}
	}
	return "0"; 
}

TypeObject * calc_string(std::string x , std::string op , std::string y , bool flag){
	TypeObject *typeobj = new TypeObject(0 , "err"); 
	typeobj->type = tok_hstring; 

	if(op == "+"){
		if(flag == true){
			typeobj->val = x + y; 
		}else{
			typeobj->val = create_string_two(x , std::stoi(y) , " ");  
		}
	}else if(op == "-"){
		if(flag == true){
			typeobj->val = "Invalid operation on strings (/)"; 
			typeobj->type = 0; 
		}else{
			int yint = std::stoi(y); 
			if(yint > x.length()){
				typeobj->val = "index out of bounds"; 
				typeobj->type = 0; 
			}else{
				typeobj->val = x.substr(yint); 
			}
		}

	}else if(op == "*"){

		if(flag == true){
			typeobj->val = "Invalid operation on strings (*)"; 
			typeobj->type = 0; 
		}else{
			typeobj->val = create_string_two(x , std::stoi(y) , x); 
		}

	}else if(op == "/"){

		if(flag == true){
			typeobj->val = "Invalid operation on strings (-)"; 
			typeobj->type = 0; 
		}else{
			int yint = std::stoi(y); 
			if(yint <= x.length()){
				typeobj->val = x.substr(0 , yint);
			}else{
				typeobj->val = "Integer operand must be less than the string length"; 
				typeobj->type = 0; 
			}
		}

	}else if(op == "<=" || op == ">=" || op == "<" || op == ">" || op == "!=" || op == "=="){
		if(flag == false){
			typeobj->val = "Type Error"; 
			typeobj->type  = 0; 
		}else{
			typeobj->val = comp(x , y , op); 
			typeobj->type = tok_hint; 
		}
	}else{
		typeobj->val = "Invalid operation on strings (" + op + ")"; 
		typeobj->type = 0; 
	}
	return typeobj; 
}

TypeObject * Function::eval(int level , std::map<std::string , Function *>ftable){
	Environment *fb = new Environment(body , argList , ftable , level ); 
	return fb->eval(); 
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

	}else if(lhs_type == tok_hstring || rhs_type == tok_hstring){
		if(rhs_type == tok_hstring && lhs_type == tok_hstring){
			return calc_string(lhs->val , op , rhs->val , true); 
		}
		else if(rhs_type == tok_hint && lhs_type == tok_hstring  || rhs_type == tok_hstring && lhs_type == tok_hint){
			std::string lhs_val , rhs_val; 
			if(rhs_type == tok_hint){
				lhs_val = lhs->val; 
				rhs_val = rhs->val; 
			}else{
				lhs_val = rhs->val; 
				rhs_val = lhs->val; 
			}

			return calc_string(lhs_val, op , rhs_val , false); 
		}
	}else{
		type = 0; 
		val = "Type Error!"; 
	}

	return (new TypeObject(type , val));
}

TypeObject * Environment::get_var(std::string id_name){
	
	//print_var_table(); 
	if(variable_table.find(id_name) == variable_table.end()){
		return new TypeObject(0 , "Varible Does not exist!"); 
	}

	Variable *var = variable_table.at(id_name); 
	return new TypeObject(var->type , var->val);
}

Function * Environment::get_func(std::string id_name){

	if(function_table.find(id_name) == function_table.end()){ 
		perror("Function not defined!");
		exit(0); 
	}

	Function *func = function_table.at(id_name); 
	return func; 
}

TypeObject * Environment::call_func(Function *func , AST_Tree_Node *fcallnode){

	//print_func_table(); 
	Function_Frame *function_frame = new Function_Frame(); 
	std::map<std::string , Variable *> varmap; 

	std::map<std::string, Variable *>::iterator varit; 
	for(varit = variable_table.begin() ; varit != variable_table.end() ; varit++){
		Variable *newvar = new Variable(varit->first , varit->second->type , level + 1); 
		newvar->val = varit->second->val; 
		varmap.insert(std::pair<std::string , Variable *>(newvar->name , newvar)); 
	}

	std::map<std::string , Variable *>::iterator argmap; 
	int i; 

	for(varit = func->argList.begin(), i = 0; varit != func->argList.end() ; varit++ , i++){

		Variable *newvar = new Variable(varit->first , varit->second->type , level + 1); 
		newvar->val = typecheck_and_eval(fcallnode->childList[i])->val;
		if(varmap.find(newvar->name) != varmap.end()){
			varmap.at(newvar->name) = newvar; 
			continue; 
		} 
		varmap.insert(std::pair<std::string , Variable *>(newvar->name , newvar)); 
	}

	function_frame->env = new Environment(func->body , varmap , function_table , level); 
	function_frame->ans = function_frame->env->eval(); 
	//function_frame->env->print_var_table(); 
	return function_frame->ans; 
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

		Function *func_sig = get_func(root->tok->token_val); 
		return call_func(func_sig , root); 
	}

	std::string op = root->tok->token_val; 
	AST_Tree_Node *lhs = root->childList[0]; 
	AST_Tree_Node *rhs = root->childList[1]; 

	TypeObject *lhs_val = typecheck_and_eval(lhs); 
	TypeObject *rhs_val = typecheck_and_eval(rhs);

	return get_val(lhs_val , op , rhs_val);  
	
}

TypeObject * Environment::eval(){
	//std::cout<<body->childList[3]->node_type<<std::endl;
	//print_var_table(); 
	for(int i = 0; i<body->childList.size() ; i++){
		AST_Tree_Node *root = body->childList[i]; 
		//std::cout<<i<<std::endl;
		//std::cout<<root->node_type<<std::endl; 
		if(root->node_type == "B"){ //Block 

			Environment *blockenv = new Environment(root , variable_table , function_table , level + 1);
			TypeObject *blockeval = blockenv->eval(); 
			if(blockeval->type == 0){
				std::cout<<blockeval->val<<std::endl; 
				exit(0); 
			}
			//std::cout<<blockeval->val<<std::endl ;
			//blockenv->print_var_table(); 
		 
		}else if(root->node_type == "DEC"){ //Declaration
			//print_var_table(); 
			int type = 0; 
			if(root->tok->token_val == "hint"){
				type = tok_hint; 
			}else if(root->tok->token_val == "hfloat"){
				type=  tok_hfloat; 
			}else if(root->tok->token_val == "hchar"){
				type = tok_hchar; 
			}else if(root->tok->token_val == "hstring"){
				type = tok_hstring; 
			}
			
			if(variable_table.find(root->childList[0]->tok->token_val) != variable_table.end()){

				Variable *checkvar = variable_table.at(root->childList[0]->tok->token_val); 
				if(checkvar->env_level == level){
					return new TypeObject(0 , "Cannot declare variable with same identifier again!");
				}else{
					Variable *newvar = new Variable(root->childList[0]->tok->token_val , type , level); 
					variable_table.at(newvar->name) = newvar; 
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

			if(eval_expr->type == 0){
				return eval_expr; 
			}

			if(eval_expr->type != var->type){
				return new TypeObject(0 , "Type Error! variable is declared with a different type"); 
			}

			var->val = eval_expr->val;

		}else if(root->node_type == "FUNC"){ //Function Definition
			//std::cout<<"in"<<std::endl; 
			int rettype = gettype(root->tok->token_val); 
			Function *newfunc = new Function(root->childList[0]->tok->token_val , rettype); 
			for(int j = 1; j<root->childList.size()-1 ; j++){

				AST_Tree_Node  *arg = root->childList[j]; 
				int var_type = gettype(arg->tok->token_val); 
				//std::cout<<arg->tok->token_val; 
				Variable *newvar = new Variable(arg->childList[0]->tok->token_val , var_type , level + 1);
				newfunc->argList.insert(std::pair<std::string , Variable *>(newvar->name , newvar)); 

			}

			newfunc->body = root->childList[root->childList.size() - 1]; 
			newfunc->level = level + 1; 

			function_table.insert(std::pair<std::string , Function *>(newfunc->name , newfunc)); 

		}else if(root->node_type == "IF"){ //conditional 

			TypeObject *cond = typecheck_and_eval(root->childList[0]); 
			if(cond->val == "1" || cond->val == "1.0"){
				Environment *ifblock = new Environment(root->childList[1] , variable_table , function_table , level + 1); 
				TypeObject *typeobj = ifblock->eval(); 
				if(typeobj->type != 222){
					return typeobj; 
				}

			}else if(root->childList.size() == 3){

				Environment *elseblock = new Environment(root->childList[2] , variable_table, function_table , level + 1); 
				TypeObject *typeobj = elseblock->eval(); 
				if(typeobj->type != 222){
					return typeobj; 
				}
			}

		}else if(root->node_type == "RET"){ //returning in function body

			TypeObject *retobj =  typecheck_and_eval(root->childList[0]); 
			return retobj; 

		}else if(root->node_type == "PRNT"){
			//std::cout<<"in"; 
			TypeObject *printobj = typecheck_and_eval(root->childList[0]); 
			std::cout<<printobj->val; 
			if(root->tok->token_val == "hprintln"){
				std::cout<<std::endl;
			}

		}else if(root->node_type == "WHILE"){
			while(true){

				TypeObject *cond = typecheck_and_eval(root->childList[0]);
				if(cond->val == "1" || cond->val == "1.0"){ 
					Environment *ifblock = new Environment(root->childList[1] , variable_table , function_table , level + 1); 
					TypeObject *typeobj = ifblock->eval(); 
					if(typeobj->type != 222){
						return typeobj; 
					}
				}else{
					break; 
				}
			}
		}
	}
	return new TypeObject(222 , "success"); 
}