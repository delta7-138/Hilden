#include "op_parser.h"

static std::vector<Environment>envList; 
int curr_env_index = 0; 
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


//Uses Shunting Yard Algorithm
float parse_binary(std::vector<TokenType>tList){
	std::vector<TokenType>output_q; 
	std::stack<TokenType>op_stack; 

	int prev_tok_number = 1000;
	for(int i = 0; i<tList.size(); i++){
		TokenType tmp = tList[i]; 
		int token_number = tmp.token_number; 
		std::string token_value = tmp.token_val;
		if(token_number==tok_hfloat || token_number==tok_id){
			if(token_number==tok_hfloat){
				output_q.push_back(tmp);
			}else if(token_number==tok_id){
				Environment currenv = envList[curr_env_index];

				Variable *var = currenv.var_map.at(token_value); 
				TokenType literal; 
				literal.token_number = tok_hfloat; 
				literal.token_val = var->val; 
				output_q.push_back(literal); 
			}
		}else if(token_number==tok_operator){
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
		}else if(token_number==tok_open_p){
			op_stack.push(tmp);
		}else if(token_number==tok_close_p){
			while(!op_stack.empty()){
				if(op_stack.top().token_val=="("){
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
		}else if(prev_tok_number==tok_operator && token_number==tok_operator){
			perror("Syntax Error!");
			exit(0);
		}else if(prev_tok_number==tok_open_p && token_number==tok_operator){
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
	
	std::stack<float>parse_stack;
	for(int i = 0; i<output_q.size(); i++){
		TokenType tok = output_q[i]; 
		if(tok.token_number==tok_hfloat || tok.token_number==tok_id){
			parse_stack.push(std::stof(tok.token_val));
		}else if(tok.token_number == tok_operator){
			float rhs = parse_stack.top();
			parse_stack.pop();
			float lhs = parse_stack.top();
			parse_stack.pop();
			float ans = get_ans(lhs , tok.token_val, rhs);
			parse_stack.push(ans); 
		}
	}
	return parse_stack.top();
}

void parse_expression(std::vector<TokenType>tList){
	Environment globalenv; 
	globalenv.level = 0; 
	envList.push_back(globalenv); 
	int ctr = 0; 
	while(ctr<tList.size()){
		TokenType curtok = tList[ctr]; 
		if(curtok.token_val=="hprint"){
			ctr++;
			TokenType nextok = tList[ctr]; 
			
			if(nextok.token_val=="["){
				std::vector<TokenType>expr;
				ctr++; 
				while(nextok.token_val!="]"){
					if(ctr==tList.size()){
						perror("Syntax Error!");
						exit(-1);
					}
					nextok = tList[ctr];
					if(nextok.token_val!="]"){
						expr.push_back(nextok);
					}
					ctr++; 
				}
				ctr++; 
				float ans = parse_binary(expr); 
				std::cout<<ans<<std::endl;
			}else{
				perror("Syntax Error!");
				exit(-1);
			}
		}else if(curtok.token_val=="hfloat"){
			ctr++; 
			TokenType nextok = tList[ctr];
			std::string var_name = nextok.token_val; 
			ctr++; 
			TokenType next_next_tok = tList[ctr];
			if(next_next_tok.token_val=="="){
				std::vector<TokenType>expr;
				ctr++;
				while(nextok.token_val!=";"){
					nextok = tList[ctr];
					expr.push_back(nextok);
					if(nextok.token_val==";"){
						break; 
					}
					ctr++;
				}
				float ans = parse_binary(expr); 
				Variable *newvar = new Variable();  
				std::ostringstream ss;
				ss << ans; 
				std::string val(ss.str());
				newvar->val = val;
				newvar->type = "float"; 
				newvar->name = var_name; 
				Environment currenv = envList[curr_env_index];
				currenv.var_map.insert(std::pair<std::string , Variable*>(var_name , newvar));
				envList[curr_env_index] = currenv; 
				ctr++; 
			}
		}else if(curtok.token_number==tok_id){
			ctr++;
			TokenType nextok = tList[ctr];
			if(nextok.token_val=="="){
				std::vector<TokenType>expr; 
				ctr++; 
				while(nextok.token_val!=";"){
					nextok = tList[ctr];
					expr.push_back(nextok);
					if(nextok.token_val==";"){
						break; 
					}
					ctr++;
				}
				float ans = parse_binary(expr); 
				std::ostringstream ss;
				ss << ans; 
				std::string val(ss.str());
				envList[curr_env_index].var_map[curtok.token_val]->val = val; 
				ctr++;
			}
		}
	}
}

