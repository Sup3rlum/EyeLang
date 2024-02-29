#include "Lexer.h"
#include "Token.h"
#include <sstream>

SourceStream::SourceStream(const std::string& name, const std::string& input)
    : 
    sourceName(name), 
    input(input), 
    position(0), 
    currentLine(1), 
    currentLineChar(0)
{
}

bool SourceStream::IsLetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool SourceStream::IsDigit(char c)
{
    return (c >= '0' && c <= '9');
}
bool SourceStream::IsLetterOrDigit(char c)
{
    return IsLetter(c) || IsDigit(c);
}
bool SourceStream::IsAlphanumeric(char c)
{
    return IsLetterOrDigit(c) || c == '_';
}
bool SourceStream::IsWhiteSpace(char c)
{
    return (c == '\n' || c == '\r' || c == ' ' || c == '\t');
}

char SourceStream::GetNextChar()
{
    char c = PeekChar();
    if (c != -1)
    {
        position++;
        return c;
    }
    return -1;
}

char SourceStream::GetChar()
{
    return input[position];
}

char SourceStream::PeekChar()
{
    if (position + 1 >= input.length())
        return -1;
    return input[position + 1];
}

SourceLocation SourceStream::CurrLocation()
{
    return SourceLocation{ sourceName, currentLine, currentLineChar };
}

bool SourceStream::SkipWhitespace()
{
    bool skippedChars = false;
    char currentChar = GetChar();
    while (IsWhiteSpace(currentChar))
    {
        if (currentChar == '\n')
        {
            currentLine++;
            currentLineChar = 0;
        }
        currentChar = GetNextChar();
        skippedChars = true;
    }
    return skippedChars;
}

bool SourceStream::SkipComment()
{
    char currentChar = GetChar();
    char nextChar = PeekChar();
    bool skippedChars = false;

    if (currentChar == '/')
    {
        if (nextChar == '/')
        {
            while (currentChar != '\n' && currentChar != -1)
            {
                currentChar = GetNextChar();
                skippedChars = true;
            }
        }
        else if (nextChar == '*')
        {
            while (currentChar != '*' || nextChar != '/')
            {
                currentChar = GetNextChar();
                nextChar = PeekChar();
                skippedChars = true;
            }
            if (skippedChars)
            {
                GetNextChar();
                GetNextChar();
            }
        }
    }
    return skippedChars;
}

void SourceStream::SkipNonCode()
{
    bool skippedWhitespace = true;
    bool skippedComments = true;

    while (skippedComments || skippedWhitespace)
    {
        skippedComments = SkipComment();
        skippedWhitespace = SkipWhitespace();
    }
}

