#include "Parser.h"

PResult<StructDefAST> Parser::ParseStructDefinition()
{
    Mark();

    if (!tokens.Match(TokenType::KwStruct))
        return NonMatch();

    auto structName = ParseName();
    if (!structName)
        return Error("Expected identifier");

    if (!tokens.Match(TokenType::LCurly))
        return Error("Expected struct body");

    MemberFuncDefAST* member = 0;
    VarDeclAST* field = 0;

    std::vector<MemberFuncDefAST*> members;
    std::vector<VarDeclAST*> fields;

    tokens.PushScope(structName);
    {
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

        } while (field || member);
    }
    tokens.PopScope();

    if (!tokens.Match(TokenType::RCurly))
        return Error("Expected '}' at the end of struct");

    return new StructDefAST{ structName, fields, members };
}
