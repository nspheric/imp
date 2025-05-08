#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>

// This is a port of the lexer from the dragon book,
// second edition.
// This is part of the compiler frontend, written in
// Java, in the back of the book.

class Tag {
public:
    static const int AND = 256, BASIC = 257, BREAK = 258, DO = 259, ELSE = 260,
                     EQ = 261, FALSE = 262, GE = 263, ID = 264, IF = 265, INDEX = 266,
                     LE = 267, MINUS = 268, NE = 269, NUM = 270, OR = 271, REAL = 272,
                     TEMP = 273, TRUE = 274, WHILE = 275;
};

class Token {
public:
    int tag;
    Token(int t) : tag(t) {}
    virtual std::string to_string() const {
        return std::string(1, static_cast<char>(tag));
    }
};

class Num : public Token {
public:
    int value;
    Num(int v) : Token(Tag::NUM), value(v) {}
    std::string to_string() const override {
        return std::to_string(value);
    }
};

class Word : public Token {
public:
    std::string lexeme;
    Word(std::string s, int tag) : Token(tag), lexeme(s) {}
    std::string to_string() const override {
        return lexeme;
    }

    static Word And, Or, Eq, Ne, Le, Ge, Minus, True, False, Temp;
};

Word Word::And("&&", Tag::AND);
Word Word::Or("||", Tag::OR);
Word Word::Eq("==", Tag::EQ);
Word Word::Ne("!=", Tag::NE);
Word Word::Le("<=", Tag::LE);
Word Word::Ge(">=", Tag::GE);
Word Word::Minus("minus", Tag::MINUS);
Word Word::True("true", Tag::TRUE);
Word Word::False("false", Tag::FALSE);
Word Word::Temp("t", Tag::TEMP);

class Lexer {
public:
    int line = 1;
    char peek = ' ';
    std::unordered_map<std::string, Word*> words;

    Lexer() {
        reserve(new Word("if", Tag::IF));
        reserve(new Word("else", Tag::ELSE));
        reserve(new Word("while", Tag::WHILE));
        reserve(new Word("do", Tag::DO));
        reserve(new Word("break", Tag::BREAK));
        reserve(&Word::True);
        reserve(&Word::False);
    }

    void reserve(Word* w) {
        words[w->lexeme] = w;
    }

    void readch() {
        peek = std::cin.get();
    }

    bool readch(char c) {
        readch();
        if (peek != c) return false;
        peek = ' ';
        return true;
    }

    Token* scan() {
        for (;; readch()) {
            if (peek == ' ' || peek == '\t') continue;
            else if (peek == '\n') line++;
            else break;
        }

        switch (peek) {
        case '&':
            if (readch('&')) return &Word::And;
            return new Token('&');
        case '|':
            if (readch('|')) return &Word::Or;
            return new Token('|');
        case '=':
            if (readch('=')) return &Word::Eq;
            return new Token('=');
        case '!':
            if (readch('=')) return &Word::Ne;
            return new Token('!');
        case '<':
            if (readch('=')) return &Word::Le;
            return new Token('<');
        case '>':
            if (readch('=')) return &Word::Ge;
            return new Token('>');
        }

        if (isdigit(peek)) {
            int v = 0;
            do {
                v = 10 * v + (peek - '0');
                readch();
            } while (isdigit(peek));
            return new Num(v);
        }

        if (isalpha(peek)) {
            std::string s;
            do {
                s += peek;
                readch();
            } while (isalnum(peek));
            auto it = words.find(s);
            if (it != words.end()) return it->second;
            Word* w = new Word(s, Tag::ID);
            words[s] = w;
            return w;
        }

        Token* tok = new Token(peek);
        peek = ' ';
        return tok;
    }
};