Token SourceStream::ReadToken()
{
    // Remove whitespaces and comments
    SkipNonCode();

    char currentChar = GetChar();
    std::string buffer = "";

    auto currentLocation = CurrLocation();

    // Start matching a word
    if (IsLetter(currentChar))
    {
        int wpos = position;
        char examinedChar = currentChar;
        while (IsAlphanumeric(examinedChar) && wpos < input.length())
        {
            buffer.push_back(examinedChar);
            examinedChar = input[++wpos];
        }

        position = wpos;
        TokenType tokenType;

        /* Match nay keywords */
        if (buffer == "if")
            tokenType = TokenType::KwIf;
        else if (buffer == "else")
            tokenType = TokenType::KwElse;
        else if (buffer == "struct")
            tokenType = TokenType::KwStruct;
        else if (buffer == "for")
            tokenType = TokenType::KwFor;
        else if (buffer == "while")
            tokenType = TokenType::KwWhile;
        else if (buffer == "comp")
            tokenType = TokenType::KwComp;
        else if (buffer == "return")
            tokenType = TokenType::KwReturn;
        else if (buffer == "void")
            tokenType = TokenType::KwVoid;
        else if (buffer == "constexpr")
            tokenType = TokenType::KwConstexpr;
        else if (buffer == "match")
            tokenType = TokenType::KwMatch;
        else if (buffer == "func")
            tokenType = TokenType::KwFunc;
        else if (buffer == "let")
            tokenType = TokenType::KwLet;
        else if (buffer == "const")
            tokenType = TokenType::KwConst;
        else if (buffer == "var")
            tokenType = TokenType::KwVar;
        else if (buffer == "enum")
            tokenType = TokenType::KwEnum;
        else if (buffer == "import")
            tokenType = TokenType::KwImport;
        else if (buffer == "namespace")
            tokenType = TokenType::KwNamespace;
        else if (buffer == "break")
            tokenType = TokenType::KwBreak;
        else if (buffer == "switch")
            tokenType = TokenType::KwSwitch;
        else if (buffer == "true")
            tokenType = TokenType::KwTrue;
        else if (buffer == "false")
            tokenType = TokenType::KwFalse;
        else
            tokenType = TokenType::Label;

        return Token{ currentLocation, tokenType, buffer };
    }
    // Match numeric data
    else if (IsDigit(currentChar))
    {
        bool foundDecimal = false;
        int wpos = position;
        char examinedChar = currentChar;
        while ((IsDigit(examinedChar) || (examinedChar == '.' && !foundDecimal)) && wpos < input.length())
        {
            if (examinedChar == '.')
                foundDecimal = true;
            buffer.push_back(examinedChar);
            examinedChar = input[++wpos];
        }
        position = wpos;
        return Token{ currentLocation, TokenType::Numeric, buffer };
    }
    // Match string literal
    else if (currentChar == '"' && position < input.length() - 1)
    {
        int wpos = ++position;
        char examinedChar = input[wpos];
        while (wpos < input.length())
        {
            buffer.push_back(examinedChar);
            examinedChar = input[++wpos];

            if (examinedChar == '"' && buffer[buffer.length() - 1] != '\\')
                break;
        }
        position = wpos+1;
        return Token{ currentLocation, TokenType::StringLit, buffer };
    }
    // Match character literal
    else if (currentChar == '\'' && position < input.length() - 1)
    {
        int wpos = ++position;
        char examinedChar = input[wpos];
        while (wpos < input.length())
        {
            buffer.push_back(examinedChar);
            examinedChar = input[++wpos];

            if (examinedChar == '\'' && buffer[buffer.length() - 1] != '\\')
                break;
        }
        position = wpos + 1;
        return Token{ currentLocation, TokenType::CharLit, buffer };
    }

    // Match Operators and brackets
    else
    {
        TokenType tokenType = TokenType::Unknown;
        position++;
        switch (currentChar)
        {
            /* Brackets */
        case '(':
            tokenType = TokenType::LParen;
            break;
        case '<':
            tokenType = TokenType::LTriangle;
            break;
        case '[':
            tokenType = TokenType::LSquare;
            break;
        case '{':
            tokenType = TokenType::LCurly;
            break;
        case ')':
            tokenType = TokenType::RParen;
            break;
        case '>':
            tokenType = TokenType::RTriangle;
            break;
        case ']':
            tokenType = TokenType::RSquare;
            break;
        case '}':
            tokenType = TokenType::RCurly;
            break;

            /* Operators */
        case '=':
            tokenType = TokenType::OpEqual;
            break;
        case '.':
            tokenType = TokenType::OpDot;
            break;
        case '+':
            tokenType = TokenType::OpPlus;
            break;
        case '-':
            tokenType = TokenType::OpMinus;
            break;
        case '*':
            tokenType = TokenType::OpAsterisk;
            break;
        case '/':
            tokenType = TokenType::OpSlash;
            break;
        case '!':
            tokenType = TokenType::OpExcl;
            break;
        case ',':
            tokenType = TokenType::OpComma;
            break;
        case ';':
            tokenType = TokenType::OpSemicolon;
            break;
        case ':':
            tokenType = TokenType::OpColon;
            break;
        case '&':
            tokenType = TokenType::OpAmpersand;
            break;
        case '|':
            tokenType = TokenType::OpLine;
            break;
        default:
            break;
        }

        return Token{ currentLocation, tokenType, std::string{currentChar} };
    }
}

bool TokenStream::MatchLabel(std::string& outName)
{
    if (tokens.size() - position < 1)
        return false;
    if (tokens[position].type != TokenType::Label)
        return false;
    outName = tokens[position].data;
    position++;
    return true;
}

bool TokenStream::MatchNumber(std::string& outValue, bool& outDecimal)
{
    if (tokens.size() - position < 1)
        return false;
    if (tokens[position].type != TokenType::Numeric)
        return false;
    outValue = tokens[position].data;

    for (char c : outValue)
        if (c == '.')
            outDecimal = true;

    position++;
    return true;
}

bool TokenStream::MatchStringLiteral(std::string& outValue)
{
    if (tokens.size() - position < 1)
        return false;
    if (tokens[position].type != TokenType::StringLit)
        return false;
    outValue = tokens[position].data;
    position++;
    return true;
}
bool TokenStream::MatchCharLiteral(char& outValue)
{
    if (tokens.size() - position < 1)
        return false;
    if (tokens[position].type != TokenType::CharLit)
        return false;
    outValue = tokens[position].data[0];
    position++;
    return true;
}

TokenStream Tokenizer::Tokenize(SourceStream& source)
{
    std::vector<Token> tokenList;

    while (!source.EndOf())
    {
        Token token = source.ReadToken();
        tokenList.push_back(token);
    }

    return TokenStream{ tokenList };
}
