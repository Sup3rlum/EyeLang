#include "Parser.h"

PResult<FuncProtoAST> Parser::ParseFunctionProto()
{
    Mark();

    if (!tokens.Match(TokenType::KwFunc))
        return NonMatch();

    auto funcName = ParseName();
    if (!funcName)
        return Error("Expected an identifier");

    if (!tokens.Match(TokenType::LParen))
        return Error("Expected a '('");

    std::vector<FuncParamAST*> inputParameters;

    auto inputParam = ParseFunctionParameter();
    if (inputParam) // Non empty function call arguments
    {
        inputParameters.push_back(inputParam);
        bool paramComma = tokens.Match(TokenType::OpComma);
        if (paramComma) // More than one argument
        {
            inputParam = ParseFunctionParameter();
            if (!inputParam)
                return Error("Expected a parameter");
            inputParameters.push_back(inputParam);

            while (inputParam && paramComma)
            {
                paramComma = tokens.Match(TokenType::OpComma);
                inputParam = ParseFunctionParameter();

                if (paramComma && !inputParam)
                    return Error("Expected a parameter");
                if (inputParam)
                    inputParameters.push_back(inputParam);
            }
        }
    }

    bool retArrow = tokens.Match(TokenType::RParen, TokenType::OpMinus, TokenType::RTriangle);
    if (!retArrow)
        return Error("Expected a return type");

    auto retTypeName = ParseType();
    if (!retTypeName)
        return Error("Expected an identifier");

    return new FuncProtoAST{ retTypeName, funcName, inputParameters };
}


PResult<FuncParamAST> Parser::ParseFunctionParameter()
{
    Mark();

    auto paramName = ParseName();
    if (!paramName)
        return NonMatch();

    if (!tokens.Match(TokenType::OpColon))
        return Error("Expected a ':'");

    auto typeName = ParseType();
    if (!typeName)
        return Error("Expected an identifier");

    return new FuncParamAST{ paramName, typeName };
}

PResult<ModuleFuncDefAST> Parser::ParseModuleFunctionDefinition()
{
    Mark();

    auto proto = ParseFunctionProto();
    if (!proto)
        return NonMatch();

    auto body = ParseBlock();
    if (!body)
        return Error("Expected a function body");

    return new ModuleFuncDefAST{ proto, body };
}

PResult<MemberFuncDefAST> Parser::ParseMemberFunctionDefinition()
{
    Mark();

    auto proto = ParseFunctionProto();
    if (!proto)
        return NonMatch();

    auto body = ParseBlock();
    if (!body)
        return Error("Expected a function body");

    return new MemberFuncDefAST{ tokens.TopScope(), proto, body };
}
