#include <iostream>
#include <unordered_map>

class Env {
private:
  std::unordered_map<Token, Id> table;
protected:
  Env prev;
public:
  Env(Env n) : table(), prev(n) {}
  void put(Token w, Id i) {
    table[w] = i;
  }
  Id get(Token w) {
    for (Env e = this; e != nullptr; e = e->prev) {
      Id found =  e->table[w];
      if (found != nullptr) return found;
    }
    return nullptr;
  }
};
