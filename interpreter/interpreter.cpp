#include "interpreter.h"

#include <type_traits>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "../error/error.h"

using Value = std::variant<std::string, int, double, bool>;

template <typename Map, typename Key>
static bool has_key(const Map& map, const Key& key)
{
	return map.find(key) != map.end();
}

static int to_int(const Value& value)
{
	return std::visit(
		[](const auto& v) -> int
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
				catch (const std::invalid_argument&)
				{
					throw std::runtime_error("cannot convert string \"" + v + "\" to integer");
				}
				catch (const std::out_of_range&)
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

static double to_double(const Value& value)
{
	return std::visit(
		[](const auto& v) -> double
		{
			using T = std::decay_t<decltype(v)>;
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
				catch (const std::invalid_argument&)
				{
					throw std::runtime_error("cannot convert string \"" + v + "\" to double");
				}
				catch (const std::out_of_range&)
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

static std::string to_string(const Value& value)
{
	return std::visit(
		[](const auto& v) -> std::string
		{
			using T = std::decay_t<decltype(v)>;
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

static bool is_double(const Value& v)
{
	return std::holds_alternative<double>(v);
}

static bool is_int(const Value& v)
{
	return std::holds_alternative<int>(v);
}

static bool is_string(const Value& v)
{
	return std::holds_alternative<std::string>(v);
}

static bool to_bool(const Value& value)
{
	return std::visit(
		[](const auto& v) -> bool
		{
			using T = std::decay_t<decltype(v)>;
			if constexpr (std::is_same_v<T, bool>)
			{
				return v;
			}
			else if constexpr (std::is_same_v<T, int>)
			{
				return v != 0;
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				return v != 0.0;
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				return !v.empty();
			}
			else
			{
				return false;
			}
		},
		value);
}

// Helper function to evaluate any node to a Value
Value interpreter::evaluateNode(const NODE& node)
{
	switch (node.nodetype)
	{
	case NODETYPE::IDENT:
		if (!has_key(variables, node.value))
		{
			throw RuntimeError(
				"undefined variable '" + node.value + "'",
				node.line
			);
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

	case NODETYPE::POSTFIX_INCREMENT:
		return evalexpr(node);


	default:
		return node.value;
	}
}


void interpreter::interpret(const NODE& node)
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
			if (const NODE& rhs = node.child[1]; rhs.nodetype == NODETYPE::NUMBER_LITERAL)
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
				if (!has_key(variables, rhs.value))
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
				if (!rhs.value.empty() && has_key(variables, rhs.value))
				{
					value = variables[rhs.value];
				}
			}

			variables[node.value] = value;
			return;
		}
	}
	else if (node.nodetype == NODETYPE::IF_STATEMENT) 
	{
		Value condResult = evalexpr(node.child[0]);
		bool cond = to_bool(condResult);

		if (cond)
		{
			for (auto& statement : node.child[1].child)
			{
				interpret(statement);
			}
		}	/// this i s just the if condition
		else if (node.child.size() > 2)
		{ 
			if (node.child[2].nodetype == NODETYPE::IF_STATEMENT)
			{
				interpret(node.child[2]);		
			}					/// the else if block 
			else
			{
				for (auto& statement : node.child[2].child)
				{
					interpret(statement);
				}
			} /// the else block 
		}
		return;
	}

	else if (node.nodetype == NODETYPE::FOR_LOOP)
	{
		if (node.child.size() < 4)
		{
			throw RuntimeError("malformed for loop", node.line);
		}

		interpret(node.child[0]);

		while (to_bool(evalexpr(node.child[1])))
		{
			for (auto& statement : node.child[3].child)
			{
				interpret(statement);
			}

			evalexpr(node.child[2]);
		}

		return;
	}

	else if (node.nodetype == NODETYPE::PRINT_STATEMENT)
	{
		for (size_t i = 0; i < node.child.size(); i++)
		{
			const NODE& arg = node.child[i];
			Value result = evaluateNode(arg);

			std::visit([](const auto& v)
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
			std::visit([](const auto& v)
				{ std::cout << v; },
				prompt);
		}

		std::cout.flush();

		// Read input from stdcin
		std::string input;
		std::getline(std::cin, input);

		// If node.value contains target variable name, assign to it
		if (!node.value.empty())
		{
			// Convert the input based on the declared type of the variable
			std::string varType = "string";  // default
			if (has_key(variable_types, node.value))
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
					throw RuntimeError("cannot convert '" + input + "' to bool", node.line);
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

Value interpreter::evalexpr(const NODE& node)
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
		catch (const std::exception& e)
		{
			throw RuntimeError(std::string("numeric literal error: ") + e.what(), node.line);
		}
	}

	case NODETYPE::DOUBLE_LITERAL:
	{
		try
		{
			return std::stod(node.value);
		}
		catch (const std::exception& e)
		{
			throw RuntimeError(std::string("double literal error: ") + e.what(), node.line);
		}
	}

	case NODETYPE::STRING_LITERAL:
		return node.value;

	case NODETYPE::BOOLEAN_LITERAL:
		return (node.value == "true");

	case NODETYPE::IDENT:
	{
		if (!has_key(variables, node.value))
		{
			throw RuntimeError("undefined variable '" + node.value + "'", node.line);
		}
		return variables[node.value];
	}

	case NODETYPE::POSTFIX_INCREMENT:
	{
		if (node.value.empty())
		{
			throw RuntimeError("increment requires a variable name", node.line);
		}

		if (!has_key(variables, node.value))
		{
			throw RuntimeError("undefined variable '" + node.value + "'", node.line);
		}

		Value current = variables[node.value];
		if (std::holds_alternative<int>(current))
		{
			variables[node.value] = std::get<int>(current) + 1;
		}
		else if (std::holds_alternative<double>(current))
		{
			variables[node.value] = std::get<double>(current) + 1.0;
		}
		else
		{
			throw RuntimeError("increment only works on int or double variables", node.line);
		}

		return current;
	}

	case NODETYPE::BINARY_OP:
	{
		const Value left = evalexpr(node.child[0]);
		const Value right = evalexpr(node.child[1]);

		// FIX: string + anything => string concatenation
		if (node.value == "+" && (is_string(left) || is_string(right)))
		{
			return to_string(left) + to_string(right);
		}

		// Comparison operators return int (0 for false, 1 for true)
		// gonna wite the if conditons in the same line cause it better to look at
		if (node.value == "==" || node.value == "!=" || node.value == "<" ||
			node.value == ">" || node.value == "<=" || node.value == ">=")
		{
			// For comparisons, try to compare as numbers first if both are numeric types
			if ((is_int(left) || is_double(left)) && (is_int(right) || is_double(right)))
			{
				const double l = to_double(left);
				const double r = to_double(right);

				if (node.value == "==") return (l == r) ? 1 : 0;
				if (node.value == "!=") return (l != r) ? 1 : 0;
				if (node.value == "<")  return (l < r) ? 1 : 0;
				if (node.value == ">")  return (l > r) ? 1 : 0;
				if (node.value == "<=") return (l <= r) ? 1 : 0;
				if (node.value == ">=") return (l >= r) ? 1 : 0;
			}
			else
			{
				// For string comparisons
				const std::string l = to_string(left);
				const std::string r = to_string(right);

				if (node.value == "==") return (l == r) ? 1 : 0;
				if (node.value == "!=") return (l != r) ? 1 : 0;
				if (node.value == "<")  return (l < r) ? 1 : 0;
				if (node.value == ">")  return (l > r) ? 1 : 0;
				if (node.value == "<=") return (l <= r) ? 1 : 0;
				if (node.value == ">=") return (l >= r) ? 1 : 0;
			}
		}

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
			catch (const std::exception& e)
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
		catch (const std::exception& e)
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
