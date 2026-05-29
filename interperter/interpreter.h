#pragma once

#include <iostream>
#include <string>
#include <map>
#include <variant>

#include "AST.h"

class interpreter
{
public:
    void interpret(const NODE &node);

private:
    std::map<std::string, std::variant<std::string, int, double, bool>> variables;
    int evalexpr(const NODE &node);
};
