build: main.cpp parse/op_parser.cpp lex/lexer.cpp lex/token.h semantics/environment.cpp
	lex lex/lexer.l 
	g++ main.cpp lex.yy.c lex/lexer.cpp parse/op_parser.cpp semantics/environment.cpp -o hilden  -g

hdebug: main.cpp parse/op_parser.cpp lex/lexer.cpp lex/token.h semantics/environment.cpp
	lex lex/lexer.l 
	g++ main.cpp lex.yy.c lex/lexer.cpp parse/op_parser.cpp semantics/environment.cpp -o hilden -fsanitize=address -g	

clean:
	rm -f hilden
	rm lex.yy.c