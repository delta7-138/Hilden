build: main.cpp parse/op_parser.cpp lex/lexer.cpp lex/token.h
	g++ main.cpp lex/lexer.cpp parse/op_parser.cpp -o hilden

hdebug: main.cpp parse/op_parser.cpp lex/lexer.cpp lex/token.h
	g++ main.cpp lex/lexer.cpp parse/op_parser.cpp -o hilden -fsanitize=address -g	

clean:
	rm -f hilden