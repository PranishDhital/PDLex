
#include "AST.h"

std::string AST::filename;

NODE AST::parsecall(const std::vector<Token>& tokens, int& i)
{
	NODE call;
	call.nodetype = NODETYPE::CALL_EXPR;
	if (i >= static_cast<int>(tokens.size()))
	{
		return call;
	}

	// Must start with an identifier (which is function name).
	if (tokens[i].type != TOKENTYPE::IDENT)
	{
		return call;
	}

	// Only treat  the IDENT as a call if it's followed by the symbol '('.
	// Otherwise, don't consume anything so the caller can decide what to do.
	if (i + 1 >= static_cast<int>(tokens.size()) || tokens[i + 1].type != TOKENTYPE::LPAREN)
	{
		return call;
	}

	// calling the name
	call.value = tokens[i].value;
	call.line = tokens[i].line;
	i++;

	// skip LPAREN
	i++;

	// parse the  arguments

	while (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::RPAREN)
	{
		if (tokens[i].type == TOKENTYPE::NUMBER)
		{
			NODE arg;
			arg.nodetype = NODETYPE::NUMBER_LITERAL;
			arg.value = tokens[i].value;
			call.child.push_back(arg);
			i++;
			continue;
		}
		else if (tokens[i].type == TOKENTYPE::BOOL_LITERAL)
		{
			NODE arg;
			arg.nodetype = NODETYPE::BOOLEAN_LITERAL;
			arg.value = tokens[i].value;
			call.child.push_back(arg);
			i++;
			continue;
		}

		else if (tokens[i].type == TOKENTYPE::IDENT)
		{
			NODE arg;
			arg.nodetype = NODETYPE::IDENT;
			arg.value = tokens[i].value;
			call.child.push_back(arg);
			i++;
			continue;
		}
		i++;
	}

	if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::RPAREN)
	{
		i++; // skip RPAREN i.e :  ')'
	}

	if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::SEMI)
	{
		i++; // skip the  SEMI colen
	}

	return call;
}

NODE AST::parseprint( const std::vector<Token>& tokens, int& i)
{
	NODE node;
	node.nodetype = NODETYPE::PRINT_STATEMENT;
	node.line   = tokens[i].line;

	if (i >= static_cast<int>(tokens.size()))
	{
		return node;
	}

	if (tokens[i].type != TOKENTYPE::PRINT)
	{
		return node;
	}

	node.value = tokens[i].value;
	node.line = tokens[i].line;
	i++;

	// skiping lparen
	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::LPAREN)
	{
		errors::syntaxError("Expected '('" , node.line, filename);
		return node;
	}
	else
	{
		i++;
	}

	while (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::RPAREN)
	{
		if (tokens[i].type == TOKENTYPE::END)
		{
			errors::syntaxError("Expected ')'" , tokens[i].line, filename);
			return node;
		}
		else if (tokens[i].type == TOKENTYPE::NUMBER)
		{
			NODE arg;
			arg.nodetype = NODETYPE::NUMBER_LITERAL;
			arg.value = tokens[i].value;
			node.child.push_back(arg);
			i++;
			continue;
		}
		else if (tokens[i].type == TOKENTYPE::IDENT)
		{
			NODE arg;
			arg.nodetype = NODETYPE::IDENT;
			arg.value = tokens[i].value;
			node.child.push_back(arg);
			i++;
			continue;
		}
		else if (tokens[i].type == TOKENTYPE::STRING_LITERAL)
		{
			NODE arg;
			arg.nodetype = NODETYPE::STRING_LITERAL;
			arg.value = tokens[i].value;
			node.child.push_back(arg);
			i++;
			continue;
		}
		else if (tokens[i].type == TOKENTYPE::BOOL_LITERAL)
		{
			NODE arg;
			arg.nodetype = NODETYPE::BOOLEAN_LITERAL;
			arg.value = tokens[i].value;
			node.child.push_back(arg);
			i++;
			continue;
		}
		else if (tokens[i].type == TOKENTYPE::EXCLAMATION || tokens[i].type == TOKENTYPE::COMMA)
		{
			i++;
			continue;
		}
		i++;
	}

	if (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::RPAREN)
	{
		errors::syntaxError("Expected ')'", tokens[i].line, filename);
	}
	else
	{
		i++;
	}

	if (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::SEMI)
	{
		errors::syntaxError("Expected ';' at the end" , tokens[i].line, filename);
	}
	else
	{
		i++;
	}

	return node;
}

