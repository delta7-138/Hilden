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
#include "parser/parser.h"
#define STATE_E 1
#define STATE_T 2
#define STATE_F 3
#define STATE_E_DASH -1
#define STATE_T_DASH -2
#define STATE_IDENTIFIER 222
#define EPSILON 0
static ParseNumericNode::std::vector<int> production(int start , int input){
	vector<int>tmp; 
	if(start==STATE_E){
		tmp.push_back(STATE_T); 
		tmp.push_back(STATE_E_DASH);
	}else if(start==STATE_T){
		tmp.push_back(STATE_F); 
		tmp.push_back(STATE_T_DASH);
	}else if(start==STATE_F){
		if(start=='('){
			tmp.push_back('('); 
			tmp.push_back(STATE_E); 
			tmp.push_back(')');
		}else{
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
			tmp.push_back('-')
			tmp.push_back(STATE_T);
			tmp.push_back(STATE_E_DASH);
			break;
			case 0:
			tmp.push_back(EPSILON);
			break;
		}
	}
	return tmp; 
}

static ParseNumericNode::getNextNode(int start , int input){

}

struct *ParseTreeNumeric build_parse_tree(std::vector<TokenType>tokenList){
	int size = tokenList.size();
	stack<int>stack; 
	stack<ParseNumericNode>stack_leaves; 
	stack.push('$');
	stack.push(STATE_E); 

	queue<TokenType>queue; 
	for(int i = 0; i<size; i++){
		TokenType tmp = tokenList[i]; 
		queue.push(tmp); 
	}
	while(!queue.empty()){
		tmp = queue.front();
		std::string token_value = tmp.token_val; 
		std::vector<int>nextState; 
		switch(tmp.token_number){
			case tok_id: 
			case tok_hfloat: 
			nextState = ParseNumericNode::production(stack.top() , STATE_IDENTIFIER);
			break;
			case tok_op: 
			nextState = ParseNumericNode::production(stack.top() , token_value.at(0));
			break; 
		}
		stack.pop(); 
		for(int i = nextState.size()-1; i>=0; i--){
			stack.push(nextState[i]);
		}
		if(stack.top()==STATE_IDENTIFIER && tmp.token_number==tok_id){
			stack.pop();
			queue.pop(); 
		}
		else if(stack.top()==EPSILON){
			stack.pop();
		}
		else if(stack.top()==token_value.at(0)){
			stack.pop();
			queue.pop();
		}
	}
}