#include "Type.cpp"
#include <string>

class Array : public Type {
public:
    Type* of;
    int size;

    Array(int sz, Type* p)
        : Type("[]", Tag::INDEX, sz * p->width), size(sz), of(p) {}

    std::string to_string() {
        return "[" + std::to_string(size) + "]" + of->to_string();
    }
};
