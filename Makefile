build: main.cpp parse/op_parser.cpp lex/lexer.cpp lex/token.h semantics/environment.cpp
	g++ main.cpp lex/lexer.cpp parse/op_parser.cpp semantics/meta.cpp semantics/environment.cpp -o hilden

hdebug: main.cpp parse/op_parser.cpp lex/lexer.cpp lex/token.h semantics/environment.cpp
	g++ main.cpp lex/lexer.cpp parse/op_parser.cpp semantics/meta.cpp semantics/environment.cpp -o hilden -fsanitize=address -g	

clean:
	rm -f hilden