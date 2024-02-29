#pragma once

#include "Common.h"

class NameAST;
class BlockAST;
class TypeAST;

class FuncParamAST : public ASTNode
{
public:
    NameAST* ParamName;
    TypeAST* TypeName;

    FuncParamAST(NameAST* name, TypeAST* typeName) : ParamName(name), TypeName(typeName)
    {
    }
    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module) override;
};

class FuncProtoAST : public ASTNode
{
public:
    TypeAST* RetType;
    NameAST* FuncName;
    std::vector<FuncParamAST*> Args;

    FuncProtoAST(TypeAST* ret, NameAST* funcName, const std::vector<FuncParamAST*>& args)
        : RetType(ret), FuncName(funcName), Args(args)
    {
    }
    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module) override;
};

class MemberFuncDefAST : public ASTNode
{
public:
    NameAST* ParentName;
    FuncProtoAST* FuncProto;
    BlockAST* Body;

    MemberFuncDefAST(NameAST* parent, FuncProtoAST* funcProto, BlockAST* body)
        : ParentName(parent), FuncProto(funcProto), Body(body)
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module) override;
};

class ModuleFuncDefAST : public ModuleStmntAST
{
public:
    FuncProtoAST* FuncProto;
    BlockAST* Body;

    ModuleFuncDefAST(FuncProtoAST* funcProto, BlockAST* body) : FuncProto(funcProto), Body(body)
    {
    }
    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module) override;
};
