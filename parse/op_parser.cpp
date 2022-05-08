#include "parser.h"

//AST_Tree_Node functions
AST_Tree_Node :: AST_Tree_Node(TokenType tok)
{
	this->tok = new TokenType(tok);
	this->node_type = n_binary;
}

AST_Tree_Node :: AST_Tree_Node(TokenType tok, int node_type)
{
	this->tok = new TokenType(tok);
	this->node_type = node_type;
}

int AST_Tree_Node :: add_child(AST_Tree_Node* node)
{
	this->childList.push_back(node);
	return 0;
}

std::vector<TokenType>getListTill(std::vector<TokenType>list , std::string delim){

	std::vector<TokenType>tList; 
	for(int i = 0; i<list.size() ; i++){
		if(tList[i].token_val == delim){
			return tList;
		}
		tList.push_back(tList[i]);
	}
	return tList; 
}

int Parser::get_precedence(std::string tokval){
	if(tokval=="+" || tokval=="-"){
		return ADD_PREC; 
	}else if(tokval=="*" || tokval=="/"){
		return MUL_PREC;
	}else{
		return EQ_PREC;
	}
	return -1; 
}

void Parser::print_ast(AST_Tree_Node* node, int depth=0){
	// if (node->tok == NULL || node == NULL || node->node_type > 5 || node->node_type < 0) return;
	std::cout << "[" << node->node_type << ":";
	node->tok->print();
	std::cout << "]" << std::endl;
	for(int i=0; i < node->childList.size(); i++){
		for(int j = 0; j < depth; j++){
			std::cout <<"|  ";
		}
		std::cout << "|__";
		print_ast(node->childList[i], depth+1);
	}
}

Parser::Parser(std::vector<TokenType>tList , std::string start_val){
	TokenType start; 
	start.token_val = start_val; 
	start.token_number = tok_undef; //For denotation only
	root = new AST_Tree_Node(start);
	for(int i = 0; i<tList.size(); i++){
		fresh_tokens.push(tList[i]); 
	}
}

TokenType Parser::get_next_tok(){
	TokenType dummy; 
	dummy.token_val = "dummy"; 
	dummy.token_number = tok_undef;

	if(!fresh_tokens.empty()){
		parsed_tokens.push(fresh_tokens.front());
		fresh_tokens.pop();  
		if(fresh_tokens.empty()){
			return dummy; 
		}
		return fresh_tokens.front(); 
	}

	return dummy; 
}

TokenType Parser::get_cur_tok(){
	if(!fresh_tokens.empty ()){
		return fresh_tokens.front(); 
	}
	TokenType dummy; 
	dummy.token_val = "dummy"; 
	dummy.token_number = tok_undef;
	return dummy; 
}


