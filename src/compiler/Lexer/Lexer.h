#pragma once

#include "Token.h"
#include <iostream>
#include <stack>
#include <string_view>
#include <unordered_map>
#include <vector>

class NameAST;

class SourceStream
{

private:
    // SourceLines lines;
    std::string sourceName;
    std::string input;
    int currentLine;
    int currentLineChar;
    int position;

public:
    SourceStream(const std::string& name, const std::string& input);
    Token ReadToken();
    SourceLocation CurrLocation();

    char GetChar();
    char GetNextChar();
    char PeekChar();

    bool EndOf()
    {
        return position >= input.length() - 1;
    }

    bool IsLetter(char);
    bool IsLetterOrDigit(char);
    bool IsDigit(char);
    bool IsAlphanumeric(char);
    bool IsWhiteSpace(char);

    bool SkipWhitespace();
    bool SkipComment();
    void SkipNonCode();
};

class TokenStream
{
public:
    TokenStream(const std::vector<Token>& tokens) : tokens(tokens), position(0)
    {
    }
    // bool Match(const std::vector<TokenType>& str);

    template <typename... TArgs> bool Match(TArgs... toks)
    {
        const std::vector<TokenType>& str = { toks... };

        if (tokens.size() - position < str.size())
            return false;
        for (int i = 0; i < str.size(); i++)
        {
            if (tokens[position + i].type != str[i])
                return false;
        }
        position += str.size();
        return true;
    }

    bool Match(const std::vector<TokenType>& str)
    {
        if (tokens.size() - position < str.size())
            return false;
        for (int i = 0; i < str.size(); i++)
        {
            if (tokens[position + i].type != str[i])
                return false;
        }
        position += str.size();
        return true;
    }

    bool MatchLabel(std::string& outName);
    bool MatchNumber(std::string& outValue, bool& outDecimal);
    bool MatchStringLiteral(std::string& outValue);
    bool MatchCharLiteral(char& outValue);

    bool EndOf()
    {
        return position >= tokens.size();
    }
    void Mark()
    {
        marks.push(position);
    }
    Token CurrentToken()
    {
        return tokens[position];
    }
    void Backtrack()
    {
        position = marks.top();
        marks.pop();
    }

    void PushScope(NameAST* name = NULL)
    {
        scopes.push(name);
    }

    void PopScope()
    {
        scopes.pop();
    }

    NameAST* TopScope()
    {
        return scopes.top();
    }



private:
    std::stack<NameAST*> scopes;
    std::stack<int> marks;

    std::vector<Token> tokens;
    int position;
};

class Tokenizer
{
public:
    Tokenizer()
    {
    }
    TokenStream Tokenize(SourceStream& source);
};
