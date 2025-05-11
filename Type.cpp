#include <string>
#include <iostream>
#include "lexer.cpp"

class Type : public Word {
public:
    int width;

    Type(std::string s, int tag, int w) : Word(s, tag), width(w) {}

    static Type* Int;
    static Type* Char;
    static Type* Bool;

    static bool numeric(Type* p) {
        return (p == Char || p == Int);
    }

    static Type* max(Type* p1, Type* p2) {
        if (!numeric(p1) || !numeric(p2))
            return nullptr;
        else if (p1 == Int || p2 == Int)
            return Int;
        return Char;
    }
};

Type* Type::Int = new Type("int", Tag::BASIC, 4);
Type* Type::Char = new Type("char", Tag::BASIC, 1);
Type* Type::Bool = new Type("bool", Tag::BASIC, 1);