AST_Tree_Node * Parser::parse_binary(std::vector<TokenType>tList){

	std::vector<AST_Tree_Node *> unparsed_AST; //partially parsed list 
	//first off convert all Tokens to AST Nodes to make parsing easier for function calls
	for(int i = 0; i<tList.size() ; i++){

		TokenType cur = tList[i];
		TokenType la; 

		if( i+1 < tList.size() ){
			la =tList[i+1]; 
		}

		if(la.token_number == tok_open_c){ //function call is happening 

			AST_Tree_Node * fcallnode = new AST_Tree_Node(cur); //fid token to be parent 
			i+=2; //go to lookahead then go to the first token of the first argument 
			while(i<tList.size() && tList[i].token_number != tok_close_c){
				int ctr = 0; 
				std::vector<TokenType>arg; 

				while(i<tList.size()){

					if(tList[i].token_number == tok_open_c){
						ctr++; 
					}else if(tList[i].token_number == tok_close_c){
						ctr--; 
					}

					if(ctr == 0 && tList[i].token_number == tok_coma){
						break;
					} 
					arg.push_back(tList[i]); 
					i++; 
				}

				fcallnode->add_child(parse_binary(arg)); 
				i++; 
			}
			unparsed_AST.push_back(fcallnode); 

		}else{

			unparsed_AST.push_back(new AST_Tree_Node(cur)); 
		}
	}

	std::vector<AST_Tree_Node *>output_q; //output queue to store the RPN representation
	std::stack<AST_Tree_Node *>op_stack; //operator stack to store the operators 

	int prev_tok_number = 1000;

	//Shunting yard algorithm to parse 

	for(int i = 0; i < unparsed_AST.size();  i++){

		AST_Tree_Node *curr = unparsed_AST[i]; 
		TokenType *curtok = curr->tok; 

		int token_number = curtok->token_number; 
		std::string token_value = curtok->token_val;

		if(token_number==tok_hfloat || token_number==tok_id || token_number==tok_hchar || token_number==tok_hint){

			output_q.push_back(curr);

		}else if(token_number == tok_operator){

			while(!op_stack.empty()){
				int prec_top = get_precedence(op_stack.top()->tok->token_val);
				int prec_curr_tok = get_precedence(token_value); 

				if(prec_curr_tok >= prec_top){
					break; 
				}

				AST_Tree_Node *top = op_stack.top(); 
				op_stack.pop(); 
				output_q.push_back(top);
			}
			op_stack.push(curr); 

		}else if(token_number == tok_open_p) op_stack.push(curr);

		else if(token_number == tok_close_p){

			while(!op_stack.empty()){

				if(op_stack.top()->tok->token_val == "("){
					op_stack.pop(); 
					break; 
				}

				AST_Tree_Node * top = op_stack.top(); 
				op_stack.pop(); 
				output_q.push_back(top);
			}
		}

		if((prev_tok_number==tok_hfloat || prev_tok_number==tok_id) && (token_number==tok_hfloat || token_number==tok_id)){
			perror("Syntax Error!");
			exit(0);
		} else if(prev_tok_number==tok_operator && token_number==tok_operator){
			perror("Syntax Error!");
			exit(0);
		} else if(prev_tok_number==tok_open_p && token_number==tok_operator){
			perror("Syntax Error!");
			exit(0);
		}

		prev_tok_number = token_number; 
	}

	while(!op_stack.empty()){

		if(op_stack.top()->tok->token_number!=tok_open_p){
			output_q.push_back(op_stack.top());
			op_stack.pop();
		}

	}


	
	std::stack<AST_Tree_Node*>parse_stack;

	for(int i = 0; i<output_q.size(); i++){

		AST_Tree_Node *curnode = output_q[i]; 
		TokenType *tok = curnode->tok;

		if(tok->token_number==tok_hfloat || tok->token_number==tok_id || tok->token_number == tok_hint || tok->token_number == tok_hchar){

			parse_stack.push(curnode); 

		} else if(tok->token_number == tok_operator){

			AST_Tree_Node* rhs_Node = parse_stack.top();
			parse_stack.pop();

			if(parse_stack.empty()){
				// return std::stof(rhs_Node->tok->token_val); 
				return rhs_Node;
			}

			AST_Tree_Node* lhs_Node = parse_stack.top();
			parse_stack.pop();
			// float ans = get_ans(std::stof(lhs_Node->eval_tok->token_val), tok.token_val, std::stof(rhs_Node->eval_tok->token_val));
		
			curnode->add_child(lhs_Node);
			curnode->add_child(rhs_Node);
			parse_stack.push(curnode); 
		}
	}

	AST_Tree_Node* ast = parse_stack.top();
	parse_stack.pop();
	// print_ast(ast);

	// return std::stof(ast->tok->token_val);
	return ast;
}

std::vector<TokenType> Parser::dequeue_and_return(std::string sep , std::string op , bool flag = false){
	std::vector<TokenType>ans; 
	std::stack<int>param; 

	if(flag){
		param.push(1);
	}
	//std::cout<<"Dequeuee"<<std::endl;
	while(true){
		TokenType curtok = get_cur_tok(); 
		if(curtok.token_val == "dummy"){
			break; 
		}

		if(flag  && curtok.token_val == op){
			param.push(1); 
		}

		get_next_tok(); 
		if(curtok.token_val == sep){ //if you reach the end terminate and return list
			if(flag){
				param.pop();
			}
			if(param.empty())
				break;  
		}
		//curtok.print(); std::cout<<std::endl; 
		//curtok.print(); 
		//std::cout<<std::endl;
		ans.push_back(curtok); 
	}
	//std::cout<<std::endl; 
	return ans; 
}

