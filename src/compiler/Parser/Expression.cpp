#include "Parser.h"

PResult<ExprAST> Parser::ParseExpression()
{
    Mark();
    auto pass = ParseAssignment();
    if (!pass)
        return NonMatch();

    return pass;
}

PResult<ExprAST> Parser::ParseAssignment()
{
    Mark();
    auto land = ParseLogicalOr();
    if (!land)
        return NonMatch();

    ExprAST* tail = NULL;
    bool op = tokens.Match(TokenType::OpEqual);
    if (op)
    {
        tail = ParseAssignment();
        if (!tail)
            return Error("Expected an expression");

        return new BinaryExprAST{ land, tail, "=" };
    }
    return land;
}

PResult<ExprAST> Parser::ParseLogicalOr()
{
    Mark();
    auto land = ParseLogicalAnd();
    if (!land)
        return NonMatch();

    ExprAST* tail = NULL;
    bool op = tokens.Match(TokenType::OpLine, TokenType::OpLine);
    if (op)
    {
        tail = ParseLogicalOr();
        if (!tail)
            return Error("Expected an expression");

        return new BinaryExprAST{ land, tail, "||" };
    }
    return land;
}
PResult<ExprAST> Parser::ParseLogicalAnd()
{
    Mark();
    auto eq = ParseEq();
    if (!eq)
        return NonMatch();

    ExprAST* tail = NULL;
    bool op = tokens.Match(TokenType::OpAmpersand, TokenType::OpAmpersand);
    if (op)
    {
        tail = ParseLogicalAnd();
        if (!tail)
            return Error("Expected an expression");

        return new BinaryExprAST{ eq, tail, "&&"};
    }

    return eq;
}
PResult<ExprAST> Parser::ParseEq()
{
    Mark();
    auto cond = ParseCond();
    if (!cond)
        return NonMatch();

    ExprAST* tail = NULL;
    bool neq = false;
    bool eq = tokens.Match(TokenType::OpEqual, TokenType::OpEqual);
    if (!eq)
        neq = tokens.Match(TokenType::OpExcl, TokenType::OpEqual);
    bool op = eq || neq;

    if (op)
    {
        tail = ParseEq();
        if (!tail)
            return Error("Expected an expression");

        return new BinaryExprAST{ cond, tail, "==" };
    }
    return cond;
}
PResult<ExprAST> Parser::ParseCond()
{
    Mark();
    auto sum = ParseSum();
    if (!sum)
        return NonMatch();

    ExprAST* tail = NULL;
    bool l = false;
    bool g = false;
    bool lt = false;
    bool gt = tokens.Match(TokenType::RTriangle, TokenType::OpEqual);
    if (!gt)
    {
        lt = tokens.Match(TokenType::LTriangle, TokenType::OpEqual);
        if (!lt)
        {
            g = tokens.Match(TokenType::RTriangle);
            if (!g)
            {
                l = tokens.Match(TokenType::LTriangle);
            }
        }
    }

    bool op = l || g || lt || gt;
    if (op)
    {
        tail = ParseCond();
        if (!tail)
            return Error("Expected an expression");

        return new BinaryExprAST{ sum, tail, "<" };
    }
    return sum;
}
PResult<ExprAST> Parser::ParseSum()
{
    Mark();
    auto term = ParseTerm();
    if (!term)
        return NonMatch();

    ExprAST* tail = NULL;
    bool minus = false;
    bool plus = tokens.Match(TokenType::OpPlus);
    if (!plus)
        minus = tokens.Match(TokenType::OpMinus);
    bool op = plus || minus;

    if (op)
    {
        tail = ParseSum();
        if (!tail)
            return Error("Expected an expression");

        return new BinaryExprAST{ term, tail, "+" };
    }
    return term;
}

PResult<ExprAST> Parser::ParseExprFactor()
{
    Mark();
    if (!tokens.Match(TokenType::LParen))
        return NonMatch();

    auto expr = ParseExpression();
    if (!expr)
        return Error("Expected an expression");

    if (!tokens.Match(TokenType::RParen))
        return Error("Expected ')'");

    return new ParenExprAST{ expr };
}

PResult<ExprAST> Parser::ParseTerm()
{
    Mark();
    auto member = ParsePreOperator();
    if (!member)
        return NonMatch();

    ExprAST* tail = NULL;
    bool div = false;
    bool mul = tokens.Match(TokenType::OpAsterisk);
    if (!mul)
        div = tokens.Match(TokenType::OpSlash);
    bool op = mul || div;

    if (op)
    {
        tail = ParseTerm();
        if (!tail)
            return Error("Expected an expression");

        return new BinaryExprAST{ member, tail, "*" };
    }
    return member;
}

PResult<ExprAST> Parser::ParsePreOperator()
{
    Mark();

    bool decr = false;
    bool incr = tokens.Match(TokenType::OpPlus, TokenType::OpPlus);
    if (!incr)
        decr = tokens.Match(TokenType::OpMinus, TokenType::OpMinus);
    bool op = incr || decr;

    if (op)
    {
        auto inner = ParsePreOperator();
        if (!inner)
            return Error("Expected an expression");

        return new UnaryExprAST{ inner, "++" };
    }
    else
    {
        auto inner = ParseMemberAccess();
        if (!inner)
            return NonMatch();

        return inner;
    }

    return NonMatch();
}


PResult<ExprAST> Parser::ParsePostOperator()
{
    return NonMatch();
}

PResult<ExprAST> Parser::ParseCompound()
{
    return NonMatch();
}

