#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <map>
#include <climits>

using namespace std;

int my_isdigit(int c) {
    return (c >= '0' && c <= '9');
}

int my_isalpha(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int my_isalnum(int c) {
    return my_isdigit(c) || my_isalpha(c);
}

//                       ТОКЕНЫ
enum TokenType {
    TOK_EOF,
    TOK_NUMBER,
    TOK_IDENT,
    TOK_KEYWORD_IF,
    TOK_KEYWORD_THEN,
    TOK_KEYWORD_WHILE,
    TOK_KEYWORD_DO,
    TOK_KEYWORD_SET,
    TOK_KEYWORD_PRINT,
    TOK_KEYWORD_PRINTC,
    TOK_KEYWORD_PRINTSTR,
    TOK_KEYWORD_READ,
    TOK_KEYWORD_READC,
    TOK_KEYWORD_DIM,
    TOK_KEYWORD_EXIT,
    TOK_KEYWORD_SHOWVARS,
    TOK_KEYWORD_GOTO,
    TOK_KEYWORD_LABEL,
    TOK_OPERATOR_PLUS,
    TOK_OPERATOR_MINUS,
    TOK_OPERATOR_MUL,
    TOK_OPERATOR_DIV,
    TOK_OPERATOR_EQ,
    TOK_OPERATOR_NE,
    TOK_OPERATOR_LT,
    TOK_OPERATOR_GT,
    TOK_OPERATOR_LE,
    TOK_OPERATOR_GE,
    TOK_OPERATOR_ASSIGN,
    TOK_SEMICOLON,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_UNKNOWN
};

struct Token {
    TokenType type;
    string   lexeme;
    int      line;
    int      value;
};

struct AddToken {
    vector<Token>& tokens;
    int& line;

    AddToken(vector<Token>& t, int& l) : tokens(t), line(l) {}

    void operator() (TokenType t, const string& lex, int val = 0) const {
        Token tok;
        tok.type = t;
        tok.lexeme = lex;
        tok.line = line;
        tok.value = val;
        tokens.push_back(tok);
    }
};

//                     ЛЕКСЕР
vector<Token> lexer(FILE* file) {
    vector<Token> tokens;
    int line = 1;
    int c;
    char buf[256];
    int buf_len = 0;

    AddToken add_token(tokens, line);

    while ((c = fgetc(file)) != EOF) {
        if (c == ' ' || c == '\t') continue;
        if (c == '#') {
            while ((c = fgetc(file)) != EOF && c != '\n') {}
            if (c == '\n') line++;
            continue;
        }
        if (c == '\n') {
            line++;
            continue;
        }
        if (my_isalpha(c) || c == '_') {
            buf_len = 0;
            buf[buf_len++] = c;
            while ((c = fgetc(file)) != EOF && (my_isalnum(c) || c == '_')) {
                buf[buf_len++] = c;
            }
            buf[buf_len] = '\0';
            ungetc(c, file);
            string lex(buf);
            if (lex == "if")            add_token(TOK_KEYWORD_IF, lex);
            else if (lex == "then")     add_token(TOK_KEYWORD_THEN, lex);
            else if (lex == "while")    add_token(TOK_KEYWORD_WHILE, lex);
            else if (lex == "do")       add_token(TOK_KEYWORD_DO, lex);
            else if (lex == "set")      add_token(TOK_KEYWORD_SET, lex);
            else if (lex == "print")    add_token(TOK_KEYWORD_PRINT, lex);
            else if (lex == "printc")   add_token(TOK_KEYWORD_PRINTC, lex);
            else if (lex == "printstr") add_token(TOK_KEYWORD_PRINTSTR, lex);
            else if (lex == "read")     add_token(TOK_KEYWORD_READ, lex);
            else if (lex == "readc")    add_token(TOK_KEYWORD_READC, lex);
            else if (lex == "dim")      add_token(TOK_KEYWORD_DIM, lex);
            else if (lex == "exit")     add_token(TOK_KEYWORD_EXIT, lex);
            else if (lex == "showvars") add_token(TOK_KEYWORD_SHOWVARS, lex);
            else if (lex == "goto")     add_token(TOK_KEYWORD_GOTO, lex);
            else if (lex == "label")    add_token(TOK_KEYWORD_LABEL, lex);
            else                        add_token(TOK_IDENT, lex);
            continue;
        }
        if (my_isdigit(c) || c == '-') {
            if (c == '-') {
                int next_c = fgetc(file);
                if (next_c == EOF || !my_isdigit(next_c)) {
                    if (next_c != EOF) ungetc(next_c, file);
                    add_token(TOK_OPERATOR_MINUS, "-");
                    continue;
                }
                ungetc(next_c, file);
            }
            buf_len = 0;
            buf[buf_len++] = c;
            while ((c = fgetc(file)) != EOF && my_isdigit(c)) {
                buf[buf_len++] = c;
            }
            buf[buf_len] = '\0';
            if (c != EOF) ungetc(c, file);
            int val = atoi(buf);
            add_token(TOK_NUMBER, buf, val);
            continue;
        }
        switch (c) {
            case '+': add_token(TOK_OPERATOR_PLUS, "+"); break;
            case '-': add_token(TOK_OPERATOR_MINUS, "-"); break;
            case '*': add_token(TOK_OPERATOR_MUL, "*"); break;
            case '/': add_token(TOK_OPERATOR_DIV, "/"); break;
            case ';': add_token(TOK_SEMICOLON, ";"); break;
            case '[': add_token(TOK_LBRACKET, "["); break;
            case ']': add_token(TOK_RBRACKET, "]"); break;
            case '=':
                if ((c = fgetc(file)) == '=') {
                    add_token(TOK_OPERATOR_EQ, "==");
                } else {
                    ungetc(c, file);
                    add_token(TOK_OPERATOR_ASSIGN, "=");
                }
                break;
            case '!':
                if ((c = fgetc(file)) == '=') {
                    add_token(TOK_OPERATOR_NE, "!=");
                } else {
                    ungetc(c, file);
                    add_token(TOK_UNKNOWN, "!");
                }
                break;
            case '<':
                if ((c = fgetc(file)) == '=') {
                    add_token(TOK_OPERATOR_LE, "<=");
                } else {
                    ungetc(c, file);
                    add_token(TOK_OPERATOR_LT, "<");
                }
                break;
            case '>':
                if ((c = fgetc(file)) == '=') {
                    add_token(TOK_OPERATOR_GE, ">=");
                } else {
                    ungetc(c, file);
                    add_token(TOK_OPERATOR_GT, ">");
                }
                break;
            default:
                add_token(TOK_UNKNOWN, string(1, c));
                break;
        }
    }
    add_token(TOK_EOF, "");
    return tokens;
}

//                       AST
enum ExprType {
    EXPR_NUMBER,
    EXPR_VARIABLE,
    EXPR_ARRAY_ACCESS,
    EXPR_BINARY_OP
};

enum BinOp {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV,
    OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE
};

struct Expr {
    ExprType type;
    int line;
    union {
        int num_val;
        string* var_name;
        struct {
            string* arr_name;
            Expr*   index;
        } array_acc;
        struct {
            BinOp op;
            Expr* left;
            Expr* right;
        } binary;
    } data;

    static Expr* make_number(int val) {
        Expr* e = new Expr;
        e->type = EXPR_NUMBER;
        e->line = 0;
        e->data.num_val = val;
        return e;
    }
    static Expr* make_variable(const string& name) {
        Expr* e = new Expr;
        e->type = EXPR_VARIABLE;
        e->line = 0;
        e->data.var_name = new string(name);
        return e;
    }
    static Expr* make_array_access(const string& name, Expr* idx) {
        Expr* e = new Expr;
        e->type = EXPR_ARRAY_ACCESS;
        e->line = 0;
        e->data.array_acc.arr_name = new string(name);
        e->data.array_acc.index = idx;
        return e;
    }
    static Expr* make_binary(BinOp op, Expr* l, Expr* r) {
        Expr* e = new Expr;
        e->type = EXPR_BINARY_OP;
        e->line = 0;
        e->data.binary.op = op;
        e->data.binary.left = l;
        e->data.binary.right = r;
        return e;
    }
};

enum CmdType {
    CMD_PRINT,
    CMD_PRINTC,
    CMD_PRINTSTR,
    CMD_SET_SCALAR,
    CMD_SET_ARRAY,
    CMD_READ,
    CMD_READC,
    CMD_DIM,
    CMD_IF,
    CMD_WHILE,
    CMD_SHOWVARS,
    CMD_EXIT,
    CMD_LABEL,
    CMD_GOTO
};

struct Command {
    CmdType type;
    int line;
    union {
        struct { Expr* expr; } print_cmd;
        struct { Expr* expr; } printc_cmd;
        struct { string* name; } printstr_cmd;
        struct { string* name; Expr* value; } set_scalar_cmd;
        struct { string* name; Expr* index; Expr* value; } set_array_cmd;
        struct { string* name; } read_cmd;
        struct { string* name; } readc_cmd;
        struct { string* name; Expr* size; } dim_cmd;
        struct { Expr* cond; vector<Command*>* body; } if_cmd;
        struct { Expr* cond; vector<Command*>* body; } while_cmd;
        struct { string* name; } label_cmd;
        struct { string* label; } goto_cmd;
    } data;

    static Command* make_print(Expr* e) {
        Command* c = new Command;
        c->type = CMD_PRINT;
        c->line = 0;
        c->data.print_cmd.expr = e;
        return c;
    }
    static Command* make_printc(Expr* e) {
        Command* c = new Command;
        c->type = CMD_PRINTC;
        c->line = 0;
        c->data.printc_cmd.expr = e;
        return c;
    }
    static Command* make_printstr(const string& name) {
        Command* c = new Command;
        c->type = CMD_PRINTSTR;
        c->line = 0;
        c->data.printstr_cmd.name = new string(name);
        return c;
    }
    static Command* make_set_scalar(const string& name, Expr* val) {
        Command* c = new Command;
        c->type = CMD_SET_SCALAR;
        c->line = 0;
        c->data.set_scalar_cmd.name = new string(name);
        c->data.set_scalar_cmd.value = val;
        return c;
    }
    static Command* make_set_array(const string& name, Expr* idx, Expr* val) {
        Command* c = new Command;
        c->type = CMD_SET_ARRAY;
        c->line = 0;
        c->data.set_array_cmd.name = new string(name);
        c->data.set_array_cmd.index = idx;
        c->data.set_array_cmd.value = val;
        return c;
    }
    static Command* make_read(const string& name) {
        Command* c = new Command;
        c->type = CMD_READ;
        c->line = 0;
        c->data.read_cmd.name = new string(name);
        return c;
    }
    static Command* make_readc(const string& name) {
        Command* c = new Command;
        c->type = CMD_READC;
        c->line = 0;
        c->data.readc_cmd.name = new string(name);
        return c;
    }
    static Command* make_dim(const string& name, Expr* size) {
        Command* c = new Command;
        c->type = CMD_DIM;
        c->line = 0;
        c->data.dim_cmd.name = new string(name);
        c->data.dim_cmd.size = size;
        return c;
    }
    static Command* make_if(Expr* cond, vector<Command*>* body) {
        Command* c = new Command;
        c->type = CMD_IF;
        c->line = 0;
        c->data.if_cmd.cond = cond;
        c->data.if_cmd.body = body;
        return c;
    }
    static Command* make_while(Expr* cond, vector<Command*>* body) {
        Command* c = new Command;
        c->type = CMD_WHILE;
        c->line = 0;
        c->data.while_cmd.cond = cond;
        c->data.while_cmd.body = body;
        return c;
    }
    static Command* make_showvars() {
        Command* c = new Command;
        c->type = CMD_SHOWVARS;
        c->line = 0;
        return c;
    }
    static Command* make_exit() {
        Command* c = new Command;
        c->type = CMD_EXIT;
        c->line = 0;
        return c;
    }
    static Command* make_label(const string& name) {
        Command* c = new Command;
        c->type = CMD_LABEL;
        c->line = 0;
        c->data.label_cmd.name = new string(name);
        return c;
    }
    static Command* make_goto(const string& label) {
        Command* c = new Command;
        c->type = CMD_GOTO;
        c->line = 0;
        c->data.goto_cmd.label = new string(label);
        return c;
    }
};

void print_expr(Expr* e);
void print_command(Command* cmd);

//                     ПАРСЕР
struct Parser {
    vector<Token>& tokens;
    size_t pos;

    Parser(vector<Token>& t) : tokens(t), pos(0) {}

    Token peek() { return tokens[pos]; }
    Token consume() { return tokens[pos++]; }
    bool match(TokenType t) {
        if (pos < tokens.size() && peek().type == t) {
            pos++;
            return true;
        }
        return false;
    }
    bool isEOF() { return pos >= tokens.size() || peek().type == TOK_EOF; }

    void error(const string& msg) {
        fprintf(stderr, "Ошибка парсера. Линия: %d: %s\n", peek().line, msg.c_str());
        exit(1);
    }

    Expr* parseExpression() {
        if (isEOF()) error("Файл пустой");
        Token t = consume();
        Expr* e = nullptr;
        if (t.type == TOK_NUMBER) {
            e = Expr::make_number(t.value);
        } else if (t.type == TOK_IDENT) {
            if (!isEOF() && peek().type == TOK_LBRACKET) {
                consume();
                Expr* idx = parseExpression();
                if (!match(TOK_RBRACKET)) error("Нет закрывающей скобки ']' после массива");
                e = Expr::make_array_access(t.lexeme, idx);
            } else {
                e = Expr::make_variable(t.lexeme);
            }
        } else {
            BinOp op;
            switch (t.type) {
                case TOK_OPERATOR_PLUS:  op = OP_ADD; break;
                case TOK_OPERATOR_MINUS: op = OP_SUB; break;
                case TOK_OPERATOR_MUL:   op = OP_MUL; break;
                case TOK_OPERATOR_DIV:   op = OP_DIV; break;
                case TOK_OPERATOR_EQ:    op = OP_EQ; break;
                case TOK_OPERATOR_NE:    op = OP_NE; break;
                case TOK_OPERATOR_LT:    op = OP_LT; break;
                case TOK_OPERATOR_GT:    op = OP_GT; break;
                case TOK_OPERATOR_LE:    op = OP_LE; break;
                case TOK_OPERATOR_GE:    op = OP_GE; break;
                default:
                    error("ожидалось выражение, получено: '" + t.lexeme + "'");
                    return nullptr;
            }
            Expr* left = parseExpression();
            Expr* right = parseExpression();
            e = Expr::make_binary(op, left, right);
        }
        if (e) e->line = t.line;
        return e;
    }

    Command* parseCommand() {
        if (isEOF()) return nullptr;
        Token t = peek();
        Command* cmd = nullptr;

        if (t.type == TOK_KEYWORD_PRINT) {
            consume();
            Expr* e = parseExpression();
            cmd = Command::make_print(e);
        }
        else if (t.type == TOK_KEYWORD_PRINTC) {
            consume();
            Expr* e = parseExpression();
            cmd = Command::make_printc(e);
        }
        else if (t.type == TOK_KEYWORD_PRINTSTR) {
            consume();
            Token arr = consume();
            if (arr.type != TOK_IDENT) error("ожидалось имя массива после 'printstr'");
            cmd = Command::make_printstr(arr.lexeme);
        }
        else if (t.type == TOK_KEYWORD_SET) {
            consume();
            Token var = consume();
            if (var.type != TOK_IDENT) error("Запрещённое имя после 'set'");
            string name = var.lexeme;
            if (!isEOF() && peek().type == TOK_LBRACKET) {
                consume();
                Expr* idx = parseExpression();
                if (!match(TOK_RBRACKET)) error("Нет закрывающей скобки ']'");
                if (!match(TOK_OPERATOR_ASSIGN)) error("Не хватает равно '='");
                Expr* val = parseExpression();
                cmd = Command::make_set_array(name, idx, val);
            } else {
                if (!match(TOK_OPERATOR_ASSIGN)) error("Не хватает равно '=' после переменной");
                Expr* val = parseExpression();
                cmd = Command::make_set_scalar(name, val);
            }
        }
        else if (t.type == TOK_KEYWORD_READ) {
            consume();
            Token var = consume();
            if (var.type != TOK_IDENT) error("запрещённая команда после 'read'");
            cmd = Command::make_read(var.lexeme);
        }
        else if (t.type == TOK_KEYWORD_READC) {
            consume();
            Token var = consume();
            if (var.type != TOK_IDENT) error("запрещённое имя после 'readc'");
            cmd = Command::make_readc(var.lexeme);
        }
        else if (t.type == TOK_KEYWORD_DIM) {
            consume();
            Token var = consume();
            if (var.type != TOK_IDENT) error("неверное имя массива после 'dim'");
            Expr* size = parseExpression();
            cmd = Command::make_dim(var.lexeme, size);
        }
        else if (t.type == TOK_KEYWORD_IF) {
            consume();
            Expr* cond = parseExpression();
            if (!match(TOK_KEYWORD_THEN)) error("не хватает 'then'");
            Command* body_cmd = parseCommand();
            vector<Command*>* body = new vector<Command*>();
            body->push_back(body_cmd);
            cmd = Command::make_if(cond, body);
        }
        else if (t.type == TOK_KEYWORD_WHILE) {
            consume();
            Expr* cond = parseExpression();
            if (!match(TOK_KEYWORD_DO)) error("не хватает 'do'");
            Command* body_cmd = parseCommand();
            vector<Command*>* body = new vector<Command*>();
            body->push_back(body_cmd);
            cmd = Command::make_while(cond, body);
        }
        else if (t.type == TOK_KEYWORD_SHOWVARS) {
            consume();
            cmd = Command::make_showvars();
        }
        else if (t.type == TOK_KEYWORD_EXIT) {
            consume();
            cmd = Command::make_exit();
        }
        else if (t.type == TOK_KEYWORD_LABEL) {
            consume();
            Token name = consume();
            if (name.type != TOK_IDENT) error("Не хватает названия метки после 'label'");
            cmd = Command::make_label(name.lexeme);
        }
        else if (t.type == TOK_KEYWORD_GOTO) {
            consume();
            Token label = consume();
            if (label.type != TOK_IDENT) error("Не хватает метки для 'goto'");
            cmd = Command::make_goto(label.lexeme);
        }
        else {
            error("неизвестный токен '" + t.lexeme + "'");
            return nullptr;
        }

        if (cmd) cmd->line = t.line;
        return cmd;
    }

    vector<Command*> parseProgram() {
        vector<Command*> program;
        while (!isEOF()) {
            while (match(TOK_SEMICOLON)) {}
            if (isEOF()) break;
            Command* cmd = parseCommand();
            if (cmd) program.push_back(cmd);
            match(TOK_SEMICOLON);
        }
        return program;
    }
};

//                    ВЫПОЛНЕНИЕ И ОШИБКИ
struct Array {
    vector<int> data;
    Array(int sz) : data(sz, 0) {}
};

struct Environment {
    vector<string> var_names;
    vector<int>    var_values;
    vector<string> arr_names;
    vector<Array>  arr_values;

    size_t goto_pc;

    int get_scalar(const string& name) {
        for (size_t i = 0; i < var_names.size(); i++)
            if (var_names[i] == name) return var_values[i];
        fprintf(stderr, "Предупреждение: неопределённая переменная '%s', using 0\n", name.c_str());
        return 0;
    }

    void set_scalar(const string& name, int val) {
        for (size_t i = 0; i < var_names.size(); i++) {
            if (var_names[i] == name) {
                var_values[i] = val;
                return;
            }
        }
        var_names.push_back(name);
        var_values.push_back(val);
    }

    void dim_array(const string& name, int size) {
        if (size <= 0) {
            fprintf(stderr, "Error: длина массива <= 0\n");
            return;
        }
        for (size_t i = 0; i < arr_names.size(); i++) {
            if (arr_names[i] == name) {
                fprintf(stderr, "Error: массив '%s' уже существует\n", name.c_str());
                return;
            }
        }
        arr_names.push_back(name);
        arr_values.push_back(Array(size));
    }

    int get_array(const string& name, int idx) {
        for (size_t i = 0; i < arr_names.size(); i++) {
            if (arr_names[i] == name) {
                if (idx < 0 || idx >= (int)arr_values[i].data.size()) {
                    fprintf(stderr, "Error: вышли за предел массива\n");
                    return 0;
                }
                return arr_values[i].data[idx];
            }
        }
        fprintf(stderr, "Error: неопределённый массив '%s'\n", name.c_str());
        return 0;
    }

    void set_array(const string& name, int idx, int val) {
        for (size_t i = 0; i < arr_names.size(); i++) {
            if (arr_names[i] == name) {
                if (idx < 0 || idx >= (int)arr_values[i].data.size()) {
                    fprintf(stderr, "Error: неверный индекс массива\n");
                    return;
                }
                arr_values[i].data[idx] = val;
                return;
            }
        }
        fprintf(stderr, "Error: неопределённый массив '%s'\n", name.c_str());
    }

    void show() {
        printf("\n=== Variables ===\n");
        for (size_t i = 0; i < var_names.size(); i++)
            printf("%s = %d\n", var_names[i].c_str(), var_values[i]);
        for (size_t i = 0; i < arr_names.size(); i++) {
            printf("%s = [", arr_names[i].c_str());
            for (size_t j = 0; j < arr_values[i].data.size(); j++) {
                if (j > 0) printf(", ");
                printf("%d", arr_values[i].data[j]);
            }
            printf("]\n");
        }
        printf("==================\n\n");
    }
};

//                       ВЫЧИСЛЕНИЯ
int eval_expr(Expr* e, Environment& env) {
    switch (e->type) {
        case EXPR_NUMBER:
            return e->data.num_val;
        case EXPR_VARIABLE:
            return env.get_scalar(*e->data.var_name);
        case EXPR_ARRAY_ACCESS: {
            int idx = eval_expr(e->data.array_acc.index, env);
            return env.get_array(*e->data.array_acc.arr_name, idx);
        }
        case EXPR_BINARY_OP: {
            int l = eval_expr(e->data.binary.left, env);
            int r = eval_expr(e->data.binary.right, env);
            switch (e->data.binary.op) {
                case OP_ADD: return l + r;
                case OP_SUB: return l - r;
                case OP_MUL: return l * r;
                case OP_DIV:
                    if (r == 0) { fprintf(stderr, "Error: Деление на ноль\n"); return 0; }
                    return l / r;
                case OP_EQ: return l == r;
                case OP_NE: return l != r;
                case OP_LT: return l < r;
                case OP_GT: return l > r;
                case OP_LE: return l <= r;
                case OP_GE: return l >= r;
            }
            return 0;
        }
    }
    return 0;
}

void execute_command(Command* cmd, Environment& env, map<string, size_t>& labels) {
    switch (cmd->type) {
        case CMD_PRINT:
            printf("%d\n", eval_expr(cmd->data.print_cmd.expr, env));
            break;
        case CMD_PRINTC:
            printf("%c", (char)eval_expr(cmd->data.printc_cmd.expr, env));
            break;
        case CMD_PRINTSTR: {
            const string& name = *cmd->data.printstr_cmd.name;
            bool found = false;
            for (size_t i = 0; i < env.arr_names.size(); ++i) {
                if (env.arr_names[i] == name) {
                    found = true;
                    const vector<int>& data = env.arr_values[i].data;
                    for (size_t j = 0; j < data.size(); ++j) {
                        if (data[j] == 0) break;
                        printf("%c", (char)data[j]);
                    }
                    break;
                }
            }
            if (!found) {
                fprintf(stderr, "Error: неопределённый массив '%s'\n", name.c_str());
            }
            break;
        }
        case CMD_SET_SCALAR:
            env.set_scalar(*cmd->data.set_scalar_cmd.name,
                           eval_expr(cmd->data.set_scalar_cmd.value, env));
            break;
        case CMD_SET_ARRAY:
            env.set_array(*cmd->data.set_array_cmd.name,
                          eval_expr(cmd->data.set_array_cmd.index, env),
                          eval_expr(cmd->data.set_array_cmd.value, env));
            break;
        case CMD_READ: {
            int val;
            scanf("%d", &val);
            env.set_scalar(*cmd->data.read_cmd.name, val);
            break;
        }
        case CMD_READC: {
            int ch = getchar();
            env.set_scalar(*cmd->data.readc_cmd.name, ch);
            break;
        }
        case CMD_DIM:
            env.dim_array(*cmd->data.dim_cmd.name,
                          eval_expr(cmd->data.dim_cmd.size, env));
            break;
        case CMD_IF: {
            if (eval_expr(cmd->data.if_cmd.cond, env)) {
                for (Command* c : *cmd->data.if_cmd.body) {
                    execute_command(c, env, labels);
                    if (env.goto_pc != SIZE_MAX) 
                        break;
                }
            }
            break;
        }
        case CMD_WHILE: {
            while (eval_expr(cmd->data.while_cmd.cond, env)) {
                for (Command* c : *cmd->data.while_cmd.body) {
                    execute_command(c, env, labels);
                    if (env.goto_pc != SIZE_MAX) 
                        break;
                }
                if (env.goto_pc != SIZE_MAX)    
                    break;
            }
            break;
        }
        case CMD_SHOWVARS:
            env.show();
            break;
        case CMD_EXIT:
            exit(0);
        case CMD_LABEL:
            break;  
        case CMD_GOTO: {
            const string& label = *cmd->data.goto_cmd.label;
            map<string, size_t>::iterator it = labels.find(label);
            if (it == labels.end()) {
                fprintf(stderr, "Error: неопределённая метка '%s'\n", label.c_str());
                exit(1);
            }
            env.goto_pc = it->second;   
            break;
        }
    }
}

//                       ПЕЧАТЬ AST
void print_expr(Expr* e) {
    if (!e) { printf("null"); return; }
    switch (e->type) {
        case EXPR_NUMBER: printf("%d", e->data.num_val); break;
        case EXPR_VARIABLE: printf("%s", e->data.var_name->c_str()); break;
        case EXPR_ARRAY_ACCESS:
            printf("%s[", e->data.array_acc.arr_name->c_str());
            print_expr(e->data.array_acc.index);
            printf("]");
            break;
        case EXPR_BINARY_OP: {
            const char* opstr = "";
            switch (e->data.binary.op) {
                case OP_ADD: opstr = "+"; break;
                case OP_SUB: opstr = "-"; break;
                case OP_MUL: opstr = "*"; break;
                case OP_DIV: opstr = "/"; break;
                case OP_EQ:  opstr = "=="; break;
                case OP_NE:  opstr = "!="; break;
                case OP_LT:  opstr = "<"; break;
                case OP_GT:  opstr = ">"; break;
                case OP_LE:  opstr = "<="; break;
                case OP_GE:  opstr = ">="; break;
            }
            printf("(%s ", opstr);
            print_expr(e->data.binary.left);
            printf(" ");
            print_expr(e->data.binary.right);
            printf(")");
            break;
        }
    }
}

void print_command(Command* cmd) {
    if (!cmd) return;
    switch (cmd->type) {
        case CMD_PRINT:
            printf("print ");
            print_expr(cmd->data.print_cmd.expr);
            break;
        case CMD_PRINTC:
            printf("printc ");
            print_expr(cmd->data.printc_cmd.expr);
            break;
        case CMD_PRINTSTR:
            printf("printstr %s", cmd->data.printstr_cmd.name->c_str());
            break;
        case CMD_SET_SCALAR:
            printf("set %s = ", cmd->data.set_scalar_cmd.name->c_str());
            print_expr(cmd->data.set_scalar_cmd.value);
            break;
        case CMD_SET_ARRAY:
            printf("set %s[", cmd->data.set_array_cmd.name->c_str());
            print_expr(cmd->data.set_array_cmd.index);
            printf("] = ");
            print_expr(cmd->data.set_array_cmd.value);
            break;
        case CMD_READ:
            printf("read %s", cmd->data.read_cmd.name->c_str());
            break;
        case CMD_READC:
            printf("readc %s", cmd->data.readc_cmd.name->c_str());
            break;
        case CMD_DIM:
            printf("dim %s ", cmd->data.dim_cmd.name->c_str());
            print_expr(cmd->data.dim_cmd.size);
            break;
        case CMD_IF:
            printf("if ");
            print_expr(cmd->data.if_cmd.cond);
            printf(" then\n  ");
            for (Command* c : *cmd->data.if_cmd.body) {
                print_command(c);
                printf(";\n  ");
            }
            printf("end");
            break;
        case CMD_WHILE:
            printf("while ");
            print_expr(cmd->data.while_cmd.cond);
            printf(" do\n  ");
            for (Command* c : *cmd->data.while_cmd.body) {
                print_command(c);
                printf(";\n  ");
            }
            printf("end");
            break;
        case CMD_SHOWVARS:
            printf("showvars");
            break;
        case CMD_EXIT:
            printf("exit");
            break;
        case CMD_LABEL:
            printf("label %s", cmd->data.label_cmd.name->c_str());
            break;
        case CMD_GOTO:
            printf("goto %s", cmd->data.goto_cmd.label->c_str());
            break;
    }
}

// Обычный режим выполнения
void interpret(vector<Command*>& program) {
    Environment env;
    env.goto_pc = SIZE_MAX;

    map<string, size_t> labels;
    for (size_t i = 0; i < program.size(); ++i) {
        if (program[i]->type == CMD_LABEL) {
            const string& name = *program[i]->data.label_cmd.name;
            if (labels.find(name) != labels.end()) {
                fprintf(stderr, "Error: Название метки занято '%s'\n", name.c_str());
                exit(1);
            }
            labels[name] = i;
        }
    }

    size_t pc = 0;
    while (pc < program.size()) {
        Command* cmd = program[pc];
        execute_command(cmd, env, labels);   

        if (env.goto_pc != SIZE_MAX) {       
            pc = env.goto_pc;                
            env.goto_pc = SIZE_MAX;          
            continue;                        
        }
        ++pc;
    }
}

// Пошаговый режим выполнения
void interpret_step(vector<Command*>& program) {
    Environment env;
    env.goto_pc = SIZE_MAX;

    map<string, size_t> labels;
    for (size_t i = 0; i < program.size(); ++i) {
        if (program[i]->type == CMD_LABEL) {
            const string& name = *program[i]->data.label_cmd.name;
            if (labels.find(name) != labels.end()) {
                fprintf(stderr, "Error: Название метки занято '%s'\n", name.c_str());
                exit(1);
            }
            labels[name] = i;
        }
    }

    size_t pc = 0;
    char input[256];

    while (pc < program.size()) {
        Command* cmd = program[pc];

        printf("[Line %d] ", cmd->line);
        print_command(cmd);
        printf("\n");

        printf("step> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';

        if (strlen(input) == 0) {
            // пустой ввод – выполняем текущую команду
        }
        else if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
            printf("Выполнение остановлено пользователем.\n");
            break;
        }
        else if (strcmp(input, "v") == 0 || strcmp(input, "vars") == 0) {
            env.show();
            continue;
        }
        else if (strncmp(input, "print ", 6) == 0) {
            const char* varname = input + 6;
            while (*varname == ' ') varname++;
            int val = env.get_scalar(string(varname));
            printf("%s = %d\n", varname, val);
            continue;
        }
        else if (strcmp(input, "h") == 0 || strcmp(input, "help") == 0) {
            printf("Команды пошагового режима:\n");
            printf("  [Enter]       - выполнить текущую команду\n");
            printf("  v / vars      - показать все переменные и массивы\n");
            printf("  print <имя>   - показать значение скалярной переменной\n");
            printf("  q / quit      - остановить выполнение\n");
            printf("  h / help      - эта справка\n");
            continue;
        }
        else {
            printf("Неизвестная команда. Введите 'h' для справки.\n");
            continue;
        }

        // Выполняем команду
        execute_command(cmd, env, labels);

        if (env.goto_pc != SIZE_MAX) {
            pc = env.goto_pc;
            env.goto_pc = SIZE_MAX;
        } else {
            ++pc;
        }
    }
}

//                           MAIN
int main(int argc, char* argv[]) {
    const char* filename = "testcode.txt";
    const char* mode = "run";

    if (argc >= 2) {
        if (strcmp(argv[1], "-lex") == 0 || strcmp(argv[1], "-parse") == 0 || 
            strcmp(argv[1], "-run") == 0 || strcmp(argv[1], "-step") == 0) {
            mode = argv[1] + 1;
            if (argc >= 3) filename = argv[2];
        } else {
            filename = argv[1];
        }
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Не удалось открыть файл '%s'\n", filename);
        return 1;
    }

    vector<Token> tokens = lexer(file);
    fclose(file);

    // Проверка на лексические ошибки (TOK_UNKNOWN)
    bool lex_error = false;
    for (const Token& t : tokens) {
        if (t.type == TOK_UNKNOWN) {
            fprintf(stderr, "Лексическая ошибка: неожиданный символ '%s' на строке %d\n",
                    t.lexeme.c_str(), t.line);
            lex_error = true;
        }
    }
    if (lex_error) {
        return 1;
    }

    if (strcmp(mode, "lex") == 0) {
        for (size_t i = 0; i < tokens.size(); i++) {
            Token& t = tokens[i];
            printf("Token: %-12s type=%2d line=%d", t.lexeme.c_str(), t.type, t.line);
            if (t.type == TOK_NUMBER) printf(" value=%d", t.value);
            printf("\n");
        }
        return 0;
    }

    Parser parser(tokens);
    vector<Command*> program = parser.parseProgram();

    if (strcmp(mode, "parse") == 0) {
        for (Command* cmd : program) {
            print_command(cmd);
            printf(";\n");
        }
        return 0;
    }

    if (strcmp(mode, "step") == 0) {
        interpret_step(program);
    } else {
        interpret(program);
    }

    return 0;
}
