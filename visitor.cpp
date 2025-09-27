#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include "ast.h"
#include "visitor.h"


using namespace std;
unordered_map<std::string, Value> memoria;

// ------------------ Implementaciones de métodos accept ------------------
Value BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value SqrtExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value AssignStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value PrintStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value SetBinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value SetExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

///////////////////////////////////////////////////////////////////////////////////

// PrintVisitor methods
Value PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

Value PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

Value PrintVisitor::visit(SqrtExp* exp) {
    cout << "sqrt(";
    exp->value->accept(this);
    cout <<  ")";
    return 0;
}


void PrintVisitor::imprimir(Program* programa){
    if (programa)
    {
        cout << "Codigo:" << endl; 
        programa->accept(this);
        cout << endl;
    }
    return ;
}

///////////////////////////////////////////////////////////////////////////////////
// EVALVisitor methods
Value EVALVisitor::visit(BinaryExp* exp) {
    Value v1 = exp->left->accept(this);
    Value v2 = exp->right->accept(this);
    
    // Asume que son ints (agrega checks para tipos si es necesario)
    int left = std::get<int>(v1);
    int right = std::get<int>(v2);
    int result;
    switch (exp->op) {
        case PLUS_OP:
            result = left + right;
            break;
        case MINUS_OP:
            result = left - right;
            break;
        case MUL_OP:
            result = left * right;
            break;
        case DIV_OP:
            if (right != 0)
                result = left / right;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case POW_OP:
            result = pow(left, right);
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;  // Envuelve en Value (implícitamente int)
}

Value EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

Value EVALVisitor::visit(SqrtExp* exp) {
    Value val = exp->value->accept(this);
    int v = std::get<int>(val);
    return static_cast<int>(floor(sqrt(v)));
}


void EVALVisitor::interprete(Program* programa){
    if (programa)
    {
        cout << "Interprete:";
        programa->accept(this);
        cout<<endl;
    }
    return;

}

///////////////////////////////////////////////////////////////////////

Value EVALVisitor::visit(PrintStm* stm) {
    Value val = stm->e->accept(this);
    
    // Imprime según el tipo
    if (std::holds_alternative<int>(val)) {
        cout << std::get<int>(val);
    } else if (std::holds_alternative<std::set<int>>(val)) {
        auto& setVal = std::get<std::set<int>>(val);
        cout << "{";
        for (auto it = setVal.begin(); it != setVal.end(); ++it) {
            if (it != setVal.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}";
    }
    cout << endl;
    return val;
}

Value EVALVisitor::visit(AssignStm* stm) {
    Value val = stm->rhs->accept(this);
    memoria[stm->id] = val;
    return val;  // Retorna el valor asignado
}

Value EVALVisitor::visit(IdExp* exp) {
    if (memoria.find(exp->value) != memoria.end()) {
        return memoria[exp->value];
    } else {
        cout << "Error: variable no definida: " << exp->value << endl;
        return 0;  // O lanza error
    }
}

Value PrintVisitor::visit(IdExp* exp) {
    cout << exp->value;
    return 0;
}

Value EVALVisitor::visit(Program* p) {
    for(auto i:p->slist) {
        i->accept(this);
    }
    return 0;
}

Value PrintVisitor::visit(PrintStm* stm) {
    cout << "print(";
    stm -> e ->accept(this);
    cout << ")" << endl;
    return 0;
}

Value PrintVisitor::visit(AssignStm* stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << endl;
    return 0;
}

Value PrintVisitor::visit(Program* p) {
    for(auto i:p->slist) {
        i->accept(this);
    }
    return 0;
}

// Nuevos métodos para sets
Value EVALVisitor::visit(SetBinaryExp* exp) {
    Value v1 = exp->left->accept(this);
    Value v2 = exp->right->accept(this);
    
    // Asume que left y right son sets
    auto& leftSet = std::get<std::set<int>>(v1);
    auto& rightSet = std::get<std::set<int>>(v2);
    std::set<int> result;
    
    switch (exp->op) {
        case CUP_OP:  // Unión
            std::set_union(leftSet.begin(), leftSet.end(), rightSet.begin(), rightSet.end(),
                           std::inserter(result, result.begin()));
            break;
        case CAP_OP:  // Intersección
            std::set_intersection(leftSet.begin(), leftSet.end(), rightSet.begin(), rightSet.end(),
                                  std::inserter(result, result.begin()));
            break;
        case DIFF_OP:  // Diferencia (left - right)
            std::set_difference(leftSet.begin(), leftSet.end(), rightSet.begin(), rightSet.end(),
                                std::inserter(result, result.begin()));
            break;
    }
    return result;
}

Value PrintVisitor::visit(SetBinaryExp* exp) {
    exp->left->accept(this);
    string opStr;
    switch (exp->op) {
        case CUP_OP: opStr = " cup "; break;
        case CAP_OP: opStr = " cap "; break;
        case DIFF_OP: opStr = " \\ "; break;
    }
    cout << opStr;
    exp->right->accept(this);
    return 0;
}

Value EVALVisitor::visit(SetExp* exp) {
    std::set<int> result;
    for (auto elem : exp->elements) {
        Value val = elem->accept(this);
        result.insert(std::get<int>(val));
    }
    return result;
}

Value PrintVisitor::visit(SetExp* exp) {
    cout << "{";
    for (auto it = exp->elements.begin(); it != exp->elements.end(); ++it) {
        if (it != exp->elements.begin()) cout << ", ";
        (*it)->accept(this);
    }
    cout << "}";
    return 0;
}
