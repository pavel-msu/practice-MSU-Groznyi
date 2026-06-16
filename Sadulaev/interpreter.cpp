#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <set>
#include <map>

using namespace std;

enum class TokenType {
    PROGRAM, VAR, BEGIN, END, INT, BOOL,
    IF, THEN, ELSE, WHILE, DO, READ, WRITE,
    IDENTIFIER, NUMBER,
    ASSIGN, PLUS, MINUS, MULTIPLY, DIVIDE,
    EQ, LSS, GTR, LEQ, GEQ, NEQ,
    SEMICOLON, COMMA, COLON, LPAREN, RPAREN,
    END_OF_FILE, UNKNOWN, UPL, BP,
    LBRACKET, RBRACKET, ARR_GET, ARR_SET, ALLOC_ARR
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
};

class Lexer {
private:
    string code;
    int n = 0;
    int line = 1;   
    int col = 1;     


    char getChar() {
        if (n >= code.length()) return '\0';
        return code[n];
    }

    void nextChar() {
        if (n >= code.length()) return;
        if (code[n] == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
        n++;
    }

    void skipSpace() {
        while (n < code.length() && isspace(getChar())) {
            nextChar();
        }
    }

public:
    Lexer(const string& text) {
        code = text;
    }

    vector<Token> tokenize() {
        vector<Token> tokens;

        while (n < code.length()) {
            skipSpace();
            if (n >= code.length()) break;

            char c = getChar();
            int startLine = line;
            int startCol = col;

            if (isalpha(c)) {
                string word = ""; 
                while (n < code.length() && isalnum(getChar())) {
                    word += getChar();
                    nextChar();
                }
                
                TokenType type = TokenType::IDENTIFIER;
                
                if (word == "program") type = TokenType::PROGRAM;
                else if (word == "var") type = TokenType::VAR;
                else if (word == "begin") type = TokenType::BEGIN;
                else if (word == "end") type = TokenType::END;
                else if (word == "int") type = TokenType::INT; 
                else if (word == "if") type = TokenType::IF;
                else if (word == "then") type = TokenType::THEN;
                else if (word == "else") type = TokenType::ELSE;
                else if (word == "while") type = TokenType::WHILE;
                else if (word == "do") type = TokenType::DO;
                else if (word == "read") type = TokenType::READ;
                else if (word == "write") type = TokenType::WRITE;

                tokens.push_back({type, word, startLine, startCol});
            }
            else if (c == '<') {
                nextChar();
                if (getChar() == '=') {
                    tokens.push_back({TokenType::LEQ, "<=", startLine, startCol});
                    nextChar();
                } else if (getChar() == '>') {
                    tokens.push_back({TokenType::NEQ, "<>", startLine, startCol});
                    nextChar();
                } else {
                    tokens.push_back({TokenType::LSS, "<", startLine, startCol});
                }
            }
            else if (c == '>') {
                nextChar();
                if (getChar() == '=') {
                    tokens.push_back({TokenType::GEQ, ">=", startLine, startCol});
                    nextChar();
                } else {
                    tokens.push_back({TokenType::GTR, ">", startLine, startCol});
                }
            }
            else if (c == ':') {
                nextChar();
                if (getChar() == '=') {
                    tokens.push_back({TokenType::ASSIGN, ":=", startLine, startCol});
                    nextChar();
                } else {
                    tokens.push_back({TokenType::COLON, ":", startLine, startCol});
                }
            }
            else if (isdigit(c)) {
                string num = ""; 
                while (n < code.length() && isdigit(getChar())) {
                    num += getChar();
                    nextChar();
                }
                tokens.push_back({TokenType::NUMBER, num, startLine, startCol});
            }
            else if (c == '=') { tokens.push_back({TokenType::EQ, "=", startLine, startCol}); nextChar(); }
            else if (c == '+') { tokens.push_back({TokenType::PLUS, "+", startLine, startCol}); nextChar(); }
            else if (c == '-') { tokens.push_back({TokenType::MINUS, "-", startLine, startCol}); nextChar(); }
            else if (c == '*') { tokens.push_back({TokenType::MULTIPLY, "*", startLine, startCol}); nextChar(); }
            else if (c == '/') { tokens.push_back({TokenType::DIVIDE, "/", startLine, startCol}); nextChar(); }
            else if (c == '(') { tokens.push_back({TokenType::LPAREN, "(", startLine, startCol}); nextChar(); }
            else if (c == ')') { tokens.push_back({TokenType::RPAREN, ")", startLine, startCol}); nextChar(); }
            else if (c == ';') { tokens.push_back({TokenType::SEMICOLON, ";", startLine, startCol}); nextChar(); }
            else if (c == '[') { tokens.push_back({TokenType::LBRACKET, "[", startLine, startCol}); nextChar(); }
            else if (c == ']') { tokens.push_back({TokenType::RBRACKET, "]", startLine, startCol}); nextChar(); }
            else {
                throw runtime_error("Lexical error at line " + to_string(startLine));
            }
        }
        
        tokens.push_back({TokenType::END_OF_FILE, "", line, col});
        return tokens;
    }
};

class Parser {
private:
    vector<Token> tokens;
    int pos = 0;           
    set<string> vars;
    vector<Token> poliz; 

