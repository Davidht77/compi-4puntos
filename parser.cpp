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
        if (isAtEnd()) {
            break;
        }
        programa->slist.push_back(parseStm());
    }

    if (!isAtEnd()) {
        throw runtime_error("Error sintáctico");
    }
    cout << "Parseo exitoso" << endl;
    return programa;
}

Stm* Parser::parseStm() {
    Exp* e;
    string nombre;

    if (match(Token::PRINT)) {
        if (!match(Token::LPAREN)) {
            throw runtime_error("Error sintáctico");
        }
        e = parseCE();
        if (!match(Token::RPAREN)) {
            throw runtime_error("Error sintáctico");
        }
        return new PrintStm(e);
    } else if (match(Token::ID)) {
        nombre = previous->text;
        if (!match(Token::ASSIGN)) {
            throw runtime_error("Error sintáctico");
        }
        e = parseCE();
        return new AssignStm(nombre, e);
    }

    throw runtime_error("Error sintáctico");
}

Exp* Parser::parseCE() {
    Exp* expr = nullptr;

    if (check(Token::LCOR)) {
        expr = parseSet();
    } else {
        expr = parseE();
    }

    while (true) {
        if (match(Token::CUP)) {
            Exp* right = parseSetExp();
            expr = new SetBinaryExp(expr, right, CUP_OP);
        } else if (match(Token::CAP)) {
            Exp* right = parseSetExp();
            expr = new SetBinaryExp(expr, right, CAP_OP);
        } else if (match(Token::DIFF)) {
            Exp* right = parseSetExp();
            expr = new SetBinaryExp(expr, right, DIFF_OP);
        } else {
            break;
        }
    }

    return expr;
}

Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseT();
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

Exp* Parser::parseSetExp() {
    if (check(Token::LCOR)) {
        return parseSet();
    }

    if (match(Token::LPAREN)) {
        Exp* inner = parseCE();
        if (!match(Token::RPAREN)) {
            throw runtime_error("Error sintáctico");
        }
        return inner;
    }

    if (match(Token::ID)) {
        return new IdExp(previous->text);
    }

    throw runtime_error("Error sintáctico");
};

Exp* Parser::parseSet(){
    if (!match(Token::LCOR)) {
        throw runtime_error("Error sintáctico");
    }

    SetExp* e = new SetExp();
    if (!check(Token::RCOR)) {
        e->elements.push_back(parseCE());
        while (match(Token::COMMA)) {
            e->elements.push_back(parseCE());
        }
    }

    if (!match(Token::RCOR)) {
        throw runtime_error("Error sintáctico");
    }

    return e;
};