// parse variables
NODE AST::parseVar(const std::vector<Token>& tokens, int& i)
{
	NODE node;
	node.nodetype = NODETYPE::VARIABLE_DECLARATION;
	node.line = tokens[i].line;

	if (i >= static_cast<int>(tokens.size()))
	{
		return node;
	}

	if (tokens[i].type != TOKENTYPE::INT && tokens[i].type != TOKENTYPE::DOUBLE && tokens[i].type != TOKENTYPE::STRING && tokens[i].type != TOKENTYPE::BOOLEAN)
	{
		return node;
	}

	const std::string declaredType = tokens[i].value;
	node.line = tokens[i].line;
	i++;

	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::IDENT)
	{
		errors::syntaxError("Need to declare a variable", node.line, filename);
		return node;
	}
	node.value = tokens[i].value;

	// Store the declared type as the first child node for interpreter to use
	NODE typeNode;
	typeNode.nodetype = NODETYPE::STRING_LITERAL; // reuse for type info
	typeNode.value = declaredType;
	node.child.push_back(typeNode);

	i++;

	if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::EQUALSTO)
	{
		i++;
		const NODE expr = parseExpr(tokens, i);
		node.child.push_back(expr);
	}

	// Expect semicolon
	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::SEMI)
	{
		errors::syntaxError("Expected ';' at the end", tokens[i].line, filename);
		return node;
	}

	i++; // skip ';'

	return node;
}

NODE AST::parsePrimary(const std::vector<Token>& tokens, int& i)
{
	NODE node;

	if (tokens[i].type == TOKENTYPE::NUMBER)
	{
		node.nodetype = NODETYPE::NUMBER_LITERAL;
		node.value = tokens[i].value;
		node.line = tokens[i].line;
		i++;
		return node;
	}
	else if (tokens[i].type == TOKENTYPE::STRING_LITERAL)
	{
		node.nodetype = NODETYPE::STRING_LITERAL;
		node.value = tokens[i].value;
		node.line = tokens[i].line;
		i++;
		return node;
	}
	else if (tokens[i].type == TOKENTYPE::BOOL_LITERAL)
	{
		node.nodetype = NODETYPE::BOOLEAN_LITERAL;
		node.value = tokens[i].value;
		node.line = tokens[i].line;
		i++;
		return node;
	}
	else if (tokens[i].type == TOKENTYPE::IDENT)
	{
		node.nodetype = NODETYPE::IDENT;
		node.value = tokens[i].value;
		node.line = tokens[i].line;
		i++;
		return node;
	}
	else if (tokens[i].type == TOKENTYPE::INPUT) {
		node.nodetype = NODETYPE::INPUT_EXPRESSION;
		node.line = tokens[i].line;
		i++; // skip 'input'

		// skip '('
		if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::LPAREN)
		{
			i++; // skip '('

			// capture variable name
			if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::IDENT)
			{
				node.value = tokens[i].value;  // store variable name
				i++; // skip variable name
			}
			// optional prompt string after variable
			else if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::STRING_LITERAL)
			{
				NODE prompt;
				prompt.nodetype = NODETYPE::STRING_LITERAL;
				prompt.value = tokens[i].value;
				prompt.line = tokens[i].line;
				node.child.push_back(prompt);
				i++; // skip the string
			}

			// skip ')'
			if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::RPAREN)
			{
				i++;
			}
			else
			{
				errors::syntaxError("Expected ')' after input", node.line, filename);
			}
		}
		else
		{
			errors::syntaxError("Expected '(' after 'input'", node.line, filename);
		}

		// skip ';' if present
		if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::SEMI)
		{
			i++;
		}

		return node;
	}
	return node;
}

