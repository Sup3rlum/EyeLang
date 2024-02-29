#include "Parser.h"

PResult<ImportStatementAST> Parser::ParseImportStatement()
{
    Mark();

    if (!tokens.Match(TokenType::KwImport))
        return NonMatch();

    NameAST* importName = ParseName(true);
    if (!importName)
        return Error("Expected identifier");

    if (!tokens.Match(TokenType::OpSemicolon))
        return Error("Expected ';' after import statement");

    return new ImportStatementAST(importName);
}

PResult<NamespaceDefAST> Parser::ParseNamespaceDefinition()
{
    Mark();

    if (!tokens.Match(TokenType::KwNamespace))
        return NonMatch();

    auto nsName = ParseName();
    if (!nsName)
        return Error("Expected identifier");

    if (!tokens.Match(TokenType::LCurly))
        return Error("Expected namespace body");

    ModuleStmntAST* moduleStmnt = 0;
    std::vector<ModuleStmntAST*> moduleStmnts;

    tokens.PushScope(nsName);
    {
        do
        {
            moduleStmnt = ParseModuleStatement();
            if (moduleStmnt)
            {
                moduleStmnts.push_back(moduleStmnt);
                continue;
            }

        } while (moduleStmnt);
    }
    tokens.PopScope();

    if (!tokens.Match(TokenType::RCurly))
        return Error("Expected '}' at the end of namespace");

    return new NamespaceDefAST{ nsName, moduleStmnts };

}

PResult<ModuleStmntAST> Parser::ParseModuleStatement()
{
    Mark();

    ModuleStmntAST* stmnt = ParseNamespaceDefinition();
    if (stmnt)
        return stmnt;
    
    stmnt = ParseModuleFunctionDefinition();
    if (stmnt)
        return stmnt;

    stmnt = ParseStructDefinition();
    if (stmnt)
        return stmnt;

    stmnt = ParseCompDefinition();
    if (stmnt)
        return stmnt;

    stmnt = ParseEnumDefinition();
    if (stmnt)
        return stmnt;

    return NonMatch();
}

