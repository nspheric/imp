#include <string>
#include <iostream>
#include "lexer.cpp"

class Type : public Word {
public:
  int width = 0;
  Type(std::string s, int tag, int w) : Word(s, tag), width(w) {}
  static const Type Int = new Type("int", Tag::BASIC, 4),
    Char = new Type("char", Tag::BASIC, 1),
    Bool = new Type("bool", Tag::BASIC, 1);
  static bool numeric(Type p) {
    if (p == Type.Char || p == Type.Int)
      return true;
    return false;
  }
  static Type max(Type p1,  Type p2) {
    if (!this->numeric(p1) || !this->numeric(p2))
      return nullptr;
    else if (p1 == Type.Int || p2 == Type.Int)
      return Type.Int;
    return Type.Char;
  }
