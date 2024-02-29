#pragma once

#include "Common.h"

class MemberFuncDefAST;
class VarDeclAST;

class NamespaceDefAST : public ModuleStmntAST
{
public:
    NameAST* Name;
    std::vector<ModuleStmntAST*> Stmnts;
    NamespaceDefAST(NameAST* name, std::vector<ModuleStmntAST*> stmnts) : Name(name), Stmnts(stmnts)
    {

    }
    ASTChildren GetChildren()
    {
        ASTChildren children{ Name };
        for (auto val : Stmnts)
            children.push_back(val);

        return children;
    }

    CGValue* Codegen(CModule* module);
};

class EnumDefAST : public ModuleStmntAST
{
public:
    NameAST* EnumName;
    std::vector<NameAST*> Values;
    EnumDefAST(NameAST* enumName, const std::vector<NameAST*> values) : EnumName(enumName), Values(values)
    {
    }
    ~EnumDefAST()
    {
    }

    ASTChildren GetChildren()
    {
        ASTChildren children = { EnumName };
        for (auto val : Values)
            children.push_back(val);

        return children;
    }

    CGValue* Codegen(CModule* module);
};

class StructDefAST : public ModuleStmntAST
{
public:
    NameAST* StructName;
    std::vector<VarDeclAST*> Fields;
    std::vector<MemberFuncDefAST*> Members;

    StructDefAST(
        NameAST* name, 
        const std::vector<VarDeclAST*>& fields,
        const std::vector<MemberFuncDefAST*>& members
    ) : StructName(name), 
        Fields(fields), 
        Members(members)
    {
    }
    ~StructDefAST()
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class CompDefAST : public ModuleStmntAST
{
public:
    NameAST* CompName;
    std::vector<NameAST*> Components;
    std::vector<VarDeclAST*> Fields;
    std::vector<MemberFuncDefAST*> Members;

    CompDefAST(
        NameAST* compName, 
        const std::vector<NameAST*>& components, 
        const std::vector<VarDeclAST*>& fields,
        const std::vector<MemberFuncDefAST*>& members
    ) : CompName(compName), 
        Components(components), 
        Fields(fields), 
        Members(members)
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};

class ImportStatementAST : public ASTNode
{
public:
    NameAST* ImportName;
    ImportStatementAST(NameAST* importName) : ImportName(importName)
    {

    }
    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};