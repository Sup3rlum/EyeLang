#pragma once

#include "../Common/Logger.h"
#include "../Lexer/Lexer.h"
#include "../AST/AST.h"
#include <fstream>
#include <iostream>
#include <span>
#include <vector>
#include <map>


template<LogSeverity logSeverity>
struct ParserLogMessage : LogMessage
{
    SourceLocation srcLoc;
    std::string message;
    ParserLogMessage(const SourceLocation& src, const std::string& str) : srcLoc(src), message(str), LogMessage(logSeverity) {}

    std::string GetStr() 
    { 
        return srcLoc.sourceName + std::string(":") + std::to_string(srcLoc.line) + ":" + std::to_string(srcLoc.character) + std::string(": ") + message; }
};

typedef ParserLogMessage<LogSeverity::Info> ParserInfo;
typedef ParserLogMessage<LogSeverity::Warning> ParserWarning;
typedef ParserLogMessage<LogSeverity::Error> ParserError;

template <typename TNode> struct PResult
{
    TNode* Ptr;
    PResult() : Ptr(NULL)
    {
    }
    PResult(TNode* ptr) : Ptr(ptr)
    {
    }
    PResult(int i) : Ptr((TNode*)i)
    {
    }

    operator bool()
    {
        return bool(Ptr);
    }
    operator TNode* ()
    {
        return Ptr;
    }
};
typedef PResult<int> PResultNone;

class Parser
{
private:
    Logger* logger;
    TokenStream tokens;

    PResultNone Error(const std::string& str)
    {
        logger->Log(new ParserError{ tokens.CurrentToken().location, str });
        return PResultNone{0};
    }
    PResultNone Warning(const std::string& str)
    {
        logger->Log(new ParserWarning{ tokens.CurrentToken().location, str });
        return PResultNone{0};
    }

    PResultNone NonMatch()
    {
        tokens.Backtrack();
        return PResultNone{0};
    }

    void Mark()
    {
        tokens.Mark();
    }

public:
    Parser(const TokenStream& tokenStream, Logger* logger = new BasicLogger()) : tokens(tokenStream), logger(logger)
    {
    }

    PResult<AST> GenerateAST();
    PResult<BlockAST> ParseBlock();

    /* Statements */
    PResult<StmntAST> ParseStatement();
    PResult<StmntAST> ParseBlockStatement();
    PResult<StmntAST> ParseNormalStatement();

    PResult<IfStmntAST> ParseIf();
    PResult<WhileStmntAST> ParseWhile();
    PResult<ForStmntAST> ParseFor();
    PResult<ReturnStmntAST> ParseReturn();
    PResult<VarDeclAST> ParseVarDeclaration();
    PResult<ConstDeclAST> ParseConstDeclaration();
    PResult<ExprStmntAST> ParseExprStatement();
    PResult<BreakStmntAST> ParseBreakStatement();

    /* Types */
    PResult<TypeAST> ParseType();

    /* Top Level  & Function*/
    PResult<FuncProtoAST> ParseFunctionProto();
    PResult<FuncParamAST> ParseFunctionParameter();
    PResult<ModuleStmntAST> ParseModuleStatement();
    PResult<ImportStatementAST> ParseImportStatement();
    PResult<ModuleFuncDefAST> ParseModuleFunctionDefinition();
    PResult<MemberFuncDefAST> ParseMemberFunctionDefinition();
    PResult<StructDefAST> ParseStructDefinition();
    PResult<CompDefAST> ParseCompDefinition();
    PResult<EnumDefAST> ParseEnumDefinition();
    PResult<NamespaceDefAST> ParseNamespaceDefinition();

    /* Expression */
    PResult<ExprAST> ParseExpression();

    /* Binary operators */
    PResult<ExprAST> ParseAssignment();
    PResult<ExprAST> ParseLogicalOr();
    PResult<ExprAST> ParseLogicalAnd();
    PResult<ExprAST> ParseEq();
    PResult<ExprAST> ParseCond();
    PResult<ExprAST> ParseSum();
    PResult<ExprAST> ParseTerm();
    PResult<ExprAST> ParseFactor();
    PResult<ExprAST> ParseCompound();

    /* Unary Operators */
    PResult<ExprAST> ParsePreOperator();
    PResult<ExprAST> ParsePostOperator();

    /* Other */
    PResult<ExprAST> ParseMemberAccess();
    PResult<ExprAST> ParseSubscript();
    PResult<ExprAST> ParseFunctionCall();
    PResult<ExprAST> ParseAtomicFactor();
    PResult<ExprAST> ParseExprFactor();

    /* Building blocks */

    PResult<NumberAST> ParseNumber();
    PResult<NameAST> ParseName(bool bScopable=false);
    PResult<StringLiteralAST> ParseStringLiteral();
    PResult<CharLiteralAST> ParseCharLiteral();
    PResult<BoolLiteralAST> ParseBoolLiteral();
};
