#pragma once

#include <string>

enum class TokenType : int
{

    /* Operators */
    OpEqual = 0,
    OpPlus,
    OpMinus,
    OpAsterisk,
    OpSlash,
    OpWedge,
    OpDot,
    OpComma,
    OpExcl,
    OpSquestion,
    OpSemicolon,
    OpColon,
    OpAmpersand,
    OpLine,

    /* KeyWords */
    KwIf,
    KwStruct,
    KwElse,
    KwFor,
    KwWhile,
    KwComp,
    KwReturn,
    KwVoid,
    KwConstexpr,
    KwMatch,
    KwFunc,
    KwConst,
    KwLet,
    KwVar,
    KwEnum,
    KwImport,
    KwNamespace,
    KwBreak,
    KwSwitch,
    KwTrue,
    KwFalse,

    Label,
    Numeric,
    StringLit,
    CharLit,

    /* Brackets */
    LParen,
    RParen,
    LSquare,
    RSquare,
    LCurly,
    RCurly,
    LTriangle,
    RTriangle,

    /* Unknown */
    Unknown
};

struct SourceLocation
{
    std::string sourceName;
    int line;
    int character;
};

struct Token
{
    SourceLocation location;
    TokenType type;
    std::string data;
};
