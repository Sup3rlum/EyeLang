#pragma once

#include "Common.h"

class ExprAST;


class ExprAST : public ASTNode
{
public:
    ExprAST()
    {
    }
    CGValue* Codegen(CModule* module) override;
};

class NumberAST : public ExprAST
{

public:
    enum class Type
    {
        Float32,
        Float64,
        Int32,
        Int64,
        UInt32,
        UInt64
    } NumType;

    std::string Value;
    NumberAST(std::string& value, Type type) : Value(value), NumType(type)
    {
    }

    CGValue* Codegen(CModule* module);
    ASTChildren GetChildren() { return ASTChildren(); }
    std::string GetValues() { return Value; }
};

class NameAST : public ExprAST
{
public:
    std::vector<std::string> Labels;
    NameAST(const std::vector<std::string>& labels) : Labels(labels)
    {
    }

    std::string GetBaseName()
    {
        return Labels[Labels.size() - 1];
    }

    ASTChildren GetChildren() { return ASTChildren(); }
    CGValue* Codegen(CModule* module);
    std::string GetValues()
    {
        std::string out = "";
        for (auto name : Labels)
        {
            out += name + " ";
        }
        return out;
    }
};

class StringLiteralAST : public ExprAST
{
public:
    std::string Value;
    StringLiteralAST(const std::string& value) : Value(value)
    {

    }

    ASTChildren GetChildren() { return ASTChildren(); }
    CGValue* Codegen(CModule* module);
    std::string GetValues() { return Value; }
};

class CharLiteralAST : public ExprAST
{
public:
    char Value;
    CharLiteralAST(char value) : Value(value)
    {

    }

    ASTChildren GetChildren() { return ASTChildren(); }
    CGValue* Codegen(CModule* module);
    std::string GetValues() { return { Value }; }
};

class BoolLiteralAST : public ExprAST
{
public:
    bool Value;
    BoolLiteralAST(bool value) : Value(value)
    {
    }

    ASTChildren GetChildren() { return ASTChildren(); }
    CGValue* Codegen(CModule* module);
    std::string GetValues() { return Value ? "true" : "false"; }
};

class ModuleStmntAST : public ASTNode
{
public:
    CGValue* Codegen(CModule* module);
};


class BinaryExprAST : public ExprAST
{
public:
    ExprAST* Left;
    ExprAST* Right;
    std::string Op;
    BinaryExprAST(ExprAST* left, ExprAST* right, const std::string& Op) : Left(left), Right(right), Op(Op)
    {

    }
    CGValue* Codegen(CModule* module) override;
    ASTChildren GetChildren() { return ASTChildren{ Left, Right }; }
    std::string GetValues() { return Op; }
};


class UnaryExprAST : public ExprAST
{
public:
    ExprAST* Inner;
    std::string Op;
    UnaryExprAST(ExprAST* inner, const std::string& Op) : Inner(inner), Op(Op)
    {

    }
    CGValue* Codegen(CModule* module) override;
    ASTChildren GetChildren() { return ASTChildren{ Inner }; }
    std::string GetValues() { return Op; }
};

class ParenExprAST : public ExprAST
{
public:
    ExprAST* Inner;
    ParenExprAST(ExprAST* inner) : Inner(inner)
    {

    }
    CGValue* Codegen(CModule* module) override;
    ASTChildren GetChildren() { return ASTChildren{ Inner }; }
};

/*
class AtomicExprAST : public ExprAST
{
public:
    AtomAST* atom;
    AtomicExprAST(AtomAST* atom) : atom(atom)
    {
    }

    CGValue* Codegen(CModule* module) override;
    ASTChildren GetChildren() { return ASTChildren{ atom }; }
};*/

class FunctionCallAST : public ExprAST
{
public:
    ExprAST* Callee;
    std::vector<ExprAST*> Arguments;
    FunctionCallAST(ExprAST* callee, const std::vector<ExprAST*>& args) : Callee(callee), Arguments(args)
    {
    }
    ~FunctionCallAST()
    {
    }

    ASTChildren GetChildren()
    {
        ASTChildren children { Callee };
        for (auto arg : Arguments)
            children.push_back(arg);

        return children;
    }

    CGValue* Codegen(CModule* module) override;
    std::string GetValues() { return "()"; }
};

class SubscriptAST : public ExprAST
{
public:
    ExprAST* Object;
    ExprAST* Index;
    SubscriptAST(ExprAST* object, ExprAST* index) : Object(object), Index(index)
    {
    }

    CGValue* Codegen(CModule* module) override;
    ASTChildren GetChildren() { return ASTChildren{ Object, Index }; }
};