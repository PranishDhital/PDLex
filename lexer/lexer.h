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
    TOKENTYPE type = TOKENTYPE::UNDEFINED;
    std::string value = "";
    int line = 1;
};

class lexer
{
public:
    static Token getnextToken(std::istream &file, int& line) ;
    static void print(const Token &tok);

private:
};