void Parser::parse(){
	//get_cur_tok().print(); 
	//std::cout<<std::endl; 
	if(fresh_tokens.empty()){
		return ; 
	}

	TokenType curtok = get_cur_tok();
	AST_Tree_Node *newnode; 
	AST_Tree_Node *assignnode; 
	bool assign = false , ifflag = false; //only assign if this flag is true

	if(curtok.token_number == tok_keyword){

		
		//case when keyword is a data type either declaration or declaration and assignment
		if(curtok.token_val == "hfloat" || curtok.token_val == "hint" || curtok.token_val == "hchar"){

			TokenType la1 = get_next_tok(); 
			TokenType la2 = get_next_tok(); 

			if(la2.token_number == tok_sep){ //<htype> <id>; only declaration

				newnode = new AST_Tree_Node(curtok); 
				newnode->add_child(new AST_Tree_Node(la1)); 

			}else if(la2.token_val == "="){ //<htype> <id> = <expr>; 

				newnode = new AST_Tree_Node(curtok); 
				newnode->add_child(new AST_Tree_Node(la1)); 
				assignnode = new AST_Tree_Node(la2); 
				assignnode->add_child(new AST_Tree_Node(la1));
				get_next_tok(); 
				std::vector<TokenType>stmt = dequeue_and_return(";" , "" , false); 
				AST_Tree_Node *stmtnode = parse_binary(stmt);
				assignnode->add_child(stmtnode); //dequeue and return will end at separator mentioned and return the list of tokens till it reaches the end
				assign = true; 

			}else if(la2.token_val == "("){

				newnode = new AST_Tree_Node(curtok); 
				newnode->add_child(new AST_Tree_Node(la1));

				//iterating till ) <htype> fid( <htype> a , <htype> b ,  <htypr> c , ... );
				//function definition 
				get_next_tok(); 
				std::vector<TokenType>typeList = dequeue_and_return("[" , "" , false); 

				//getting param list

				for(int i = 0; i<typeList.size()-1; i+=3){

					AST_Tree_Node *param_type_node = new AST_Tree_Node(typeList[i]); 
					param_type_node->add_child(new AST_Tree_Node(typeList[i+1]));

					newnode->add_child(param_type_node); 
				}

				Parser *function_body_parser = new Parser(dequeue_and_return("]" , "[" , true) , "FB");
				function_body_parser->parse(); 
				newnode->add_child(function_body_parser->root); //adding body of function definition
			}

		}else if(curtok.token_val == "hif"){

			newnode = new AST_Tree_Node(curtok);
			newnode->add_child(parse_binary(dequeue_and_return("[" , "" , false)));
			//get_cur_tok().print(); 
			Parser * parse_body_1 = new Parser(dequeue_and_return("]" , "[" , true) , "E");
			parse_body_1->parse(); 
			newnode->add_child(parse_body_1->root); 
			ifflag = true; 

		}else if(curtok.token_val == "ret"){

			newnode = new AST_Tree_Node(curtok); 
			get_next_tok(); 
			newnode->add_child(parse_binary(dequeue_and_return(";" , "" , false))); 
			//dequeue_and_return("]" , "" , false); 
		}


	}else if(curtok.token_number == tok_id){

		TokenType la = get_next_tok(); 
		newnode = new AST_Tree_Node(la); 

		if(la.token_val == "="){
			newnode->add_child(new AST_Tree_Node(curtok));
			get_next_tok();
			newnode->add_child(parse_binary(dequeue_and_return(";" , "" , false)));
		}

	}else if(curtok.token_number == tok_open_b){

		get_next_tok(); 
		Parser * parse_body = new Parser(dequeue_and_return("]" , "[" , true) ,"E");
		parse_body->parse();
		newnode = parse_body->root;
	}

	if(curtok.token_val != "dummy"){
		root->add_child(newnode); 
		if(assign){
			root->add_child(assignnode); 
		}
	}
	//fresh_tokens.front().print(); 
	parse(); 
}


