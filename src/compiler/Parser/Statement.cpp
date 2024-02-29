#include "Parser.h"

PResult<BlockAST> Parser::ParseBlock()
{
    Mark();
    bool matchOpenCurly = tokens.Match(TokenType::LCurly);
    if (!matchOpenCurly)
        return NonMatch();

    std::vector<StmntAST*> statements;
    auto parsedStatement = ParseStatement();

    while (parsedStatement)
    {
        statements.push_back(parsedStatement);
        parsedStatement = ParseStatement();
        
        if (!parsedStatement)
        {
            bool matchCloseCurly = tokens.Match(TokenType::RCurly);
            if (!matchCloseCurly)
                return Error("Unexpected token at end of block");

            break;
        }
    }


    return new BlockAST{ statements };
}

PResult<StmntAST> Parser::ParseBlockStatement()
{
    Mark();
    StmntAST* stmnt = ParseIf();
    if (stmnt)
        return stmnt;

    stmnt = ParseFor();
    if (stmnt)
        return stmnt;

    stmnt = ParseWhile();
    if (stmnt)
        return stmnt;

    return NonMatch();
}

PResult<ExprStmntAST> Parser::ParseExprStatement()
{
    Mark();

    auto expr = ParseExpression();
    if (!expr)
        return NonMatch();

    return new ExprStmntAST{ expr };
}

PResult<StmntAST> Parser::ParseNormalStatement()
{
    Mark();

    StmntAST* stmnt = ParseExprStatement();
    if (stmnt)
        return stmnt;
        
    stmnt = ParseVarDeclaration();
    if (stmnt)
        return stmnt;

    stmnt = ParseConstDeclaration();
    if (stmnt)
        return stmnt;

    stmnt = ParseReturn();
    if (stmnt)
        return stmnt;

    stmnt = ParseBreakStatement();
    if (stmnt)
        return stmnt;

    return NonMatch();
}

PResult<StmntAST> Parser::ParseStatement()
{
    Mark();

    StmntAST* stmnt = ParseBlockStatement();
    if (stmnt)
        return stmnt;

    stmnt = ParseNormalStatement();
    if (stmnt)
    {
        auto semicolon = tokens.Match(TokenType::OpSemicolon);
        if (!semicolon)
            return Error("Expected ';'");
        return stmnt;
    }
    return NonMatch();
}

PResult<IfStmntAST> Parser::ParseIf()
{
    Mark();
    bool matchStartCond = tokens.Match(TokenType::KwIf, TokenType::LParen);
    if (!matchStartCond)
        return NonMatch();

    auto condition = ParseExpression();
    if (!condition)
        return Error("Expected an expression");

    bool matchEndCond = tokens.Match(TokenType::RParen);
    if (!matchEndCond)
        return Error(")");

    auto block = ParseBlock();
    if (!block)
        return Error("Expected a body");

    return new IfStmntAST{ condition, block };
}

PResult<WhileStmntAST> Parser::ParseWhile()
{
    Mark();
    bool matchStartCond = tokens.Match(TokenType::KwWhile, TokenType::LParen);
    if (!matchStartCond)
        return NonMatch();

    auto condition = ParseExpression();
    if (!condition)
        return Error("Expected an expression");

    bool matchEndCond = tokens.Match(TokenType::RParen);
    if (!matchEndCond)
        return Error(")");

    auto block = ParseBlock();
    if (!block)
        return Error("Expected a body");

    return new WhileStmntAST{ condition, block };
}

PResult<ForStmntAST> Parser::ParseFor()
{
    Mark();
    bool matchStartCond = tokens.Match(TokenType::KwFor, TokenType::LParen);
    if (!matchStartCond)
        return NonMatch();

    auto init = ParseStatement();
    if (!init)
        return Error("Expected initializer statement");

    bool firstSemicolon = tokens.Match(TokenType::OpSemicolon);
    if (!firstSemicolon)
        return Error("Expected ';'");

    auto cond = ParseExpression();
    if (!cond)
        return Error("Expected an expression");

    bool secondSemicolon = tokens.Match(TokenType::OpSemicolon);
    if (!secondSemicolon)
        return Error("Expected ';'");

    auto iter = ParseStatement();
    if (!iter)
        return Error("Expected an iterator statement");

    bool matchEndCond = tokens.Match(TokenType::RParen);
    if (!matchEndCond)
        return Error("Expected ')'");

    auto block = ParseBlock();
    if (!block)
        return Error("Expected a body");

    return new ForStmntAST{ init, cond, iter, block };
}

PResult<ReturnStmntAST> Parser::ParseReturn()
{
    Mark();
    if (!tokens.Match(TokenType::KwReturn))
        return NonMatch();

    auto expr = ParseExpression();
    if (!expr)
        return Error("Expected an expression");

    return new ReturnStmntAST{ expr };
}

PResult<BreakStmntAST> Parser::ParseBreakStatement()
{
    Mark();
    if (!tokens.Match(TokenType::KwReturn))
        return NonMatch();

    return new BreakStmntAST{};
}

PResult<VarDeclAST> Parser::ParseVarDeclaration()
{
    Mark();

    bool varKw = tokens.Match(TokenType::KwVar);
    if (!varKw)
        return NonMatch();

    auto varName = ParseName();
    if (!varName)
        return Error("Expected an identifier");

    if (!tokens.Match(TokenType::OpColon))
        return Error("Expected ':' after var declaration");

    auto varTypeName = ParseType();
    if (!varTypeName)
        return Error("Expected type identifier");

    ExprAST* rhs = NULL;
    if (tokens.Match(TokenType::OpEqual))
    {
        rhs = ParseExpression();
        if (!rhs)
            return Error("Expected an expression as assignment");
    }

    return new VarDeclAST{ varName, varTypeName, rhs };
}


PResult<ConstDeclAST> Parser::ParseConstDeclaration()
{
    Mark();

    bool varKw = tokens.Match(TokenType::KwConst);
    if (!varKw)
        return NonMatch();

    auto varName = ParseName();
    if (!varName)
        return Error("Expected an identifier");

    TypeAST* varTypeName = NULL;
    if (tokens.Match(TokenType::OpColon))
    {
        //return Error("Expected ':' after var declaration");

        varTypeName = ParseType();
        if (!varTypeName)
            return Error("Expected type identifier");

    }

    if (!tokens.Match(TokenType::OpEqual))
        return Error("Must assign to const at declaration");

    ExprAST* rhs = ParseExpression();
    if (!rhs)
        return Error("Expected an expression as assignment");

    return new ConstDeclAST{ varName, varTypeName, rhs };
}
