#include "AST.h"

NODE AST::parsecall(std::vector<Token>& tokens, int& i)
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

	// Only treat  the IDENT as a call if it's followed by the symblo '('.
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

NODE AST::parseprint(std::vector<Token>& tokens, int& i)
{
	NODE node;
	node.nodetype = NODETYPE::PRINT_STATEMENT;

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
		errorHandler.syntaxError("Expected '('" , node.line);
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
			errorHandler.syntaxError("Expected ')'" , tokens[i].line);
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
		else if (tokens[i].type == TOKENTYPE::COMMA)
		{
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
		else if (tokens[i].type == TOKENTYPE::EXCLAMATION)
		{
			i++;
			continue;
		}
		i++;
	}

	if (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::RPAREN)
	{
		errorHandler.syntaxError("Expected ')'", tokens[i].line);
	}
	else
	{
		i++;
	}

	if (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::SEMI)
	{
		errorHandler.syntaxError("Expected ';' at the end" , tokens[i].line);
	}
	else
	{
		i++;
	}

	return node;
}

// parse variables
NODE AST::parseVar(std::vector<Token>& tokens, int& i)
{
	NODE node;
	node.nodetype = NODETYPE::VARIABLE_DECLARATION;

	if (i >= static_cast<int>(tokens.size()))
	{
		return node;
	}

	if (tokens[i].type != TOKENTYPE::INT && tokens[i].type != TOKENTYPE::DOUBLE && tokens[i].type != TOKENTYPE::STRING && tokens[i].type != TOKENTYPE::BOOLEAN)
	{
		return node;
	}

	std::string declaredType = tokens[i].value;
	i++;

	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::IDENT)
	{
		errorHandler.syntaxError("Need to declare a variable", node.line);
		return node;
	}
	node.value = tokens[i].value;
	node.line = tokens[i].line;
	i++;

	if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::EQUALSTO)
	{
		i++;

		NODE expr = parseExpr(tokens, i);
		node.child.push_back(expr);
	}
	if (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::SEMI)
	{
		errorHandler.syntaxError("Expected ';' at the end", tokens[i].line);
	}
	else
	{
		i++; // skip ';'
	}

	return node;
}

NODE AST::parsePrimary(std::vector<Token>& tokens, int& i)
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

	return node;
}

NODE AST::parseExpr(std::vector<Token>& tokens, int& i)
{
	NODE left = parsePrimary(tokens, i); // the left side

	while (i < static_cast<int>(tokens.size()) &&
		(tokens[i].type == TOKENTYPE::PLUS || tokens[i].type == TOKENTYPE::MINUS || tokens[i].type == TOKENTYPE::MULTIPLY || tokens[i].type == TOKENTYPE::DIVIDE || tokens[i].type == TOKENTYPE::PERCENTAGE))
	{
		std::string op = tokens[i].value;
		int opLine = tokens[i].line;
		i++;

		NODE right = parsePrimary(tokens, i); // get the right side

		NODE expr;
		expr.nodetype = NODETYPE::BINARY_OP;
		expr.value = op;
		expr.line = opLine;
		expr.child.push_back(left);  // the left side
		expr.child.push_back(right); // the right side in the tree

		// for the cgain
		left = expr;
	}
	return left;
}

NODE AST::parseReassign(std::vector<Token>& tokens, int& i)
{
	NODE node;
	node.nodetype = NODETYPE::VARIABLE_DECLARATION; // reuse the  same node type

	if (i >= static_cast<int>(tokens.size()))
		return node;

	// grab variables name
	node.value = tokens[i].value;
	node.line = tokens[i].line;
	i++; // skip the IDENT

	// skip '='
	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::EQUALSTO)
	{
		errorHandler.syntaxError("Expected '=' after variable name", node.line);
		return node;
	}
	i++; // skip '='

	NODE expr = parseExpr(tokens, i);
	node.child.push_back(expr);

	// expect a ';'
	if (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::SEMI)
	{
		errorHandler.syntaxError("Expected ';'", tokens[i].line);
	}
	else
	{
		i++; // skip ';'
	}

	return node;
}
