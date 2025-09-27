#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <list>
#include <ostream>
#include <variant>
#include <set>
#include <vector>

using namespace std;

// Tipo Value para manejar int o set<int>
using Value = std::variant<int, std::set<int>>;

class Visitor; 

// Operadores binarios soportados
enum BinaryOp { 
    PLUS_OP, 
    MINUS_OP, 
    MUL_OP, 
    DIV_OP,
    POW_OP
};

//Operadores de conjuntos soportados
enum SetOp {
    CUP_OP,   // cup (unión)
    CAP_OP,   // cap (intersección)
    DIFF_OP   // \ (diferencia)
};

// Clase abstracta Exp
class Exp {
public:
    virtual Value  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;  // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op);  // Conversión operador → string
    static string setopToChar(SetOp op);  // Conversión operador → string
};

// Expresión binaria
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    Value accept(Visitor* visitor);
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();
};

// Expresión numérica
class NumberExp : public Exp {
public:
    int value;
    Value accept(Visitor* visitor);
    NumberExp(int v);
    ~NumberExp();
};

class IdExp : public Exp {
public:
    string value;
    Value accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};

// Raiz cuadrada
class SqrtExp : public Exp {
public:
    Exp* value;
    Value accept(Visitor* visitor);
    SqrtExp(Exp* v);
    ~SqrtExp();
}
;
class Stm{
public:
    virtual Value  accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;  
};
class AssignStm: public Stm{
public:
    string id;
    Exp* rhs;
    Value accept(Visitor* visitor);
    AssignStm(string , Exp* );
    ~AssignStm();
};

class PrintStm: public Stm{
public:
    Exp* e;
    Value accept(Visitor* visitor);
    PrintStm(Exp*);
    ~PrintStm();
};


/// Expresión conjunto binaria
class SetBinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    SetOp op;
    Value accept(Visitor* visitor);
    SetBinaryExp(Exp* l, Exp* r, SetOp op);
    ~SetBinaryExp();
};

/// Expresión conjunto elemento
class SetExp : public Exp {
public:
    vector<Exp*> elements;
    Value accept(Visitor* visitor);
    SetExp(vector<Exp*> elems);
    SetExp();
    ~SetExp();
};


class Program{
public:
    list<Stm*> slist;
    Value accept(Visitor* visitor);
    Program();
    ~Program();
}
;

#endif // AST_H
