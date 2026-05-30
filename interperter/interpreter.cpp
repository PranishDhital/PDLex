#include "interpreter.h"

#include <type_traits>

using Value = std::variant<std::string, int, double, bool>;

// This code below is written by AI
static int to_int(const Value &value)
{
    return std::visit(
        [](const auto &v) -> int
        {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, int>)
            {
                return v;
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return static_cast<int>(v);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return v ? 1 : 0;
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                try
                {
                    return std::stoi(v);
                }
                catch (...)
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        },
        value);
}

void interpreter::interpret(const NODE &node)
{
    if (node.nodetype == NODETYPE::VARIABLE_DECLARATION)
    {
        Value value = 0;
        if (!node.child.empty())
        {
            const NODE &rhs = node.child[0];
            if (rhs.nodetype == NODETYPE::NUMBER_LITERAL)
            {
                if (rhs.value.find('.') != std::string::npos)
                {
                    value = std::stod(rhs.value);
                }
                else
                {
                    value = std::stoi(rhs.value);
                }
            }
            else if (rhs.nodetype == NODETYPE::DOUBLE_LITERAL)
            {
                value = std::stod(rhs.value);
            }
            else if (rhs.nodetype == NODETYPE::BINARY_OP)
            {
                value = evalexpr(rhs);
            }
            else if (rhs.nodetype == NODETYPE::IDENT)
            {
                if (variables.find(rhs.value) == variables.end())
                {
                    errorHandler.runtimeError("undefined variable '" + rhs.value + "'", node.line);
                    return;
                }
                value = variables[rhs.value];
            }
            else if (rhs.nodetype == NODETYPE::STRING_LITERAL)
            {
                // std::cerr << "Runtime Error: cannot assign string to int " << node.value << "\n";
                value = rhs.value;
            }
            else if (rhs.nodetype == NODETYPE::BOOLEAN_LITERAL)
            {
                value = (rhs.value == "true");
            }
        }

        variables[node.value] = value;
        return;
    }

    if (node.nodetype == NODETYPE::PRINT_STATEMENT)
    {
        for (size_t i = 0; i < node.child.size(); i++)
        {
            const NODE &arg = node.child[i];
            if (arg.nodetype == NODETYPE::IDENT)
            {
                if (variables.find(arg.value) == variables.end())
                {
                    errorHandler.runtimeError("undefined variable '" + arg.value + "'", node.line);
                    return;
                }
                std::visit([](auto v)
                           { std::cout << v; }, variables[arg.value]);
            }
            else if (arg.nodetype == NODETYPE::STRING_LITERAL)
            {
                std::cout << arg.value;
            }
            else if (arg.nodetype == NODETYPE::NUMBER_LITERAL)
            {
                std::cout << arg.value;
            }
            else if (arg.nodetype == NODETYPE::BOOLEAN_LITERAL)
            {
                std::cout << arg.value;
            }
            else
            {
                std::cout << arg.value;
            }
            if (i + 1 < node.child.size())
            {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

int interpreter::evalexpr(const NODE &node)
{
    switch (node.nodetype)
    {
    case NODETYPE::NUMBER_LITERAL:
        return std::stoi(node.value);

    case NODETYPE::IDENT:
    {
        if (variables.find(node.value) == variables.end())
        {
            errorHandler.runtimeError("undefined variable '" + node.value + "'", node.line);
            return 0; // safely fallback, don't proceed to stoi
        }
        return to_int(variables[node.value]);
    }

    case NODETYPE::BINARY_OP:
    {
        int left = evalexpr(node.child[0]);
        int right = evalexpr(node.child[1]);

        if (node.value == "+")
        {
            return left + right;
        }
        else if (node.value == "-")
        {
            return left - right;
        }
        else if (node.value == "*")
        {
            return left * right;
        }
        else if (node.value == "%")
        {
            return left % right;
        }
        else if (node.value == "/")
        {
            if (right == 0)
            {
                errorHandler.runtimeError("division by zero", node.line);
                return 0;
            }
            return left / right;
        }
        errorHandler.runtimeError("unknown operator '" + node.value + "'", node.line);
        return 0;
    }

    default:
        errorHandler.runtimeError("unexpected node in expression", node.line);
        return 0;
    }
}
