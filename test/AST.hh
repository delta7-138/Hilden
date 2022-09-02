#include <string>
#include <vector>
#include <memory>
#include "Number.hh"

class ExpressionAST {
public:
    virtual ~ExpressionAST() {}
};  

class VariableAST : ExpressionAST {
private:
    std::string name;

public:
    VariableAST(const std::string& name): name(name) {}
};

class FunctionCallAST : ExpressionAST {
private:
    std::string name;
    std::vector<std::unique_ptr<ExpressionAST>> args;
    ValueType ret_type;

public:
    FunctionCallAST(const std::string& name, std::vector<std::unique_ptr<ExpressionAST>> args, ValueType ret_type)
    : name(name), args(std::move(args)), ret_type(ret_type) {}
};

class LiteralAST : ExpressionAST {
private:
    ValueType type;
    std::unique_ptr<Number> value;

public:
    LiteralAST(std::unique_ptr<Number> value)
    : value(std::move(value)), type(value->get_type()) {}

    const Number* get_value() {
        return value.get();
    }
};


