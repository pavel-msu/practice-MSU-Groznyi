#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

const string OP_WRITE = "WRITE";
const string OP_READ = "READ";
const string OP_CALL = "CALL";
const string OP_RETURN = "RETURN";
const string OP_DEREFERENCE = "DEREFERENCE";
const string OP_MATH_PLUS = "MATH_PLUS";
const string OP_MATH_MINUS = "MATH_MINUS";
const string OP_MATH_MUL = "MATH_MUL";
const string OP_MATH_DIV = "MATH_DIV";
const string OP_LOGIC_NOT = "LOGIC_NOT";
const string OP_LOGIC_AND = "LOGIC_AND";
const string OP_LOGIC_OR = "LOGIC_OR";
const string OP_RESET = "RESET";

namespace Op {
    enum Type { 
        WRITE,
        READ,
        CALL,
        RETURN,
        DEREFERENCE,
        MATH_PLUS,
        MATH_MINUS,
        MATH_MUL,
        MATH_DIV,
        LOGIC_NOT,
        LOGIC_AND,
        LOGIC_OR,
        RESET
    };

    static const vector<Type> All = {
        WRITE,
        READ,
        CALL,
        RETURN,
        DEREFERENCE,
        MATH_PLUS,
        MATH_MINUS,
        MATH_MUL,
        MATH_DIV,
        LOGIC_NOT,
        LOGIC_AND,
        LOGIC_OR,
        RESET
    };

    static std::map<std::string, Op::Type> smap;
}

const int operator+(Op::Type op) { 
    return (int)op;
}

const string& opToString(Op::Type op) {
    switch (op) {
        case Op::WRITE: return OP_WRITE;
        case Op::READ: return OP_READ;
        case Op::CALL: return OP_CALL;
        case Op::RETURN: return OP_RETURN;
        case Op::DEREFERENCE: return OP_DEREFERENCE;
        case Op::MATH_PLUS: return OP_MATH_PLUS;
        case Op::MATH_MINUS: return OP_MATH_MINUS;
        case Op::MATH_MUL: return OP_MATH_MUL;
        case Op::MATH_DIV: return OP_MATH_DIV;
        case Op::LOGIC_NOT: return OP_LOGIC_NOT;
        case Op::LOGIC_AND: return OP_LOGIC_AND;
        case Op::LOGIC_OR: return OP_LOGIC_OR;
        case Op::RESET: return OP_RESET;
    } 
}

const Op::Type opFromString(const string& str) {
    auto& m = Op::smap;
    if (m.empty()) {
        for (const auto op : Op::All) {
            m.emplace(opToString(op), op);
        }
    }

    return m[str];
}

