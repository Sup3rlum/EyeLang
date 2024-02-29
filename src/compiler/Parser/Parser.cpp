#include "Parser.h"

#define CHECK

PResult<AST> Parser::GenerateAST()
{
    std::vector<ImportStatementAST*> Imports;
    std::vector<ModuleStmntAST*> Statements;
    while (!tokens.EndOf())
    {

        if (auto importStmnt = ParseImportStatement())
        {
            Imports.push_back(importStmnt);
        }
        else if (auto stmnt = ParseModuleStatement())
        {
            Statements.push_back(stmnt);
        }
        else
        {
            return Error("Unrecognized statement");
        }

    }

    return new AST{ Imports, Statements };
}
