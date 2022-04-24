hilden : main.cpp op_parser.cpp lexer.cpp token.h
	g++ main.cpp lexer.cpp op_parser.cpp token.h -o hilden

hdebug: main.cpp op_parser.cpp lexer.cpp token.h
	g++ main.cpp lexer.cpp op_parser.cpp token.h -o hilden -fsanitize=address -g	

clean:
	rm -f hilden