PResult<ExprAST> Parser::ParseMemberAccess()
{
    Mark();
    auto funcCall = ParseFunctionCall();
    if (!funcCall)
        return NonMatch();

    ExprAST* tail = NULL;
    bool dot = tokens.Match(TokenType::OpDot);

    if (dot)
    {
        tail = ParseMemberAccess();
        if (!tail)
            return Error("Expected an expression");

        return new BinaryExprAST{ funcCall, tail, "." };
    }
    return funcCall;
}

PResult<ExprAST> Parser::ParseFunctionCall()
{
    Mark();

    auto subscript = ParseSubscript();
    if (!subscript)
        return NonMatch();

    std::vector<ExprAST*> inputArguments;
    if (!tokens.Match(TokenType::LParen))
        return subscript;

    auto inputArg = ParseExpression();
    if (inputArg) // Non empty function call arguments
    {
        inputArguments.push_back(inputArg);
        bool argComma = tokens.Match(TokenType::OpComma);
        if (argComma) // More than one argument
        {
            inputArg = ParseExpression();
            if (!inputArg)
                return Error("Expected an expression");
            inputArguments.push_back(inputArg);

            while (inputArg && argComma)
            {
                argComma = tokens.Match(TokenType::OpComma);
                inputArg = ParseExpression();

                if (argComma && !inputArg)
                    return Error("Expected an expression");
                if (inputArg)
                    inputArguments.push_back(inputArg);
            }
        }
    }

    if (!tokens.Match(TokenType::RParen))
        return Error("Expected a ')'");

    return new FunctionCallAST{ subscript, inputArguments };
}


PResult<ExprAST> Parser::ParseSubscript()
{

    Mark();
    auto factor = ParseFactor();
    if (!factor)
        return NonMatch();

    ExprAST* index = NULL;
    bool lsquare = tokens.Match(TokenType::LSquare);

    if (lsquare)
    {
        index = ParseExpression();
        if (!index)
            return Error("Expected an expression");

        bool rsquare = tokens.Match(TokenType::RSquare);
        if (!rsquare)
            return Error("Expected ']'");

        return new SubscriptAST{ factor, index };
    }
    return factor;

}


PResult<ExprAST> Parser::ParseFactor()
{
    Mark();
    auto exprFactor = ParseExprFactor();
    if (exprFactor)
        return exprFactor;

    auto atomFactor = ParseAtomicFactor();
    if (atomFactor)
        return atomFactor;

    return NonMatch();
}
PResult<ExprAST> Parser::ParseAtomicFactor()
{
    Mark();

    auto number = ParseNumber();
    if (number)
        return (ExprAST*)number;

    auto name = ParseName(true);
    if (name)
        return (ExprAST*)name;

    auto str = ParseStringLiteral();
    if (str)
        return (ExprAST*)str;

    auto ch = ParseCharLiteral();
    if (ch)
        return (ExprAST*)ch;

    auto bl = ParseBoolLiteral();
    if (bl)
        return (ExprAST*)bl;

    return NonMatch();
}

PResult<NumberAST> Parser::ParseNumber()
{
    Mark();
    std::string numVal = "";
    bool decimal = false;
    auto number = tokens.MatchNumber(numVal, decimal);
    if (!number)
        return NonMatch();

    auto t = NumberAST::Type::Int32;
    std::string suffix = "";
    bool hasSuffix = tokens.MatchLabel(suffix);

    if (!hasSuffix && decimal)
        t = NumberAST::Type::Float64;

    if (hasSuffix)
    {
        if (suffix == "d" || suffix == "D")
            t = NumberAST::Type::Float64;
        else if (suffix == "f" || suffix == "F")
            t = NumberAST::Type::Float32;
        else if (suffix == "l" || suffix == "L")
            t = NumberAST::Type::Int64;
        else if (suffix == "u" || suffix == "U")
            t = NumberAST::Type::UInt32;
        else if (suffix == "ul" || suffix == "UL")
            t = NumberAST::Type::UInt64;
    }

    return new NumberAST{ numVal, t };
}

PResult<NameAST> Parser::ParseName(bool bScopable)
{
    Mark();

    std::vector<std::string> labels;
    std::string name = "";
    auto nameLabel = tokens.MatchLabel(name);
    if (!nameLabel)
        return NonMatch();

    labels.push_back(name);

    if (bScopable)
    {
        do
        {
            if (!tokens.Match(TokenType::OpColon, TokenType::OpColon))
                break;

            nameLabel = tokens.MatchLabel(name);
            if (!nameLabel)
                return Error("Expected identifier");

            labels.push_back(name);

        } while (nameLabel);
    }
    return new NameAST{ labels };
}

PResult<StringLiteralAST> Parser::ParseStringLiteral()
{
    Mark();
    std::string value = "";
    auto stringLit = tokens.MatchStringLiteral(value);
    if (!stringLit)
        return NonMatch();

    return new StringLiteralAST{ value };
}
PResult<CharLiteralAST> Parser::ParseCharLiteral()
{
    Mark();
    char value = '\0';
    auto stringLit = tokens.MatchCharLiteral(value);
    if (!stringLit)
        return NonMatch();

    return new CharLiteralAST{ value };
}

PResult<BoolLiteralAST> Parser::ParseBoolLiteral()
{
    Mark();
    bool value = true;

    if (tokens.Match(TokenType::KwTrue))
        value = true;
    else if (tokens.Match(TokenType::KwFalse))
        value = false;
    else
        return NonMatch();

    return new BoolLiteralAST{ value };
}