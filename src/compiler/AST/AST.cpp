#include "AST.h"

ASTChildren AST::GetChildren()
{
    ASTChildren children{};
    for (auto imp : Imports)
        children.push_back(imp);
    for (auto stmnt : Statements)
        children.push_back(stmnt);

    return children;
}

ASTChildren BlockAST::GetChildren()
{
    ASTChildren children;
    for (auto stmnt : Statements)
        children.push_back(stmnt);

    return children;
}


ASTChildren StructDefAST::GetChildren()
{
    ASTChildren children{ StructName };
    for (auto field : Fields)
        children.push_back(field);
    for (auto member : Members)
        children.push_back(member);

    return children;
}

ASTChildren CompDefAST::GetChildren()
{
    ASTChildren children{ CompName };
    for (auto comp : Components)
        children.push_back(comp);
    for (auto field : Fields)
        children.push_back(field);
    for (auto member : Members)
        children.push_back(member);

    return children;
}

ASTChildren VarDeclAST::GetChildren()    { return ASTChildren{ VarName, TypeName, RHS }; }
ASTChildren ConstDeclAST::GetChildren()  { return ASTChildren{ VarName, TypeName, RHS }; }
ASTChildren IfStmntAST::GetChildren()       { return ASTChildren{ Condition, Body }; }
ASTChildren ForStmntAST::GetChildren()      { return ASTChildren{ Init, Condition, Iter, Body }; }
ASTChildren ForRangeStmntAST::GetChildren() { return ASTChildren{ Item, Container, Body }; }
ASTChildren WhileStmntAST::GetChildren()    { return ASTChildren{ Condition, Body }; }
ASTChildren AssignmentAST::GetChildren()        { return ASTChildren{ LHS, RHS }; }
ASTChildren ReturnStmntAST::GetChildren()   { return ASTChildren{ Expr }; }
ASTChildren ImportStatementAST::GetChildren()   { return ASTChildren{ ImportName }; }
ASTChildren ExprStmntAST::GetChildren()         { return ASTChildren{ Expr }; }

ASTChildren ModuleFuncDefAST::GetChildren()  { return ASTChildren{ FuncProto, Body }; }
ASTChildren MemberFuncDefAST::GetChildren()  { return ASTChildren{ ParentName, FuncProto, Body }; }
ASTChildren FuncParamAST::GetChildren() { return ASTChildren{ ParamName, TypeName }; }

ASTChildren FuncProtoAST::GetChildren()
{
    ASTChildren children { RetType, FuncName };
    for (auto arg : Args)
        children.push_back(arg);

    return children;
}