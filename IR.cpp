#include "lexer.cpp"
#include <iostream>
#include <stdexcept>
#include <string>

class Node {
public:
    int lexline;

    Node() : lexline(Lexer::line) {}

    void error(const std::string &s) {
        throw std::runtime_error("near line " + std::to_string(lexline) + ": " + s);
    }

    static int labels;

    int new_label() {
        return ++labels;
    }

    void emit_label(int i) {
        std::cout << "L" << i << ":" << std::endl;
    }

    void emit(const std::string &s) {
        std::cout << "\t" << s << std::endl;
    }
};

int Node::labels = 0;

class Expr : public Node {
public:
    Token* op;
    Type* type;

    Expr(Token* tok, Type* p) : op(tok), type(p) {}

    virtual Expr* gen() {
        return this;
    }

    virtual Expr* reduce() {
        return this;
    }

    virtual void jumping(int t, int f) {
        emit_jumps(to_string(), t, f);
    }

    void emit_jumps(const std::string &test, int t, int f) {
        if (t != 0 && f != 0) {
            emit("if " + test + " goto L" + std::to_string(t));
            emit("goto L" + std::to_string(f));
        } else if (t != 0) {
            emit("if " + test + " goto L" + std::to_string(t));
        } else if (f != 0) {
            emit("iffalse " + test + " goto L" + std::to_string(f));
        }
    }

    virtual std::string to_string() {
        return op->to_string();
    }
};

class Id : public Expr {
public:
    int offset;

    Id(Word* id, Type* p, int b) : Expr(id, p), offset(b) {}
};

class Temp; 

class Op : public Expr {
public:
    Op(Token* tok, Type* p) : Expr(tok, p) {}

    Expr* reduce() override {
        Expr* x = gen();
        Temp* t = new Temp(type);
        emit(t->to_string() + " = " + x->to_string());
        return t;
    }
};

class Arith : public Op {
public:
    Expr* expr1;
    Expr* expr2;

    Arith(Token* tok, Expr* x1, Expr* x2)
        : Op(tok, nullptr), expr1(x1), expr2(x2) {
        type = Type::max(expr1->type, expr2->type);
        if (type == nullptr)
            error("type error");
    }

    Expr* gen() override {
        return new Arith(op, expr1->reduce(), expr2->reduce());
    }

    std::string to_string() override {
        return expr1->to_string() + " " + op->to_string() + " " + expr2->to_string();
    }
};
