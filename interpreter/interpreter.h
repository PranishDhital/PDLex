#pragma once

#include <string>
#include <map>
#include <variant>

#include "AST.h"
#include "../error/error.h"

using Value = std::variant<std::string, int, double, bool>;

class interpreter
{
public:
    void interpret(const NODE &node);
    void clearError() { error_occurred = false; }
    [[nodiscard]] bool hasError() const { return error_occurred; }


private:
    std::map<std::string, std::variant<std::string, int, double, bool>> variables;
    std::map<std::string, std::string> variable_types;  // Track declared types (int, double, string, bool)
    errors errorHandler;
    bool error_occurred = false;

    Value evalexpr(const NODE &node);
    Value evaluateNode(const NODE &node);
};
