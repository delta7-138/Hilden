# HILDEN

Hilden is a new language which implements blocked scoping written in C++.

Check out `test/` for examples

## Run

1. Run `make` from the root of this repository
2. Run an example `./hilden test/fact.hl`

## Language Design and Implementation

### Lexer
`lex/lexer.cpp`

The lexer converts the text code into a list of tokens. Uses state machines to output token type.

The output of the lexer is passed to the Parser

### Parser
`parse/op_parse.cpp`

The parser takes the ordered token list from the lexer and uses it to generate an [Abstract Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree).

The parser works recursively. Everytime an open block is encountered, the parser is called recursively, which returns a abstract syntax tree node.

When the parser encounters a binary expression, it passes on the parsing to a parse_binary which mutually recurses with the main parse function (since a binary expression may have an expression within it). The binary parser uses the [Shunting Yard Algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) for parsing.

The generated AST is passed to the Semantic Analyser

### Semantic Analysis
`semantics/environment.cpp`

The semantic analyser handles environments. Scoped declarations are local to the scope. Any declaration outside the scope is accessible inside but not vice versa.

The SA also evaluates the AST. Uses a modification of depth first tree traversal, looks at the node type, does typechecking and evaluates accrodingly.

**Pass the `--tree` flag while executing Hilden code to print out the generated AST**

