#include "Parser.h"

PResult<CompDefAST> Parser::ParseCompDefinition()
{
    Mark();

    if (!tokens.Match(TokenType::KwComp))
        return NonMatch();

    auto compositionName = ParseName();
    if (!tokens.Match(TokenType::LSquare))
        return Error("Expected '['");

    std::vector<NameAST*> CompNames;
    NameAST* componentName = ParseName();
    if (!componentName)
        return Error("Expected component identifier");
    CompNames.push_back(componentName);

    bool compComma = tokens.Match(TokenType::OpComma);
    if (compComma) // More than one component
    {
        componentName = ParseName();
        if (!componentName)
            return Error("Expected a component identifier");
        CompNames.push_back(componentName);

        while (componentName && compComma)
        {
            compComma = tokens.Match(TokenType::OpComma);
            componentName = ParseName();

            if (compComma && !componentName)
                return Error("Expected a component identifier");
            if (componentName)
                CompNames.push_back(componentName);
        }
    }

    if (!tokens.Match(TokenType::RSquare))
        return Error("Expected ']'");
    if (!tokens.Match(TokenType::LCurly))
        return Error("Expected struct body");

    MemberFuncDefAST* member = 0;
    VarDeclAST* field = 0;
    std::vector<MemberFuncDefAST*> members;
    std::vector<VarDeclAST*> fields;

    tokens.PushScope(compositionName);
    do
    {
        member = ParseMemberFunctionDefinition();
        if (member)
        {
            members.push_back(member);
            continue;
        }

        field = ParseVarDeclaration();
        if (field)
        {
            if (!tokens.Match(TokenType::OpSemicolon))
                return Error("Expected ';'");
            fields.push_back(field);
            continue;
        }

    } while (member || field);
    tokens.PopScope();

    if (!tokens.Match(TokenType::RCurly))
        return Error("Expected '}' at the end of composition");

    return new CompDefAST{ compositionName, CompNames, fields, members };
}
