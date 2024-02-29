#pragma once

#include "Common.h"

/*
 *   Regular statements;
 */

class NameAST;
class ExprAST;
class BlockAST;
class TypeAST;
class CModule;

class StmntAST : public ASTNode
{
public:
    CGValue* Codegen(CModule* module);
};

class VarDeclAST : public StmntAST
{
public:
    NameAST* VarName;
    TypeAST* TypeName;
    ExprAST* RHS;
    VarDeclAST(NameAST* name, TypeAST* typeName, ExprAST* init) : VarName(name), TypeName(typeName), RHS(init)
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class ConstDeclAST : public StmntAST
{
public:
    NameAST* VarName;
    TypeAST* TypeName;
    ExprAST* RHS;
    ConstDeclAST(NameAST* name, TypeAST* typeName, ExprAST* init) : VarName(name), TypeName(typeName), RHS(init)
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class IfStmntAST : public StmntAST
{
public:
    ExprAST* Condition;
    BlockAST* Body;
    IfStmntAST(ExprAST* cond, BlockAST* body) : Condition(cond), Body(body)
    {
    }
    ~IfStmntAST()
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class ForStmntAST : public StmntAST
{
public:
    StmntAST* Init;
    ExprAST* Condition;
    StmntAST* Iter;
    BlockAST* Body;

    ForStmntAST(StmntAST* init, ExprAST* cond, StmntAST* iter, BlockAST* body)
        : Init(init), Condition(cond), Body(body), Iter(iter)
    {
    }

    ~ForStmntAST()
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class ForRangeStmntAST : public StmntAST
{
public:
    StmntAST* Item;
    StmntAST* Container;
    BlockAST* Body;

    ForRangeStmntAST(StmntAST* item, StmntAST* container, BlockAST* body)
        : Item(item), Container(container), Body(body)
    {
    }

    ~ForRangeStmntAST()
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class WhileStmntAST : public StmntAST
{
public:
    ExprAST* Condition;
    BlockAST* Body;

    WhileStmntAST(ExprAST* cond, BlockAST* body) : Condition(cond), Body(body)
    {
    }

    ~WhileStmntAST()
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class ExprStmntAST : public StmntAST
{
public:
    ExprAST* Expr;
    ExprStmntAST(ExprAST* expr) : Expr(expr)
    {

    }
    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class AssignmentAST : public StmntAST
{
public:
    ExprAST* LHS;
    ExprAST* RHS;

    AssignmentAST(ExprAST* lhs, ExprAST* rhs) : LHS(lhs), RHS(rhs)
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class ReturnStmntAST : public StmntAST
{
public:
    ExprAST* Expr;
    ReturnStmntAST(ExprAST* expr) : Expr(expr)
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class BreakStmntAST : public StmntAST
{
public:
    BreakStmntAST()
    {
    }

    ASTChildren GetChildren() { return ASTChildren(); }
    CGValue* Codegen(CModule* module);
};