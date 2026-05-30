#pragma once

#include <iostream>
#include <string>
#include <map>
#include <variant>

#include "AST.h"
#include "../error/error.h"

class interpreter
{
public:
    void interpret(const NODE &node);

private:
    std::map<std::string, std::variant<std::string, int, double, bool>> variables;
    errors errorHandler;
    int evalexpr(const NODE &node);
};
