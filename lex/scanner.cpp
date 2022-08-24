#include "scanner.h"

vector<TokenType> gettokenlist(char * file)
{
    yyin = fopen(file, "r");
    int ntoken;
    vector<TokenType>tokenList;
    ntoken = yylex();
    while (ntoken)
    {
        // printf("%d %s\n", ntoken,yytext);
        TokenType t ;
        t.token_number = ntoken;
        t.token_val = yytext;
        tokenList.push_back(t);
        ntoken = yylex();
    }

    return tokenList;
}

// int main(int argc , char ** argv)
// {
//     vector<TokenType>tokenList = gettokenlist(argv[1]);

//     for (int i = 0; i < tokenList.size();i++)
//     {
//         cout << tokenList[i].token_number << " " <<  tokenList[i].token_val<< endl;
//     }

// }