NODE AST::parseExpr(const std::vector<Token>& tokens, int& i)
{
	NODE left = parsePrimary(tokens, i); // the left side

	while (i < static_cast<int>(tokens.size()) &&
		(tokens[i].type == TOKENTYPE::PLUS || tokens[i].type == TOKENTYPE::MINUS || tokens[i].type == TOKENTYPE::MULTIPLY || tokens[i].type == TOKENTYPE::DIVIDE || tokens[i].type == TOKENTYPE::PERCENTAGE))
	{
		const std::string op = tokens[i].value;
		const int opLine = tokens[i].line;
		i++;

		NODE right = parsePrimary(tokens, i); // get the right side

		NODE expr;
		expr.nodetype = NODETYPE::BINARY_OP;
		expr.value = op;
		expr.line = opLine;
		expr.child.push_back(left);  // the left side
		expr.child.push_back(right); // the right side in the tree

		// for the chain
		left = expr;
	}
	return left;
}

NODE AST::parseReassign(const std::vector<Token>& tokens, int& i)
{
	NODE node;
	node.nodetype = NODETYPE::VARIABLE_DECLARATION; // reuse the  same node type
	node.line = tokens[i].line;

	if (i >= static_cast<int>(tokens.size())) {
		return node;
	}

	// grab variables name
	node.value = tokens[i].value;
	node.line = tokens[i].line;
	i++; // skip the IDENT

	// skip '='
	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::EQUALSTO)
	{
		errors::syntaxError("Expected '=' after variable name", node.line, filename);
		return node;
	}
	i++; // skip '='

	const NODE expr = parseExpr(tokens, i);
	node.child.push_back(expr);

	// expect a ';'
	if (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::SEMI)
	{
		errors::syntaxError("Expected ';'", tokens[i].line, filename);
	}
	else
	{
		i++; // skip ';'
	}

	return node;
}


NODE AST::parseInput(const std::vector<Token> &tokens, int &i)
{
	NODE node;
	node.nodetype = NODETYPE::INPUT_EXPRESSION;
	node.line = tokens[i].line;

	i++; // skip 'input'

	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::LPAREN)
	{
		errors::syntaxError("Expected '(' after input", tokens[i].line, filename);
		return node;
	}

	i++; // skip '('

	// Check what's inside: could be IDENT, STRING_LITERAL, or both
	std::string variableName;

	// First element: could be variable name OR prompt string
	if (tokens[i].type == TOKENTYPE::IDENT)
	{
		// input(variable_name);
		variableName = tokens[i].value;
		node.value = variableName;
		i++;
	}
	else if (tokens[i].type == TOKENTYPE::STRING_LITERAL)
	{
		// input("prompt", variable_name); OR input("prompt");
		const std::string prompt = tokens[i].value;;
		NODE promptNode;
		promptNode.nodetype = NODETYPE::STRING_LITERAL;
		promptNode.value = prompt;
		node.child.push_back(promptNode);
		i++;

		// Check for comma and variable name
		if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::COMMA)
		{
			i++; // skip ','

			if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::IDENT)
			{
				variableName = tokens[i].value;
				node.value = variableName;
				i++;
			}
		}
	}

	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::RPAREN)
	{
		errors::syntaxError("Expected ')' after input", tokens[i].line, filename);
		return node;
	}

	i++; // skip ')'

	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::SEMI)
	{
		errors::syntaxError("Expected ';' at the end", tokens[i].line, filename);
		return node;
	}

	i++; // skip ';'

	return node;
}