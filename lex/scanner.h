#ifndef __SCAN_H_
#define __SCAN_H_

#include "lexer.h"
#include <vector>
#include "token.h"
using namespace std;




vector<TokenType> gettokenlist(char * file);

#endif