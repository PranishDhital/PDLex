#include "interpreter.h"

#include <type_traits>
#include <iostream>
#include <cmath>
#include <stdexcept>

using Value = std::variant<std::string, int, double, bool>;

static int to_int(const Value &value)
{
    return std::visit(
        []<typename T>(const T &v) -> int
        {
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
                catch (const std::invalid_argument &)
                {
                    throw std::runtime_error("cannot convert string \"" + v + "\" to integer");
                }
                catch (const std::out_of_range &)
                {
                    throw std::runtime_error("integer conversion out of range for \"" + v + "\"");
                }
            }
            else
            {
                return 0;
            }
        },
        value);
}

static double to_double(const Value &value)
{
    return std::visit(
        []<typename T>(const T &v) -> double
        {
            if constexpr (std::is_same_v<T, double>)
            {
                return v;
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                return static_cast<double>(v);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return v ? 1.0 : 0.0;
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                try
                {
                    return std::stod(v);
                }
                catch (const std::invalid_argument &)
                {
                    throw std::runtime_error("cannot convert string \"" + v + "\" to double");
                }
                catch (const std::out_of_range &)
                {
                    throw std::runtime_error("double conversion out of range for \"" + v + "\"");
                }
            }
            else
            {
                return 0.0;
            }
        },
        value);
}

static std::string to_string(const Value &value)
{
    return std::visit(
        []<typename T>(const T &v) -> std::string
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                return v;
            }
            else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>)
            {
                return std::to_string(v);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return v ? "true" : "false";
            }
            else
            {
                return "";
            }
        },
        value);
}

static bool is_double(const Value &v)
{
    return std::holds_alternative<double>(v);
}

static bool is_string(const Value &v)
{
    return std::holds_alternative<std::string>(v);
}

// Helper function to evaluate any node to a Value
Value interpreter::evaluateNode(const NODE &node)
{
    switch (node.nodetype)
    {
        case NODETYPE::IDENT:
            if (!variables.contains(node.value))
            {
                errors::runtimeError(
                    "undefined variable '" + node.value + "'",
                    node.line
                );
                return 0;
            }
            return variables[node.value];

        case NODETYPE::NUMBER_LITERAL:
            return (node.value.find('.') != std::string::npos)
                ? Value(std::stod(node.value))
                : Value(std::stoi(node.value));

        case NODETYPE::STRING_LITERAL:
            return node.value;

        case NODETYPE::BINARY_OP:
            return evalexpr(node);

        default:
            return node.value;
    }
}


