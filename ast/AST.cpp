
#include "AST.h"
#include "../error/error.h"

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
		throw SyntaxError("Expected '('", node.line, filename);
	}
	i++;

	while (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::RPAREN)
	{
		if (tokens[i].type == TOKENTYPE::END)
		{
			throw SyntaxError("Expected ')'", tokens[i].line, filename);
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
		throw SyntaxError("Expected ')'", tokens[i].line, filename);
	}
	i++;

	if (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::SEMI)
	{
		throw SyntaxError("Expected ';' at the end", tokens[i].line, filename);
	}
	i++;

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
		throw SyntaxError("Need to declare a variable", node.line, filename);
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
		throw SyntaxError("Expected ';' at the end", i < static_cast<int>(tokens.size()) ? tokens[i].line : node.line, filename);
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
				throw SyntaxError("Expected ')' after input", node.line, filename);
			}
		}
		else
		{
			throw SyntaxError("Expected '(' after 'input'", node.line, filename);
		}

		// skip ';' if present
		if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::SEMI)
		{
			i++;
		}

		return node;
	}
	throw SyntaxError("Unexpected token in primary expression", tokens[i].line, filename);
}

NODE AST::parseExpr(const std::vector<Token>& tokens, int& i)
{
	return parseEquality(tokens, i);
}

NODE AST::parseEquality(const std::vector<Token>& tokens, int& i)
{
	NODE left = parseComparison(tokens, i);

	while (i < static_cast<int>(tokens.size()) &&
		(tokens[i].type == TOKENTYPE::EQEQ || tokens[i].type == TOKENTYPE::NOTEQUAL))
	{
		const std::string op = tokens[i].value;
		const int opLine = tokens[i].line;
		i++;

		NODE right = parseComparison(tokens, i);

		NODE expr;
		expr.nodetype = NODETYPE::BINARY_OP;
		expr.value = op;
		expr.line = opLine;
		expr.child.push_back(left);
		expr.child.push_back(right);

		left = expr;
	}

	return left;
}

NODE AST::parseComparison(const std::vector<Token>& tokens, int& i)
{
	NODE left = parseTerm(tokens, i);

	// Note: Currently the lexer doesn't have token types for <, >, <=, >=
	// This function is prepared for when those are added to the lexer
	while (i < static_cast<int>(tokens.size()) &&
		(tokens[i].type == TOKENTYPE::PLUS ||  // Placeholder until comparison tokens are added
		 tokens[i].type == TOKENTYPE::MINUS))   // These will be replaced with actual comparison operators
	{
		// This condition will never be true in current implementation
		// It's here as a placeholder for future comparison operator support
		const std::string op = tokens[i].value;
		const int opLine = tokens[i].line;
		i++;

		NODE right = parseTerm(tokens, i);

		NODE expr;
		expr.nodetype = NODETYPE::BINARY_OP;
		expr.value = op;
		expr.line = opLine;
		expr.child.push_back(left);
		expr.child.push_back(right);

		left = expr;
	}

	return left;
}

NODE AST::parseTerm(const std::vector<Token>& tokens, int& i)
{
	NODE left = parseFactor(tokens, i);

	while (i < static_cast<int>(tokens.size()) &&
		(tokens[i].type == TOKENTYPE::PLUS || tokens[i].type == TOKENTYPE::MINUS))
	{
		const std::string op = tokens[i].value;
		const int opLine = tokens[i].line;
		i++;

		NODE right = parseFactor(tokens, i);

		NODE expr;
		expr.nodetype = NODETYPE::BINARY_OP;
		expr.value = op;
		expr.line = opLine;
		expr.child.push_back(left);
		expr.child.push_back(right);

		left = expr;
	}

	return left;
}

NODE AST::parseFactor(const std::vector<Token>& tokens, int& i)
{
	NODE left = parsePrimary(tokens, i);

	while (i < static_cast<int>(tokens.size()) &&
		(tokens[i].type == TOKENTYPE::MULTIPLY || tokens[i].type == TOKENTYPE::DIVIDE || tokens[i].type == TOKENTYPE::PERCENTAGE))
	{
		const std::string op = tokens[i].value;
		const int opLine = tokens[i].line;
		i++;

		NODE right = parsePrimary(tokens, i);

		NODE expr;
		expr.nodetype = NODETYPE::BINARY_OP;
		expr.value = op;
		expr.line = opLine;
		expr.child.push_back(left);
		expr.child.push_back(right);

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
		throw SyntaxError("Expected '=' after variable name", node.line, filename);
	}
	i++; // skip '='

	const NODE expr = parseExpr(tokens, i);
	node.child.push_back(expr);

	// expect a ';'
	if (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::SEMI)
	{
		throw SyntaxError("Expected ';'", tokens[i].line, filename);
	}
	else if (i < static_cast<int>(tokens.size()))
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
		throw SyntaxError("Expected '(' after input", i < static_cast<int>(tokens.size()) ? tokens[i].line : node.line, filename);
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
		throw SyntaxError("Expected ')' after input", i < static_cast<int>(tokens.size()) ? tokens[i].line : node.line, filename);
	}

	i++; // skip ')'

	if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::SEMI)
	{
		throw SyntaxError("Expected ';' at the end", i < static_cast<int>(tokens.size()) ? tokens[i].line : node.line, filename);
	}

	i++; // skip ';'

	return node;
}


//NODE AST::parseIfStatement(const std::vector<Token>& tokens, int& i)
//{
//	NODE node;
//	node.nodetype = NODETYPE::IF_STATEMENT;
//	node.line = tokens[i].line;
//
//	i++; // skip 'if'
//
//
//	if (i >= tokens.size() || tokens[i].type != TOKENTYPE::LPAREN)
//		throw SyntaxError("Expected '(' after if", tokens[i].line, filename);
//
//	i++; 
//
//	
//	NODE condition = parseExpr(tokens, i);
//
//	// expect ')'
//	if (i >= tokens.size() || tokens[i].type != TOKENTYPE::RPAREN)
//		throw SyntaxError("Expected ')' after if condition", tokens[i].line, filename);
//
//	i++; // skip ')'
//
//
//	node.child.push_back(condition);
//
//	return node;
//}