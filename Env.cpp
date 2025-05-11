#include <iostream>
#include <unordered_map>

class Env {
private:
    std::unordered_map<Token, Id> table;
protected:
    Env* prev;
public:
    Env(Env* n) : prev(n) {}

    void put(Token w, Id i) {
        table[w] = i;
    }

    Id get(Token w) {
        for (Env* e = this; e != nullptr; e = e->prev) {
            auto it = e->table.find(w);
            if (it != e->table.end())
                return it->second;
        }
        return nullptr;
    }
};
