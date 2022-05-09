#include "lexer.h"

Lexer::Lexer(std::string source){
    this->source = source; 
}

std::vector<TokenType> Lexer::gettok(){
     //Splitting the source code into words separated by spaces
    std::stringstream splitter(source);
    std::string word; 

    while(splitter>>word){
        std::string buffer = ""; 
        char c, la; 
        int len = word.length(); 
        
        for(int i = 0; i<len; i++){
            c = word.at(i);
            //std::cout<<c<<std::endl;
            //std::cout<<i<<","<<len<<std::endl; 
            if(c=='+' || c=='-' || c=='*' || c=='/' || c=='=' || c=='(' || c==')' || c==';' || c=='!' || c=='>' || c=='<' || c=='[' || c==']' || c=='{' || c == '}' || c == ','){
                if(buffer!=""){
                    tokenList.push_back(getNextToken(buffer)); 
                }
                TokenType pushtok; 
                switch(c){
                    case '(': 
                    pushtok.token_number = tok_open_p; 
                    pushtok.token_val = "("; 
                    break; 

                    case ')': 
                    pushtok.token_number = tok_close_p; 
                    pushtok.token_val = ")";
                    break; 

                    case '[':
                    pushtok.token_number = tok_open_b;
                    pushtok.token_val = "[";
                    break; 

                    case ']':
                    pushtok.token_number = tok_close_b;
                    pushtok.token_val = "]";
                    break; 

                    case '{':
                    pushtok.token_number = tok_open_c; 
                    pushtok.token_val = "{"; 
                    break; 

                    case '}': 
                    pushtok.token_number = tok_close_c; 
                    pushtok.token_val = "}"; 
                    break; 

                    case ';': 
                    pushtok.token_number = tok_sep; 
                    pushtok.token_val = ";";
                    break;

                    case ',': 
                    pushtok.token_number = tok_coma; 
                    pushtok.token_val = ","; 
                    break; 

                    case '=':
                    case '!': 
                    case '>': 
                    case '<': 
                    pushtok.token_number = tok_operator; 
                    pushtok.token_val = "";
                    pushtok.token_val+=c;
                    if(i+1<len){
                        char la = word.at(i+1);
                        if(la=='='){
                            i++;
                            pushtok.token_val+="=";
                        }

                    }
                    break; 

                    default: 
                    pushtok.token_number = tok_operator; 
                    pushtok.token_val = std::string(1 , c);
                }
                tokenList.push_back(pushtok);
                buffer = "";
            }else{
                buffer+=c; 
            }
        } //push the remaining buffer to a token id+67
        if(buffer!=""){
            tokenList.push_back(getNextToken(buffer));
        }
    }
    return tokenList; 
}

bool Lexer::isKeyword(std::string word){
    if(word == "hfloat" || word == "hchar" || word == "hstring" || word == "hprint" || word == "hwhile" || word == "hif" || word == "hint" || word == "hdec" || word == "ret" || word == "helse"){
        return true;
    }
    return false; 
}

/*
Should only start with alphabet and succeed with alphanumeric characters only
'_' is allowed
*/
bool Lexer::isIdentifier(std::string word){
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
bool Lexer::isIntegerLiteral(std::string word){
    char c = word.at(0); 
    int curr_state = 0; 
    int i = 0 , size = word.length(); 
    while(i<size){
        char c = word.at(i); 
        switch(curr_state){
            case 0: 
            if(!isDigit(c)){
                curr_state = -1; //forever in dead state 
            }
            break; 
        }
        i++; 
    }
    if(curr_state==-1){
        return false; 
    }
    return true; 
}
bool Lexer::isFloatingLiteral(std::string word){
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
bool Lexer::isStringLiteral(std::string word){
    char c = word.at(0);
    if(c!='"'){
        return false; 
    }
    return true; 
}

bool Lexer::isCharLiteral(std::string word){
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

bool Lexer::isAlphabet(char c){
    if(c>='A' && c<='Z' || c>='a' && c<='z'){
        return true;
    }
    return false; 
}

bool Lexer::isDigit(char c){
    if(c>='0' && c<='9'){
        return true;
    }
    return false; 
}

bool Lexer::isAlphanumeric(char c){
    return (isDigit(c) || isAlphabet(c));
}

TokenType Lexer::getNextToken(std::string buffer){
    TokenType tok;
    tok.token_val = buffer; 
    
    if(isKeyword(buffer)){
        tok.token_number = tok_keyword; 
    }else if(isIdentifier(buffer)){
        tok.token_number = tok_id;
    }else if(isIntegerLiteral(buffer)){
        tok.token_number = tok_hint;
    }else if(isFloatingLiteral(buffer)){
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

void Lexer::print_token_list(){
    for(int i = 0; i<tokenList.size(); i++){
        tokenList[i].print(); 
        std::cout<<std::endl; 
    }
}
