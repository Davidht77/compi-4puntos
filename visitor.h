#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>

class BinaryExp;
class NumberExp;
class SqrtExp;
class Program;
class PrintStm;
class AssignStm;
class SetBinaryExp;
class SetExp;

class Visitor {
public:
    virtual Value visit(BinaryExp* exp) = 0;
    virtual Value visit(NumberExp* exp) = 0;
    virtual Value visit(IdExp* exp) = 0;
    virtual Value visit(SqrtExp* exp) = 0;
    virtual Value visit(Program* exp) = 0;
    virtual Value visit(PrintStm* exp) = 0;
    virtual Value visit(AssignStm* exp) = 0;
    virtual Value visit(SetBinaryExp* exp) = 0;
    virtual Value visit(SetExp* exp) = 0;
};

class PrintVisitor : public Visitor {
public:

    Value visit(BinaryExp* exp) override;
    Value visit(NumberExp* exp) override;
    Value visit(SqrtExp* exp) override;
    Value visit(PrintStm* exp) override;
    Value visit(AssignStm* exp) override;
    Value visit(Program* exp) override;
    Value visit(IdExp* exp) override;
    Value visit(SetBinaryExp* exp) override;
    Value visit(SetExp* exp) override;
    void imprimir(Program* program);
};

class EVALVisitor : public Visitor {
public:
    Value visit(BinaryExp* exp) override;
    Value visit(NumberExp* exp) override;
    Value visit(SqrtExp* exp) override;
    Value visit(PrintStm* exp) override;
    Value visit(IdExp* exp) override;
    Value visit(AssignStm* exp) override;
    Value visit(Program* exp) override;
    Value visit(SetBinaryExp* exp) override;
    Value visit(SetExp* exp) override;
    void interprete(Program* program);
};



#endif // VISITOR_H
