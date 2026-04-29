#include <iostream>
#include <string>
#include <vector>
#include <sstream>

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
        nodes[0] = new Node("S");
        nodes[1] = new Node("IDENTIFIER");
        nodes[2] = new Node("NUMBER");
        nodes[3] = new Node("FUNCTION_DEF");
        nodes[4] = new Node("FUNCTION_END");
        nodes[5] = new Node("MATH_OP_BINARY");
        nodes[6] = new Node("MATH_OP_UNARY");
        nodes[7] = new Node("KEYWORD_IF");
        nodes[8] = new Node("KEYWORD_LOOP");
        nodes[9] = new Node("KEYWORD_BREAK");
        nodes[10] = new Node("KEYWORD_WRITE");
        nodes[11] = new Node("KEYWORD_READ");
        nodes[12] = new Node("ENDLINE");

        nodes[0]->mass_link("0123456789", nodes[1]);
        nodes[1]->mass_link("0123456789", nodes[1]);
        
        nodes[0]->mass_link("_", nodes[2]);
        nodes[2]->mass_link("0123456789", nodes[2]);

        nodes[0]->mass_link("'", nodes[3]);
        nodes[3]->mass_link("0123456789", nodes[4]);
        nodes[4]->mass_link("0123456789", nodes[4]);

        nodes[0]->mass_link("+-=<>^/*", nodes[5]);
        nodes[0]->mass_link("!", nodes[6]);
        nodes[0]->mass_link("#", nodes[7]);
        nodes[0]->mass_link("@", nodes[8]);
        nodes[0]->mass_link("$", nodes[9]);
        nodes[0]->mass_link("%", nodes[10]);
        nodes[0]->mass_link("~", nodes[11]);
        nodes[0]->mass_link("\n", nodes[12]);
    };
    void step(char letter) {
        this->state = this->state->process(letter);
    }
    const string& process(const char* token) {
        this->state = nodes[0];
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
    vector<string> tokens;
    while (getline(cin, line)) {
        stringstream ss(line);
        string token;
        while (ss >> token) tokens.push_back(token);
        tokens.push_back("\n");
    }
    for (const string& token : tokens) {
        cout << automat.process(token.c_str());
    }
}