#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        throw runtime_error("Error léxico");
    }
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (check(Token::ERR)) {
            throw runtime_error("Error lexico");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

//Funciones de respaldo y restauración del estado del parser (hipotéticas)

// =============================
// Reglas gramaticales
// =============================

Program* Parser::parseProgram() {
    Program * programa = new Program();
    programa->slist.push_back(parseStm());
    while(match(Token::SEMICOL)) {
        programa->slist.push_back(parseStm());
    }

    if (!isAtEnd()) {
        throw runtime_error("Error sintáctico");
    }
    cout << "Parseo exitoso" << endl;
    return programa;
}

Stm* Parser::parseStm() {
    Stm* stm;
    Exp* e;
    string nombre;
    if(match(Token::PRINT)) {
        match(Token::LPAREN);
        e = parseCE();
        match(Token::RPAREN);
        return new PrintStm(e);
    }
    else if(match(Token::ID)) {
        nombre = previous->text;
        match(Token::ASSIGN);
        e = parseCE();
        return new AssignStm(nombre, e);
    }
    return stm;
}

Exp* Parser::parseCE() {
    if (check(Token::LCOR)) { 
        return parseSetExp();  
    }
    Token* savedState = current;
    try {
        Exp* setExp = parseSetExp();
        return setExp;  
    } catch (runtime_error& e) {
        current = savedState;
    }
    
    return parseE();
}

Exp* Parser::parseE() {
    Exp* l = parseE();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseE();
        l = new BinaryExp(l, r, op);
    }
    return l;
}


Exp* Parser::parseT() {
    Exp* l = parseF();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else{
            op = DIV_OP;
        }
        Exp* r = parseF();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseF() {
    Exp* e; 
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    } 
    else if (match(Token::LPAREN))
    {
        e = parseCE();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::SQRT))
    {   
        match(Token::LPAREN);
        e = parseCE();
        match(Token::RPAREN);
        return new SqrtExp(e);
    }
    else if (match(Token::ID))
    {
        return new IdExp(previous->text);
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}

//Nueva logica para conjuntos:

Exp* Parser::parseSetExp(){
    Exp* l = parseSetTerm();
    while (match(Token::CUP) || match(Token::CAP) || match(Token::DIFF)) {
        SetOp op;
        if (previous->type == Token::CUP){
            op = CUP_OP;
        }
        else if (previous->type == Token::CAP){
            op = CAP_OP;
        }
        else if (previous->type == Token::DIFF){
            op = DIFF_OP;
        }
        Exp* r = parseSetTerm();
        l = new SetBinaryExp(l, r, op);
    }
    return l;
};

Exp* Parser::parseSetTerm(){
    return parseSetFactor();
};

Exp* Parser::parseSetFactor(){
    Exp* e; 
    if (check(Token::LCOR)) {
        e = parseSet();
        return e;
    } 
    else if (match(Token::LPAREN))
    {
        e = parseSetExp();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::ID))
    {
        return new IdExp(previous->text);
    }
    else {
        throw runtime_error("Error sintáctico");
    }
};

Exp* Parser::parseSet(){
    if(match(Token::LCOR)){
        SetExp* e = new SetExp();
        if (!check(Token::RCOR)){
            e->elements.push_back(parseSetExp());
            while(match(Token::COMMA)){
                Exp* r = parseSetExp();
                e->elements.push_back(r);
            }
        }
        match(Token::RCOR);
        return e;
    }
    else{
        throw runtime_error("Error sintáctico");
    }
};