void interpreter::interpret(const NODE &node)
{
    if (node.nodetype == NODETYPE::VARIABLE_DECLARATION)
    {
        // Get the declared type from child[0]
        std::string declaredType = "int";  // default
        Value value = 0;

        if (!node.child.empty() && node.child[0].value == "string")
        {
            declaredType = "string";
            value = std::string("");  // Initialize strings to empty
        }
        else if (!node.child.empty() && node.child[0].value == "double")
        {
            declaredType = "double";
            value = 0.0;
        }
        else if (!node.child.empty() && node.child[0].value == "bool")
        {
            declaredType = "bool";
            value = false;
        }
        else
        {
            // int or default
            value = 0;
        }

        // Store the declared type so we can use it for input() later
        variable_types[node.value] = declaredType;

        // If there's an initialization expression, evaluate it
        // if it will be at child[1] now (child[0] is the type)
        if (node.child.size() > 1)
        {
            if (const NODE &rhs = node.child[1]; rhs.nodetype == NODETYPE::NUMBER_LITERAL)
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
                // FIX: evalexpr now returns Value, not int
                value = evalexpr(rhs);
            }
            else if (rhs.nodetype == NODETYPE::IDENT)
            {
                if (!variables.contains(rhs.value))
                {
                    errors::runtimeError("undefined variable '" + rhs.value + "'", node.line);
                    return;
                }
                value = variables[rhs.value];
            }
            else if (rhs.nodetype == NODETYPE::STRING_LITERAL)
            {
                value = rhs.value;
            }
            else if (rhs.nodetype == NODETYPE::BOOLEAN_LITERAL)
            {
                value = (rhs.value == "true");
            }
            else if (rhs.nodetype == NODETYPE::INPUT_EXPRESSION)
            {
                // Handle inline input as RHS (e.g. int x = input("Enter x: ");)
                interpret(rhs);
                // After interpret(rhs), the input value will be in variables[rhs.value]
                // if rhs.value is non-empty, otherwise we need to handle it differently
                if (!rhs.value.empty() && variables.contains(rhs.value))
                {
                    value = variables[rhs.value];
                }
            }
        }

        variables[node.value] = value;
        return;
    }

    else if (node.nodetype == NODETYPE::PRINT_STATEMENT)
    {
        for (size_t i = 0; i < node.child.size(); i++)
        {
            const NODE &arg = node.child[i];
            Value result = evaluateNode(arg);

            std::visit([]<typename T>(const T &v)
                       { std::cout << v; },
                       result);

            if (i + 1 < node.child.size())
            {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }

    else if (node.nodetype == NODETYPE::INPUT_EXPRESSION)
    {
        // Display prompt if present
        if (!node.child.empty())
        {
            Value prompt = evaluateNode(node.child[0]);
            std::visit([]<typename T>(const T &v)
                       { std::cout << v; },
                       prompt);
        }

        // This is IMPORTANT: Flush output so prompt appears before waiting for input
        std::cout.flush();

        // Read input from stdin
        std::string input;
        std::getline(std::cin, input);

        // If node.value contains target variable name, assign to it
        if (!node.value.empty())
        {
            // Convert the input based on the declared type of the variable
            std::string varType = "string";  // default
            if (variable_types.contains(node.value))
            {
                varType = variable_types[node.value];
            }

            if (varType == "int")
            {
                try
                {
                    variables[node.value] = std::stoi(input);
                }
                catch (const std::exception&)
                {
                    errors::runtimeError("cannot convert '" + input + "' to int", node.line);
                    return;
                }
            }
            else if (varType == "double")
            {
                try
                {
                    variables[node.value] = std::stod(input);
                }
                catch (const std::exception&)
                {
                    errors::runtimeError("cannot convert '" + input + "' to double", node.line);
                    return;
                }
            }
            else if (varType == "bool")
            {
                if (input == "true" || input == "1" || input == "yes")
                {
                    variables[node.value] = true;
                }
                else if (input == "false" || input == "0" || input == "no")
                {
                    variables[node.value] = false;
                }
                else
                {
                    errors::runtimeError("cannot convert '" + input + "' to bool", node.line);
                    return;
                }
            }
            else  // string
            {
                variables[node.value] = input;
            }
        }
        return;
    }
}

Value interpreter::evalexpr(const NODE &node)
{
    switch (node.nodetype)
    {
    case NODETYPE::NUMBER_LITERAL:
    {
        try
        {
            if (node.value.find('.') != std::string::npos)
                return std::stod(node.value);
            return std::stoi(node.value);
        }
        catch (const std::exception &e)
        {
            errors::runtimeError(std::string("numeric literal error: ") + e.what(), node.line);
            return 0;
        }
    }

    case NODETYPE::DOUBLE_LITERAL:
    {
        try
        {
            return std::stod(node.value);
        }
        catch (const std::exception &e)
        {
            errors::runtimeError(std::string("double literal error: ") + e.what(), node.line);
            return 0.0;
        }
    }

    case NODETYPE::STRING_LITERAL:
        return node.value;

    case NODETYPE::BOOLEAN_LITERAL:
        return (node.value == "true");

    case NODETYPE::IDENT:
    {
        if (!variables.contains(node.value))
        {
            errors::runtimeError("undefined variable '" + node.value + "'", node.line);
            return 0;
        }
        return variables[node.value];
    }

    case NODETYPE::BINARY_OP:
    {
        const Value left  = evalexpr(node.child[0]);
        const Value right = evalexpr(node.child[1]);

        // FIX: string + anything => string concatenation
        if (node.value == "+" && (is_string(left) || is_string(right)))
        {
            return to_string(left) + to_string(right);
        }

        // FIX: if either operand is a double, promote to double arithmetic
        if (is_double(left) || is_double(right))
        {
            try
            {
                const double l = to_double(left);
                const double r = to_double(right);

                if (node.value == "+") return l + r;
                if (node.value == "-") return l - r;
                if (node.value == "*") return l * r;
                if (node.value == "/")
                {
                    if (r == 0.0)
                    {
                        errors::runtimeError("division by zero", node.line);
                        return 0.0;
                    }
                    return l / r;
                }
                if (node.value == "%")
                {
                    // fmod for doubles
                    if (r == 0.0)
                    {
                        errors::runtimeError("modulo by zero", node.line);
                        return 0.0;
                    }
                    return std::fmod(l, r);
                }
                errors::runtimeError("unknown operator '" + node.value + "'", node.line);
                return 0.0;
            }
            catch (const std::exception &e)
            {
                errors::runtimeError(std::string("type conversion error in expression: ") + e.what(), node.line);
                return 0.0;
            }
        }

        // integer arithmetic
        try
        {
            const int l = to_int(left);
            const int r = to_int(right);

            if (node.value == "+") return l + r;
            if (node.value == "-") return l - r;
            if (node.value == "*") return l * r;
            if (node.value == "%")
            {
                if (r == 0)
                {
                    errors::runtimeError("modulo by zero", node.line);
                    return 0;
                }
                return l % r;
            }
            if (node.value == "/")
            {
                if (r == 0)
                {
                    errors::runtimeError("division by zero", node.line);
                    return 0;
                }
                return l / r;
            }
            errors::runtimeError("unknown operator '" + node.value + "'", node.line);
            return 0;
        }
        catch (const std::exception &e)
        {
            errors::runtimeError(std::string("type conversion error in expression: ") + e.what(), node.line);
            return 0;
        }
    }

    default:
        errors::runtimeError("unexpected node in expression", node.line);
        return 0;
    }
}