    Token get_token() {
        if (pos >= tokens.size()) return tokens.back();
        return tokens[pos];
    }

    void next_token() {
        if (pos < tokens.size()) pos++;
    }

    void proverka(TokenType expected) {
        if (get_token().type == expected) {
            next_token();
        } else {
            throw runtime_error("Syntax error at line " + to_string(get_token().line) + 
                                     ": unexpected token '" + get_token().value + "'");
        }
    }

public:
    Parser(const vector<Token>& t) {
        tokens = t;
    }

    vector<Token> getPoliz() const {
        return poliz;
    }

    void parse() {
        parseProgram();
        proverka(TokenType::END_OF_FILE);
    }

    void printPoliz() {
        for (const auto& token : poliz) {
            cout << token.value << " ";
        }
    }

private:

    void parseProgram() {
        proverka(TokenType::PROGRAM);
        proverka(TokenType::IDENTIFIER);
        proverka(TokenType::SEMICOLON);
        
        parseVar();
        parseBlock();
    }

    void parseVar() {
        if (get_token().type == TokenType::VAR) { 
                    proverka(TokenType::VAR); 

                    while (get_token().type == TokenType::IDENTIFIER) { 
                        Token name_t = get_token();
                        string name = name_t.value;
                        vars.insert(name);                        
                        proverka(TokenType::IDENTIFIER);
                        if (get_token().type == TokenType::LBRACKET) {  
                            proverka(TokenType::LBRACKET);
                            Token size_t = get_token();
                            proverka(TokenType::NUMBER);
                            proverka(TokenType::RBRACKET);
                            
                            poliz.push_back(name_t);
                            poliz.push_back(size_t);
                            poliz.push_back({TokenType::ALLOC_ARR, "ALLOC", 0, 0});
                        } 

                        proverka(TokenType::COLON);
                        proverka(TokenType::INT);
                        proverka(TokenType::SEMICOLON);
                    }
                }
    }

    void parseBlock() {
        proverka(TokenType::BEGIN);
        parseAllCommands();
        proverka(TokenType::END);
    }

    void parseAllCommands() {
        while (get_token().type != TokenType::END && get_token().type != TokenType::END_OF_FILE) {
            parseOneCommand();
            if (get_token().type == TokenType::SEMICOLON) {
                proverka(TokenType::SEMICOLON);
            }
        }
    }

    void parseOneCommand() {
        TokenType t = get_token().type;

        if (t == TokenType::WRITE) parseWrite();
        else if (t == TokenType::READ) parseRead();
        else if (t == TokenType::IDENTIFIER) parseAssignment();
        else if (t == TokenType::WHILE) parseWhile();
        else if (t == TokenType::IF) parseIf();
        else if (t == TokenType::BEGIN) parseBlock();
        else {
            throw runtime_error("Syntax error at line " + to_string(get_token().line) + 
                                     ": unexpected token '" + get_token().value + "'");
        }
    }

    void parseAssignment() {
        Token var_token = get_token(); 
        proverka(TokenType::IDENTIFIER);

        bool is_array = false;
        if (get_token().type == TokenType::LBRACKET) { //если это массив
            is_array = true;
            proverka(TokenType::LBRACKET);
            poliz.push_back(var_token);
            parseExpression();       
            proverka(TokenType::RBRACKET);
        } else {
            poliz.push_back(var_token);
        }

        Token assignOp = get_token(); 
        proverka(TokenType::ASSIGN);

        parseExpression(); //справа от :=


        if (is_array) poliz.push_back({TokenType::ARR_SET, "[]=", 0, 0});
        else poliz.push_back(assignOp);
    }

    void parseIf() {
        proverka(TokenType::IF);
        parseCondition();

        int p1 = poliz.size(); 
        poliz.push_back({TokenType::NUMBER, "0", 0, 0}); 
        poliz.push_back({TokenType::UPL, "UPL", 0, 0});

        proverka(TokenType::THEN);
        parseOneCommand(); 

        int p2 = poliz.size(); 
        poliz.push_back({TokenType::NUMBER, "0", 0, 0});
        poliz.push_back({TokenType::BP, "BP", 0, 0});

        poliz[p1].value = to_string(poliz.size());

        if (get_token().type == TokenType::ELSE) {
            proverka(TokenType::ELSE);
            parseOneCommand();
        }

        poliz[p2].value = to_string(poliz.size());
    }

