#pragma once


#include <istream>
#include <string>


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
    EQEQ,
    RCURLEY,
    LCURLEY,
    GREATER_THAN,
    GREATER_OR_EQ,
    SMALLER_THAN,
    SMALLER_OR_EQ,

    INT,
    DOUBLE,
    PRINT,
    STRING_LITERAL,
    STRING,
    BOOLEAN,
    BOOL_LITERAL,
    INPUT,
    IF,
    ElSE,
    FOR,

    END,
    UNDEFINED
};

struct Token
{
    TOKENTYPE type = TOKENTYPE::UNDEFINED;
    std::string value = "";
    int line = 1;
};

class lexer
{
public:
    Token getnextToken(std::istream &file, int& line);
    void print(const Token &tok);

private:
};

