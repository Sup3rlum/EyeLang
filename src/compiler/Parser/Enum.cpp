#include "Parser.h"

PResult<EnumDefAST> Parser::ParseEnumDefinition()
{
    Mark();

    if (!tokens.Match(TokenType::KwEnum))
        return NonMatch();

    auto enumName = ParseName();
    if (!enumName)
        return Error("Expected an identifier");

    if (!tokens.Match(TokenType::LCurly))
        return Error("Expected '{'");

    NameAST* entry = ParseName();
    if (!entry)
        return Error("Expected identifier");

    std::vector<NameAST*> values;

    auto argComma = tokens.Match(TokenType::OpComma);
    if (argComma)
    {
        entry = ParseName();
        if (!entry)
            return Error("Expected an identifier");

        values.push_back(entry);

        while (entry && argComma)
        {
            argComma = tokens.Match(TokenType::OpComma);
            entry = ParseName();

            if (argComma && !entry)
                return Error("Expected an identifier");
            if (entry)
                values.push_back(entry);
        }
    }

    if (!tokens.Match(TokenType::RCurly))
        return Error("Expected '}' at the end of enum");

    return new EnumDefAST{ enumName, values };
}
