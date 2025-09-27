#include "ast.h"
#include "visitor.h"

#include <complex.h>
#include <iostream>

using namespace std;

// ------------------ Exp ------------------
Exp::~Exp() {}

string Exp::binopToChar(BinaryOp op) {
    switch (op) {
        case PLUS_OP:  return "+";
        case MINUS_OP: return "-";
        case MUL_OP:   return "*";
        case DIV_OP:   return "/";
        case POW_OP:   return "**";
        default:       return "?";
    }
}

string Exp::setopToChar(SetOp op) {
    switch (op) {
        case CUP_OP:  return "∪";
        case CAP_OP:  return "∩";
        case DIFF_OP: return "−";
        default:     return "?";
    }
}

// ------------------ BinaryExp ------------------
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp o)
    : left(l), right(r), op(o) {}

    
BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}



// ------------------ NumberExp ------------------
NumberExp::NumberExp(int v) : value(v) {}

NumberExp::~NumberExp() {}


// ------------------ SqrtExp ------------------
SqrtExp::SqrtExp(Exp* v) : value(v) {}

SqrtExp::~SqrtExp() {}

// ------------------ SetBinaryExp ------------------
SetBinaryExp::SetBinaryExp(Exp* l, Exp* r, SetOp o)
    : left(l), right(r), op(o) {}
SetBinaryExp::~SetBinaryExp() {
    delete left;
    delete right;
}

// ------------------ SetExp ------------------
SetExp::SetExp() {}
SetExp::SetExp(vector<Exp*> elems) : elements(elems) {}
SetExp::~SetExp() {
    for (Exp* e : elements) {
        delete e;
    }
}

///////////////////////////////////////////////
Program::Program(){}
Program::~Program(){}
Stm::~Stm(){}
PrintStm::PrintStm(Exp* e1){
    e=e1;
}
PrintStm::~PrintStm(){}
AssignStm::AssignStm(string id1, Exp* rhs1){
    id = id1;
    rhs=rhs1;
}
AssignStm::~AssignStm(){}

IdExp::IdExp(string v) {
    value=v;
}
IdExp::~IdExp() {
}