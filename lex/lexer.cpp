#include "lexer.h"

std::string Identifier;  //for storing identifier names 
double NumVal; //for storing token number in case of special tokens

std::vector<TokenType> gettok(std::string source){
    std::vector<TokenType>tokenList; //Splitting the source code into words separated by spaces
    std::stringstream splitter(source);
    std::string word; 
    while(splitter>>word){
        
        if(isKeyword(word)){
            TokenType tok; 
            tok.token_number = tok_keyword;
            tok.token_val = word; 
            tokenList.push_back(tok); 
            continue;
        }
        std::string buffer = ""; 
        char c, la; 
        int len = word.length(); 
        
        for(int i = 0; i<len; i++){
            c = word.at(i);
            //std::cout<<c<<std::endl;
            //std::cout<<i<<","<<len<<std::endl; 
            if(c=='+' || c=='-' || c=='*' || c=='/' || c=='=' || c=='(' || c==')' || c==';' || c=='!' || c=='>' || c=='<' || c=='[' || c==']'){
                if(buffer!=""){
                    tokenList.push_back(getNextToken(buffer)); 
                }
                if(c=='('){
                    TokenType parentok; 
                    parentok.token_number = tok_open_p; 
                    parentok.token_val = "("; 
                    tokenList.push_back(parentok);
                }else if(c==')'){
                    TokenType parentok; 
                    parentok.token_number = tok_close_p; 
                    parentok.token_val = ")";
                    tokenList.push_back(parentok);
                }else if(c=='['){
                    TokenType blocktok; 
                    blocktok.token_number = tok_open_b;
                    blocktok.token_val = "[";
                    tokenList.push_back(blocktok); 
                }else if(c==']'){
                    TokenType blocktok; 
                    blocktok.token_number = tok_close_b;
                    blocktok.token_val = "]";
                    tokenList.push_back(blocktok); 
                }else if(c==';'){
                    TokenType septok; 
                    septok.token_number = tok_sep; 
                    septok.token_val = ";";
                    tokenList.push_back(septok);
                }else if(c=='=' || c=='!' || c=='>' || c=='<'){
                    TokenType optok; 
                    optok.token_number = tok_operator; 
                    optok.token_val = "";
                    optok.token_val+=c;
                    if(i+1<len){
                        char la = word.at(i+1);
                        if(la=='='){
                            i++;
                            optok.token_val+="=";
                        }

                    }
                    tokenList.push_back(optok);
                }else{
                    TokenType optok; 
                    optok.token_number = tok_operator; 
                    optok.token_val = std::string(1 , c);
                    tokenList.push_back(optok);
                }
                buffer = "";
            }else{
                buffer+=c; 
            }
        } //push the remaining buffer to a token
        if(buffer!=""){
            tokenList.push_back(getNextToken(buffer));
        }
    }
    return tokenList; 
}

bool isKeyword(std::string word){
    if(word == "hfloat" || word == "hchar" || word == "hstring" || word == "hprint" || word == "hwhile" || word == "hif" || word=="hint" || word=="hdec" || word=="ret"){
        return true;
    }
    return false; 
}

/*
Should only start with alphabet and succeed with alphanumeric characters only
'_' is allowed
*/
bool isIdentifier(std::string word){
    char c = word.at(0); 
    if(!isAlphabet(c)){
        return false; 
    }
    int size = word.length(); 
    for(int i = 1; i<size; i++){
        char c = word.at(i);
        if(!isAlphanumeric(c) && c!='_'){
            return false; 
        }
    }
    return true; 
}


/*
Should only start with digit
Must have a decimal point(for now)
Ex. : 3.0 2.0 4.0 1.0 1.34 , etc. are valid
      3, .34, 1. are invalid 
*/
bool isNumeric(std::string word){
    char c = word.at(0); 
    int curr_state = 0;
    int i = 0, size = word.length(); 
    while(i<size){
        char c = word.at(i);
        switch(curr_state){
            case 0: 
            if(isDigit(c)){
                curr_state++;
            }else{
                curr_state=-1;
            }
            break;
            case 1:
            if(!isDigit(c) && c!='.'){
                curr_state = -1; //error state
            }else if(c=='.'){
                curr_state++; 
            }
            break;
            case 2:
            if(isDigit(c)){
                curr_state++;
            }else{
                curr_state=-1;
            }
            break;
            case 3:
            if(!isDigit(c)){
                curr_state = -1; 
            }
            break;
        }
        i++;
    }
    if(curr_state==3){
        return true;
    }
    return false;
}

/*TBD*/
bool isStringLiteral(std::string word){
    char c = word.at(0);
    if(c!='"'){
        return false; 
    }
    return true; 
}

bool isCharLiteral(std::string word){
    char c = word.at(0);
    int curr_state = 0; 
    int i = 0, size = word.length(); 
    while(i<size){
        char c = word.at(i);
        switch(curr_state){
            case 0:
            if(c=='\''){
                curr_state++;
            }
            break;
            case 1:
            curr_state++; 
            break;
            case 2:
            if(c=='\''){
                curr_state++;
            }else{
                curr_state=4; 
            }
            break;
            case 3:
            curr_state++; 
            break;
        }
        i++;
    }
    if(curr_state==3){
        return true;
    }
    return false;
}

bool isAlphabet(char c){
    if(c>='A' && c<='Z' || c>='a' && c<='z'){
        return true;
    }
    return false; 
}

bool isDigit(char c){
    if(c>='0' && c<='9'){
        return true;
    }
    return false; 
}

bool isAlphanumeric(char c){
    return (isDigit(c) || isAlphabet(c));
}

TokenType getNextToken(std::string buffer){
    TokenType tok;
    tok.token_val = buffer; 
    if(isIdentifier(buffer)){
        tok.token_number = tok_id;
    }else if(isNumeric(buffer)){
        tok.token_number = tok_hfloat;
    }else if(isStringLiteral(buffer)){
        tok.token_number = tok_hstring; 
    }else if(isCharLiteral(buffer)){
        tok.token_number = tok_hchar; 
    }else{
        tok.token_number = tok_undef; 
    }
    return tok; 
}

