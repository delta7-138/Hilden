#include "op_parser.h"
int token_list_ptr; 
std::vector<TokenType>tList; 
TokenType curtok; 
AST_Tree_Node* head; 
std::map<int , int>block_lookup; 
int get_precedence(std::string tokval){
	if(tokval=="+" || tokval=="-"){
		return ADD_PREC; 
	}else if(tokval=="*" || tokval=="/"){
		return MUL_PREC;
	}else{
		return EQ_PREC;
	}
	return -1; 
}

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

AST_Tree_Node * init_parse(std::vector<TokenType>tokenList){
	TokenType tok_E; 
	tok_E.token_val = "E"; 
	tok_E.token_number = tok_undef;
	head = new AST_Tree_Node(tok_E); 
	token_list_ptr = 0; 
	tList = tokenList; 
	curtok = tList[0]; 

	//Creating block lookup table 
	std::stack<int>block_stack; 
	int index = -1; 
	for(int i = 0; i<tList.size(); i++){
		if(tList[i].token_number==tok_open_b){
			index = i; 
			break; 
		}
	}
	if(index!=-1){
		block_stack.push(index); 
		index++; 
		while(true){ // creating lookup table for starting and ending position of blocks with starting position as key. 
			//return error in case index > size of tList
			if(index>=tokenList.size()){
				break; 
			}
			TokenType tmp = tList[index]; 
			if(tmp.token_number==tok_open_b){
				block_stack.push(index); 
			}
			else if(tmp.token_number==tok_close_b){
				block_lookup.insert(std::pair<int , int>(block_stack.top() , index)); 
				std::cout<<block_stack.top()<<" "<<index<<std::endl; 
				if(block_stack.empty()){
					return nullptr; //error in parsing unbalanced block braces
				}
				block_stack.pop(); 
			}
			index++; 
		}
		if(!block_stack.empty()){
			return nullptr; //error in parsing unbalanced block braces
		}
	}
	return head; 
}

void get_next_tok(){
	token_list_ptr++; 
}



void print_ast(AST_Tree_Node* node, int depth=0){
	std::cout << "[" << node->node_type << ":";
	node->tok->print();
	std::cout << "]" << std::endl;
	for(int i=0; i < node->childList.size(); i++){
		for(int j = 0; j < depth; j++){
			std::cout<<"|_ ";
		}
		print_ast(node->childList[i], depth+1);
	}
}

float get_ans(float lhs , std::string op , float rhs){
	if(op=="+"){
		return lhs + rhs; 
	}else if(op=="*"){
		return lhs * rhs;
	}else if(op=="/"){
		return lhs / rhs; 
	}else if(op=="-"){
		return lhs - rhs;
	}else if(op ==">="){
		return lhs >= rhs;
	}else if(op =="<="){
		return lhs <= rhs; 
	}else if(op == "!="){
		return lhs != rhs; 
	}else if(op == "=="){
		return lhs == rhs; 
	}
	return -1.0; 
}

/**
 * Parses binary expressions. Takes token list tList of the binary expression
 * Uses the Shunting Yard algorithm
 * Returns a binary AST Node
 */