    void parseWhile() {
        proverka(TokenType::WHILE);
        
        int start = poliz.size(); 
        parseCondition();

        int end_jump = poliz.size(); 
        poliz.push_back({TokenType::NUMBER, "0", 0, 0});
        poliz.push_back({TokenType::UPL, "UPL", 0, 0});

        proverka(TokenType::DO);
        parseOneCommand();

        // Прыжок обратно к условию
        poliz.push_back({TokenType::NUMBER, to_string(start), 0, 0});
        poliz.push_back({TokenType::BP, "BP", 0, 0});

        // Заполняем адрес выхода
        poliz[end_jump].value = to_string(poliz.size());
    }

    void parseCondition() {
        parseExpression(); 
        
        Token op = get_token();
        TokenType t = op.type;
        
        if (t == TokenType::EQ || t == TokenType::LSS || t == TokenType::GTR || 
            t == TokenType::LEQ || t == TokenType::GEQ || t == TokenType::NEQ) {
            next_token();         
            parseExpression(); 
            poliz.push_back(op); 
        } else {
            throw runtime_error("Error: expected <, >, =, <=, >=, <>");
        }
    }

    void parseExpression() {
        parseMULTDIV(); 
        while (get_token().type == TokenType::PLUS || get_token().type == TokenType::MINUS) {
            Token op = get_token(); 
            next_token();
            parseMULTDIV();          
            poliz.push_back(op);  
        }
    }

    void parseMULTDIV() {
        parseItem();
        while (get_token().type == TokenType::MULTIPLY || get_token().type == TokenType::DIVIDE) {
            Token op = get_token(); 
            next_token();
            parseItem();
            poliz.push_back(op);  
        }
    }

    void parseItem() {
            Token t = get_token();
            
            if (t.type == TokenType::NUMBER) {
                poliz.push_back(t);
                proverka(TokenType::NUMBER);
            } 
            else if (t.type == TokenType::IDENTIFIER) {
                proverka(TokenType::IDENTIFIER);
                
                //если идет скобка
                if (get_token().type == TokenType::LBRACKET) {
                    proverka(TokenType::LBRACKET);
                    poliz.push_back(t); 
                    parseExpression();      
                    proverka(TokenType::RBRACKET);
                    poliz.push_back({TokenType::ARR_GET, "[]", 0, 0}); 
                } else {
                    poliz.push_back(t); 
                }
            } 
            else if (t.type == TokenType::LPAREN) {
                proverka(TokenType::LPAREN);
                parseExpression();
                proverka(TokenType::RPAREN);
            } 
        }

    void parseRead() {
        proverka(TokenType::READ);
        proverka(TokenType::LPAREN);

        if (get_token().type == TokenType::IDENTIFIER) {
            if (!vars.count(get_token().value)) {
                throw runtime_error("Error: variable '" + get_token().value + "' not declared");
            }
            poliz.push_back(get_token()); 
            proverka(TokenType::IDENTIFIER);
        } else {
            proverka(TokenType::IDENTIFIER); 
        }

        proverka(TokenType::RPAREN);
        proverka(TokenType::SEMICOLON);

        poliz.push_back({TokenType::READ, "READ", 0, 0});
    }

    void parseWrite() {
        proverka(TokenType::WRITE);
        proverka(TokenType::LPAREN);
        parseExpression(); 
        proverka(TokenType::RPAREN);
        proverka(TokenType::SEMICOLON);
        poliz.push_back({TokenType::WRITE, "WRITE", 0, 0});
    }
};

class Memory {
private:
    map<string, int> vars;
    map<string, vector<int>> arrays; 

public:
    void add_var(const string& name) { vars[name] = 0; }
    void set_val(const string& name, int val) { vars[name] = val; }
    int get_val(const string& name) { return vars[name]; }


    void alloc_arr(const string& name, int size) {
        arrays[name].resize(size, 0);
    }

    void set_arr_val(const string& name, int index, int val) {
        if (index < 0 || index >= arrays[name].size()) 
            throw runtime_error("Index out of bounds: " + name + "[" + to_string(index) + "]");
        arrays[name][index] = val;
    }

    int get_arr_val(const string& name, int index) {
        if (index < 0 || index >= arrays[name].size()) 
            throw runtime_error("Index out of bounds: " + name + "[" + to_string(index) + "]");
        return arrays[name][index];
    }
};

class Interpretator {
private:
    vector<Token> poliz;
    Memory mem;

    int to_int(Token t) {
        if (t.type == TokenType::NUMBER) return stoi(t.value);
        if (t.type == TokenType::IDENTIFIER) return mem.get_val(t.value);
        throw runtime_error("Error: expected number or variable");
    }

public:
    Interpretator(const vector<Token>& p) {
        poliz = p;
    }

