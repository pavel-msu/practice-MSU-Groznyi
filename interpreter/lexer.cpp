#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include "lexer.hpp"

using namespace std;

const string BAD = "BAD";

class Node {
  private:
    Node* table[256];
  public:
    const string name;
    Node(string _name): name(_name) {};
    void link(char letter, Node* new_state) {
        this->table[letter] = new_state;
    }
    void mass_link(const char* letters, Node* new_state) {
        for (const char* letter = letters; *letter; letter++) {
            this->link(*letter, new_state);
        }
    };
    Node* process(char letter) {
        return this->table[letter];
    };
};

class Automat {
  private:
    Node* state;
    Node* nodes[30];
  public:
    Automat() {
        for (const auto lex : Lex::All) {
            nodes[+lex] = new Node(lexToString(lex));
        }
       
        nodes[+Lex::S]->mass_link("0", nodes[+Lex::FUNC_NAME]);
        nodes[+Lex::FUNC_NAME]->mass_link("0123456789", nodes[+Lex::FUNC_NAME]);

        nodes[+Lex::S]->mass_link("123456789", nodes[+Lex::VAR_NAME]);
        nodes[+Lex::VAR_NAME]->mass_link("0123456789", nodes[+Lex::VAR_NAME]);
        
        nodes[+Lex::S]->mass_link("_", nodes[+Lex::DEREFERENCE]);
        nodes[+Lex::DEREFERENCE]->mass_link("123456789", nodes[+Lex::NUMBER]);
        nodes[+Lex::NUMBER]->mass_link("0123456789", nodes[+Lex::NUMBER]);

        nodes[+Lex::S]->mass_link("'", nodes[+Lex::FUNCTION_BORDER]);
        
        nodes[+Lex::S]->mass_link("+-=<>^/*", nodes[+Lex::MATH_OP_BINARY]);
        nodes[+Lex::S]->mass_link("!", nodes[+Lex::MATH_OP_UNARY]);
        nodes[+Lex::S]->mass_link("#", nodes[+Lex::KEYWORD_IF]);
        nodes[+Lex::S]->mass_link("@", nodes[+Lex::KEYWORD_LOOP]);
        nodes[+Lex::S]->mass_link("$", nodes[+Lex::KEYWORD_RETURN]);
        nodes[+Lex::S]->mass_link("%", nodes[+Lex::KEYWORD_WRITE]);
        nodes[+Lex::S]->mass_link("~", nodes[+Lex::KEYWORD_READ]);
        nodes[+Lex::S]->mass_link("\n", nodes[+Lex::ENDLINE]);
    };
    void step(char letter) {
        this->state = this->state->process(letter);
    }
    const string& process(const char* token) {
        this->state = nodes[+Lex::S];
        for (const char* letter = token; *letter; letter++) {
            this->step(*letter);
        } 
        return this->get_state();
    }
    const string& get_state() {
        if (this->state == NULL) return BAD;
        return this->state->name;
    };
};

int main() {
    Automat automat;

    string line;
    int i = 0;
    while (getline(cin, line)) {
        stringstream ss(line);
        string token;
        i++;
        while (ss >> token) {
            cout << token << '\t'  
              << setw(20) << automat.process(token.c_str()) << '\t'
              << i << endl;
        }
        cout << "\\n\t" 
            << setw(20) << lexToString(Lex::ENDLINE) << '\t' << i << endl;
    }
}