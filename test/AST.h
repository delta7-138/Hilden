#include <string>
#include <memory>

enum Type{ 
    INTEGER, 
    FLOAT,
    CHAR,
    STRING
};

class ExprAST {
public:
    virtual ~ExprAST() {}
};

class NumberExprAST : public ExprAST {
private:
    Type type;

public:
    NumberExprAST(void) = default;
    NumberExprAST(Type type)
    :type(type) {}
    Type getType(void) {
        return type;
    }
};

class FloatExprAST : public NumberExprAST {
private:
    float data;
public:
    FloatExprAST(float data)
    : NumberExprAST(FLOAT), data(data) {

    }
    float getData() const {
        return data;
    }
};

class IntExprAST: public NumberExprAST {
private:
    int data;
public:
    IntExprAST(int data)
    : NumberExprAST(INTEGER), data(data) {

    }
    int getData() const {
        return data;
    }
};

class CharExprAST : public NumberExprAST {
private:
    char data;
public:
    CharExprAST(char data)
    : NumberExprAST(CHAR), data(data) {

    }
    char getData() const {
        return data;
    }
};

class StringExprAST : public NumberExprAST {
private:
    std::string data;
public:
    StringExprAST(const std::string& data)
    : NumberExprAST(STRING), data(data) {

    }
    const std::string& getData() const {
        return data;
    }
};


class VariableExprAST : public ExprAST {
private:
    Type type;
    std::string name;
public:
    VariableExprAST(const std::string &name) : name(name) {}
};

class BinaryExprAST : public ExprAST {
private:
    char op;
    std::unique_ptr<ExprAST> LHS, RHS;
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
    : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};