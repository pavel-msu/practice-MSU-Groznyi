#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
#include <sstream>
#include "lexer.hpp"
#include "operation.hpp"

using namespace std;

struct Token {
    Lex::Type type;
    int srcLine;
    std::string value;

    friend istream& operator>>(istream& in, Token& t) {
        string type;
        in >> t.value >> type >> t.srcLine;
        t.type = lexFromString(type);
        return in;
    }
};

const Token EOF_TOKEN = { Lex::END_OF_FILE, 0, "" };

Token nextToken() {
    string line;
    if (getline(cin, line)){
        stringstream ss(line);
        Token token;
        ss >> token;
        return token;
    }
    return EOF_TOKEN;
}

// struct Operation {};

class AbstractParser {
    vector<Lex::Type> supported;
    void error(Token& bad) {
        cerr << "Unexpected token: " << lexToString(bad.type) 
            << ". Expected one of ";
        for (auto& lex: supported) {
            cerr << lexToString(lex) << " ";
        }
        cerr << endl;
    }
public:
    AbstractParser(std::initializer_list<Lex::Type> list): supported(list) {};
    AbstractParser(const vector<Lex::Type>& vector): supported(vector) {};
    void parse() {
        Token next = nextToken();
        for (auto& lex: supported) {
            if (lex == next.type) {
                this->handle(next);
                return;
            }
        }
        error(next);
    }

    virtual void handle(Token& token) {};
};

struct Write: AbstractParser {
    Write() : AbstractParser({Lex::KEYWORD_WRITE}) {}
    virtual void handle(Token& token) {
        
    };
} write;

struct Line: AbstractParser {
    Line() : AbstractParser(Lex::All) {}
    virtual void handle(Token& token) {
        
    }
} line;

struct Endline: AbstractParser {
    Endline() : AbstractParser({Lex::ENDLINE}) {}
    virtual void handle(Token& token) {};
} endline;

struct Main: AbstractParser {
    Main() : AbstractParser(Lex::All) {}
    virtual void handle(Token& token) {
        if (token.type == Lex::END_OF_FILE) return;
        line.handle(token);
        endline.parse();
        this->parse();
    };
} mainParser;

struct Program: AbstractParser {
    Program() : AbstractParser(Lex::All) {}
    virtual void handle(Token& token) {
        switch(token.type) {
            case Lex::FUNCTION_BORDER: 
                this->parse();
                break;
            case Lex::ENDLINE:
                this->parse();
                break;
            default:
                mainParser.parse();
        }
    }
} program;

int main() {
    program.parse();
}