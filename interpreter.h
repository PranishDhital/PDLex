#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "parser.h"

class interpreter
{
public:
    void interpret(const NODE &node);

private:
    std::unordered_map<std::string, int> variables;
};
