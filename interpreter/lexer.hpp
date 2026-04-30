#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

const string LEX_S = "S";
const string LEX_VAR_NAME = "VAR_NAME";
const string LEX_FUNC_NAME = "FUNC_NAME";
const string LEX_DEREFERENCE = "DEREFERENCE";
const string LEX_NUMBER = "NUMBER";
const string LEX_FUNCTION_BORDER = "FUNCTION_BORDER";
const string LEX_MATH_OP_BINARY = "MATH_OP_BINARY";
const string LEX_MATH_OP_UNARY = "MATH_OP_UNARY";
const string LEX_KEYWORD_IF = "KEYWORD_IF";
const string LEX_KEYWORD_LOOP = "KEYWORD_LOOP";
const string LEX_KEYWORD_RETURN = "KEYWORD_RETURN";
const string LEX_KEYWORD_WRITE = "KEYWORD_WRITE";
const string LEX_KEYWORD_READ = "KEYWORD_READ";
const string LEX_ENDLINE = "ENDLINE";
const string LEX_EOF = "EOF";

namespace Lex {
    enum Type { 
        S,
        VAR_NAME,
        FUNC_NAME,
        DEREFERENCE,
        NUMBER,
        FUNCTION_BORDER,
        MATH_OP_BINARY,
        MATH_OP_UNARY,
        KEYWORD_IF,
        KEYWORD_LOOP,
        KEYWORD_RETURN,
        KEYWORD_WRITE,
        KEYWORD_READ,
        ENDLINE,
        END_OF_FILE
    };

    static const vector<Type> All = {
        S,
        VAR_NAME,
        FUNC_NAME,
        DEREFERENCE,
        NUMBER,
        FUNCTION_BORDER,
        MATH_OP_BINARY,
        MATH_OP_UNARY,
        KEYWORD_IF,
        KEYWORD_LOOP,
        KEYWORD_RETURN,
        KEYWORD_WRITE,
        KEYWORD_READ,
        ENDLINE,
        END_OF_FILE
    };

    static std::map<std::string, Lex::Type> smap;
}

const int operator+(Lex::Type lex) { 
    return (int)lex;
}

const string& lexToString(Lex::Type lex) {
    switch (lex) {
        case Lex::S: return LEX_S;
        case Lex::VAR_NAME: return LEX_VAR_NAME;
        case Lex::FUNC_NAME: return LEX_FUNC_NAME;
        case Lex::DEREFERENCE: return LEX_DEREFERENCE;
        case Lex::NUMBER: return LEX_NUMBER;
        case Lex::FUNCTION_BORDER: return LEX_FUNCTION_BORDER;
        case Lex::MATH_OP_BINARY: return LEX_MATH_OP_BINARY;
        case Lex::MATH_OP_UNARY: return LEX_MATH_OP_UNARY;
        case Lex::KEYWORD_IF: return LEX_KEYWORD_IF;
        case Lex::KEYWORD_LOOP: return LEX_KEYWORD_LOOP;
        case Lex::KEYWORD_RETURN: return LEX_KEYWORD_RETURN;
        case Lex::KEYWORD_WRITE: return LEX_KEYWORD_WRITE;
        case Lex::KEYWORD_READ: return LEX_KEYWORD_READ;
        case Lex::ENDLINE: return LEX_ENDLINE;
        case Lex::END_OF_FILE: return LEX_EOF;
    } 
}

const Lex::Type lexFromString(const string& str) {
    auto& m = Lex::smap;
    if (m.empty()) {
        for (const auto lex : Lex::All) {
            m.emplace(lexToString(lex), lex);
        }
    }

    return m[str];
}

