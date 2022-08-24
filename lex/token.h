#ifndef __TOKEN_H_
#define __TOKEN_H_
/*hint hfloat hchar hstring - four datatypes 
[ ] - For opening and closing a block   
; - separator token 
*/

enum Token{
    tok_sep = -1, 
    tok_open_p = -2, 
    tok_close_p = -3, 
    tok_keyword = -4, 
    tok_operator = -5, 
    tok_undef = -6, 
    tok_hfloat = -7, 
    tok_hchar = -8, 
    tok_hstring = -9, 
    tok_hint = -10,
    tok_id = -11, 
    tok_open_b = -12, 
    tok_close_b = -13
};

#endif