    void run() {
        vector<Token> stack;
        int i = 0; 

        while (i < poliz.size()) {
            Token t = poliz[i];
            TokenType type = t.type;

            if (type == TokenType::NUMBER || type == TokenType::IDENTIFIER) {
                stack.push_back(t);
                i++;
            } 
            else if (type == TokenType::LSS || type == TokenType::GTR || type == TokenType::EQ ||
                     type == TokenType::LEQ || type == TokenType::GEQ || type == TokenType::NEQ) {
                
                int b = to_int(stack.back()); stack.pop_back();
                int a = to_int(stack.back()); stack.pop_back();
                int res = 0;
                
                if (type == TokenType::LSS) res = (a < b);
                else if (type == TokenType::GTR) res = (a > b);
                else if (type == TokenType::EQ)  res = (a == b);
                else if (type == TokenType::LEQ) res = (a <= b);
                else if (type == TokenType::GEQ) res = (a >= b);
                else if (type == TokenType::NEQ) res = (a != b);
                
                stack.push_back({TokenType::NUMBER, to_string(res), 0, 0});
                i++;
            }
            //Условный переход
            else if (type == TokenType::UPL) {
                int jump_to = to_int(stack.back()); stack.pop_back();
                int cond = to_int(stack.back()); stack.pop_back();
                
                if (cond == 0) i = jump_to; 
                else i++;                   
            }
            //Безусловный переход
            else if (type == TokenType::BP) {
                int jump_to = to_int(stack.back()); stack.pop_back();
                i = jump_to;
            }
            else if (type == TokenType::PLUS || type == TokenType::MINUS || 
                     type == TokenType::MULTIPLY || type == TokenType::DIVIDE) {
                
                int b = to_int(stack.back()); stack.pop_back();
                int a = to_int(stack.back()); stack.pop_back();
                int res = 0;
                
                if (type == TokenType::PLUS) res = a + b;
                else if (type == TokenType::MINUS) res = a - b;
                else if (type == TokenType::MULTIPLY) res = a * b;
                else if (type == TokenType::DIVIDE) {
                    if (b == 0) throw runtime_error("Error: division by zero");
                    res = a / b; 
                }

                stack.push_back({TokenType::NUMBER, to_string(res), 0, 0});
                i++;
            }
            else if (type == TokenType::ASSIGN) {
                int val = to_int(stack.back()); stack.pop_back();
                string var_name = stack.back().value; stack.pop_back();
                
                mem.set_val(var_name, val);
                i++;
            }
            else if (type == TokenType::READ) {
                string var_name = stack.back().value; stack.pop_back();
                
                int val;
                cout << var_name << " = "; 
                cin >> val;
                
                mem.set_val(var_name, val);
                i++;
            }
            else if (type == TokenType::WRITE) {
                int val = to_int(stack.back()); stack.pop_back();
                cout << val << endl;
                i++;
            }
            else if (type == TokenType::ALLOC_ARR) {
                int size = to_int(stack.back()); stack.pop_back();
                string arr_name = stack.back().value; stack.pop_back();
                mem.alloc_arr(arr_name, size);
                i++;
            }
            // чтение массива
            else if (type == TokenType::ARR_GET) {
                int index = to_int(stack.back()); stack.pop_back();
                string arr_name = stack.back().value; stack.pop_back();
                
                int val = mem.get_arr_val(arr_name, index);
                stack.push_back({TokenType::NUMBER, to_string(val), 0, 0});
                i++;
            }
            //запись в массив
            else if (type == TokenType::ARR_SET) {
                int val = to_int(stack.back()); stack.pop_back();   // что записываем
                int index = to_int(stack.back()); stack.pop_back(); // по какому индексу
                string arr_name = stack.back().value; stack.pop_back(); // куда записываем
                
                mem.set_arr_val(arr_name, index, val);
                i++;
            }
        }
    }
};

string read_file(const string& name) {
    ifstream f(name);

    string text = "";
    string line;
    while (getline(f, line)) {
        text += line + "\n";
    }
    return text;
}

int main(int argc, char* argv[]) {
    string mode = argv[1];
    string filename = argv[2];

    try {
        string code = read_file(filename);

        if (mode == "-l") {
            Lexer lex(code);
            auto tokens = lex.tokenize();
            for (auto t : tokens) {
                cout << t.value;
            }
        }
        else if (mode == "-p") {
            Lexer lex(code);
            Parser p(lex.tokenize());
            p.parse();
            p.printPoliz();
        }
        else if (mode == "-i") {
            Lexer lex(code);
            Parser p(lex.tokenize());
            p.parse();
            
            Interpretator exec(p.getPoliz());
            exec.run();
        }
    } 
    catch (const exception& e) {
        cout << e.what() << "\n";
    }

    return 0;
}
