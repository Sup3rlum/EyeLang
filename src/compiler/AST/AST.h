#pragma once

#include "Common.h"
#include "Expression.h"
#include "Function.h"
#include "Module.h"
#include "Statement.h"
#include "Block.h"
#include "Type.h"

class AST : public ASTNode
{
public:
    std::vector<ImportStatementAST*> Imports;
    std::vector<ModuleStmntAST*> Statements;
    AST(const std::vector<ImportStatementAST*> imports,const std::vector<ModuleStmntAST*> stmnts) : Imports(imports), Statements(stmnts)
    {
    }
    CGValue* Codegen(CModule* module);
    ASTChildren GetChildren();
};
