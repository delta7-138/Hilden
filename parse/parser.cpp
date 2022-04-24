/* 
TO DO LIST : 
1. Understanding unique_ptr and move functions. 
2. Then use some generic parsing algorithm like LL(1)
3. At least one dry run of the basic arith instructions to be done by today
*/

/*
We need to use LL(1) parsing 
for binary expression grammar
*/
#include "parser.h" 
#define STATE_E 1
#define STATE_T 2
#define STATE_F 3
#define STATE_E_DASH -1
#define STATE_T_DASH -2
#define STATE_IDENTIFIER 222
#define EPSILON 0

std::vector<int>production(int start , int input){
	std::vector<int>tmp; 
	if(start==STATE_E){
		if(input==STATE_IDENTIFIER || input=='('){
			tmp.push_back(STATE_T); 
			tmp.push_back(STATE_E_DASH);
		}
	}else if(start==STATE_T){
		if(input==STATE_IDENTIFIER || input=='('){
			tmp.push_back(STATE_F); 
			tmp.push_back(STATE_T_DASH);
		}
	}else if(start==STATE_F){
		if(input=='('){
			tmp.push_back('('); 
			tmp.push_back(STATE_E); 
			tmp.push_back(')');
		}else if(input==STATE_IDENTIFIER){
			tmp.push_back(STATE_IDENTIFIER);
		}

	}else if(start==STATE_E_DASH){
		switch(input){
			case '+':
			tmp.push_back('+'); 
			tmp.push_back(STATE_T); 
			tmp.push_back(STATE_E_DASH); 
			break; 
			case '-': 
			tmp.push_back('-');
			tmp.push_back(STATE_T);
			tmp.push_back(STATE_E_DASH);
			break;
			case 0:
			case ')':
			case '$':
			tmp.push_back(EPSILON);
			break;
		}
	}else if(start==STATE_T_DASH){
		switch(input){
			case '*':
			tmp.push_back('*'); 
			tmp.push_back(STATE_F); 
			tmp.push_back(STATE_T_DASH);
			break;

			case '/':
			tmp.push_back('/');
			tmp.push_back(STATE_F);
			tmp.push_back(STATE_T_DASH); 
			break;

			case 0: 
			case '+':
			case '-':
			case ')':
			case '$':
			tmp.push_back(EPSILON);
			break;
		}
	}
	return tmp; 
}


float build_parse_tree(std::vector<TokenType>tokenList){
	int size = tokenList.size();
	std::stack<int>stack; 
	std::queue<float>ast; 
	stack.push('$');
	stack.push(STATE_E); 


	std::queue<TokenType>queue; 
	for(int i = 0; i<size; i++){
		TokenType tmp = tokenList[i]; 
		queue.push(tmp); 
	}

	TokenType head_tok;
	while(!queue.empty()){
		std::cout<<"in"<<std::endl; 
		head_tok = queue.front();
		std::string head_tok_val = head_tok.token_val;
		int head_tok_number = head_tok.token_number; 
		int head_stack = stack.top(); 
		std::vector<int>nextState;  
		switch(head_tok_number){
			case tok_hfloat: 
			case tok_id: 
			nextState = production(head_stack , STATE_IDENTIFIER);
			break;
			case tok_operator: 
			case tok_open_p: 
			case tok_close_p: 
			nextState = production(head_stack , head_tok_val.at(0));
		}
		stack.pop(); //when new production rule
		for(int i = 0; i<nextState.size(); i++){
			stack.push(nextState[i]); 
		}
		if(stack.empty()){
			std::cout<<"empty";
		}
		int head_stack_next = stack.top(); 
		std::cout<<head_stack_next<<std::endl;
		if(head_stack_next == STATE_IDENTIFIER || head_stack_next == '(' || head_stack_next == ')' || head_stack_next == '+' || head_stack_next == '-' || head_stack_next == '*' || head_stack_next =='/'){
			stack.pop();
			queue.pop();
			std::cout<<queue.front().token_val<<std::endl;
			if(stack.empty()){
				std::cout<<"empty"<<std::endl;
			}
		}else if(head_stack==EPSILON){
			stack.pop();
		}else{
			std::cout<<"continue"<<std::endl;
		}
	} 
	return 0;
}