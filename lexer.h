#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

enum class TOKENTYPE
{
    NUMBER,
    SEMI,
    LPAREN,
    RPAREN,
    IDENT,
    EQUALSTO,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,

    INT,
    PRINT,

    END,
    UNDEFINED
};

struct Token
{
    TOKENTYPE type;
    std::string value;
};

class lexer
{
public:
    Token getnextToken(std::ifstream &file);
    void print(const Token &tok);

private:
};