AST_Tree_Node* parse_binary(){
	std::vector<TokenType>output_q; 
	std::stack<TokenType>op_stack; 

	int prev_tok_number = 1000;
	for(int i = token_list_ptr; tList[token_list_ptr].token_number!=tok_sep || tList[token_list_ptr].token_val=="then"; i++ , token_list_ptr++){
		TokenType tmp = tList[i]; 
		int token_number = tmp.token_number; 
		std::string token_value = tmp.token_val;
		if(token_number==tok_hfloat || token_number==tok_id){
			if(token_number==tok_hfloat){
				output_q.push_back(tmp);
			}
		} else if(token_number == tok_operator){
			while(!op_stack.empty()){
				int prec_top = get_precedence(op_stack.top().token_val);
				int prec_curr_tok = get_precedence(token_value); 
				if(prec_curr_tok >= prec_top){
					break; 
				}
				TokenType top = op_stack.top(); 
				op_stack.pop(); 
				output_q.push_back(top);
			}
			op_stack.push(tmp); 
		} else if(token_number == tok_open_p) op_stack.push(tmp);
		else if(token_number == tok_close_p){
			while(!op_stack.empty()){

				if(op_stack.top().token_val == "("){
					op_stack.pop(); 
					break; 
				}

				TokenType top = op_stack.top(); 
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
		if(op_stack.top().token_number!=tok_open_p){
			output_q.push_back(op_stack.top());
			op_stack.pop();
		}
	}
	
	std::stack<AST_Tree_Node*>parse_stack;
	for(int i = 0; i<output_q.size(); i++){
		TokenType tok = output_q[i]; 

		if(tok.token_number==tok_hfloat || tok.token_number==tok_id){
			AST_Tree_Node* ast = new AST_Tree_Node(tok);
			ast->eval_tok = new TokenType(tok);
			parse_stack.push(ast);
		} else if(tok.token_number == tok_operator){
			AST_Tree_Node* rhs_Node = parse_stack.top();
			parse_stack.pop();
			if(parse_stack.empty()){
				// return std::stof(rhs_Node->tok->token_val); 
				return rhs_Node;
			}
			AST_Tree_Node* lhs_Node = parse_stack.top();
			parse_stack.pop();
			float ans = get_ans(std::stof(lhs_Node->eval_tok->token_val), tok.token_val, std::stof(rhs_Node->eval_tok->token_val));
			TokenType tok_ans;
			tok_ans.token_number = tok_hfloat;
			tok_ans.token_val = std::to_string(ans);
			AST_Tree_Node* ast = new AST_Tree_Node(tok);
			ast->eval_tok = new TokenType(tok_ans);
			ast->add_child(lhs_Node);
			ast->add_child(rhs_Node);
			parse_stack.push(ast); 
		}
	}
	AST_Tree_Node* ast = parse_stack.top();
	parse_stack.pop();
	// print_ast(ast);

	// return std::stof(ast->tok->token_val);
	return ast;
}

/**
 * Parses ternery expressions. Takes a token list tList of the ternery expression
 * Returns a ternery AST node
 */
AST_Tree_Node* parse_ternary(std::vector<TokenType> tList) {
	return nullptr;
}

/**
 * Parses the entire code. Takes a token list tList of the entire code.
 * Calls parse_unary, parse_binary or parse_ternary depending on the expression.
 * Constructs the abstract syntax tree
 * TODO: Return the AST instead of printing.
 */


/**
Grammar of the language
E -> stmt;E | [E]
stmt -> A | D | Def | If | Loop | U {A for Assignment D for Declaration and C for Calling a function Def for function definition ,
If and Loop are self-explanatory
U for unary updation}

D -> <htype> id | htype fid (<htype>,*)
Def -> <htype> fid (<htype> id, *)Body  (Body for function body)
A -> id = S  | <htype> id = S (S is a subexpression containing Binary, Unary and Ternary Operations)
U -> ++id | id++ | --id | id-- | id+=S | id-=S 
If -> if S then E else E
Body -> [E] | [E ret S]
*/


/**
Recursive parse_expression relies on two variables -> head of the AST passed and the tokenList index upto which it should keep parsing
In case of blocks term is set as the closing block symbol to stop parsing when it reaches there, the index of which is stored in the block lookup table
computed at the start of parsing. 
Also in case of blocks we can accordingly pass the child, call parse_expression on the child with the appropriate term and get the tree for the same, 
which is then added as to the childList of the head in the function frame calling it.
This mechanism is useful in function calls as well and we can later add the environment variable which is essential for semantic analysis. 
*/
AST_Tree_Node* parse_expression(AST_Tree_Node *head , int term){
	if(token_list_ptr>=tList.size() || token_list_ptr>=term){ //Base case for recursion
		return head; 
	}
	curtok = tList[token_list_ptr]; 
	int token_number = curtok.token_number; 
	std::string curtok_val = curtok.token_val; 


	if(token_number==tok_open_b){
		// E -> [E] rule 
		AST_Tree_Node *newblocknode; 
		newblocknode = new AST_Tree_Node(curtok); 
		TokenType blocktok; 
		blocktok.token_val = "E"; 
		blocktok.token_number = tok_undef;
		AST_Tree_Node *subblock = new AST_Tree_Node(blocktok); 
		get_next_tok(); 
		
		newblocknode->add_child(parse_expression(subblock , block_lookup.at(token_list_ptr-1))); 

		newblocknode->add_child(new AST_Tree_Node(tList[token_list_ptr]));
		
		if(token_list_ptr==tList.size()){
			head->add_child(newblocknode); 
			return head;
		}
		
		head->add_child(newblocknode); 
	}

	else if(token_number==tok_keyword){
		//stmt -> D | If | Loop | Def rule 
		AST_Tree_Node *newnode;
		bool assflag; 
		AST_Tree_Node *assnode;
		if(curtok_val=="hif"){ //stmt -> If rule 
		}else if(curtok_val=="hwhile"){ //stmt -> while rule 

		}else{ // here we have to see lookahead and further check if it is the stmt->D smt->Def rule 
			assflag = false; 
			TokenType la2 = tList[token_list_ptr+2];  //Check if it is assignment and declaration also (A -> <htype> id = S rule)
			if(la2.token_val=="="){ //creating a declaration node and an assignment node of type <hfloat> -> <x> and =->(x , expr)
				newnode = new AST_Tree_Node(curtok , n_unary);
				newnode->add_child(new AST_Tree_Node(tList[token_list_ptr+1] , n_unary));
				assnode = new AST_Tree_Node(la2 , n_binary); 
				assnode->add_child(new AST_Tree_Node(tList[token_list_ptr+1] , n_unary));
				assnode->add_child(parse_binary());
				assflag = true; 

			}else if(la2.token_val==";"){ //creating a declaration node of type <hfloat> -> <idname> 
				newnode = new AST_Tree_Node(curtok , n_unary);
				newnode->add_child(new AST_Tree_Node(tList[token_list_ptr+1] , n_unary));
				get_next_tok();
				get_next_tok(); 
			}else{
				//Function declaration also has arguments 
			}
		}
		head->add_child(newnode);
		if(assflag){ // only if assignment happens add this node
			head->add_child(assnode);
		}

	}else if(token_number==tok_id){
		//this is the A -> id = S or stmt -> U (for unary updation) rule 
		AST_Tree_Node *child; 
		TokenType la1 = tList[token_list_ptr+1]; //lookahead by 1
		if(la1.token_val=="="){
			child = new AST_Tree_Node(la1 , n_binary);
			child->add_child(new AST_Tree_Node(curtok , n_unary)); 
			get_next_tok(); 
			get_next_tok(); 
			child->add_child(parse_binary());
		}
		head->add_child(child);   
	}
	token_list_ptr++; 
	return parse_expression(head , term); //calling it recursively
}