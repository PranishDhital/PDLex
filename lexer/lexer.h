#pragma once

#include <iostream>
#include <istream>
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
    COMMA,
    NOTEQUAL,
    EXCLAMATION,
    PERCENTAGE,

    INT,
    DOUBLE,
    PRINT,
    STRING_LITERAL,
    STRING,
    BOOLEAN,
    BOOL_LITERAL,
    INPUT,

    END,
    UNDEFINED
};

struct Token
{
    TOKENTYPE type;
    std::string value;
    int line;
};

class lexer
{
public:
    Token getnextToken(std::ifstream &file, int& line);
    void print(const Token &tok);

private:
};
