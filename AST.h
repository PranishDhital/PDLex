#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "lexer.h"

enum class NODETYPE
{
    CALL_EXPR,
    NUMBER_LITERAL,
    STRING_LITERAL,
    DOUBLE_LITERAL,
    BOOLEAN_LITERAL,
    IDENT,
    PRINT_STATEMENT,
    VARIABLE_DECLARATION
};

struct NODE
{
    NODETYPE nodetype;
    std::string value;
    std::vector<NODE> child;
};

class AST
{
public:
    NODE parsecall(std::vector<Token> &tokens, int &i);
    NODE parseprint(std::vector<Token> &tokens, int &i);
    NODE parseVar(std::vector<Token> &tokens, int &i);
};
