#include "Type.cpp"

class Array : Type  {
public:
  Type of;
  int size = 1;
  Array(int sx, Type p) : Type("[]", Tag::INDEX, sz*p->width), size(sz), of(p) {}
  std::string to_string() {
    return "[" + size + "]" + of->to_string();
  }
}
    
    
