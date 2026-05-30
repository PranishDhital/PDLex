#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "lexer.h"
#include "../error/error.h"

enum class NODETYPE
{
	CALL_EXPR,
	NUMBER_LITERAL,
	STRING_LITERAL,
	DOUBLE_LITERAL,
	BOOLEAN_LITERAL,
	IDENT,
	BINARY_OP,
	PRINT_STATEMENT,
	VARIABLE_DECLARATION
};

struct NODE
{
	NODETYPE nodetype;
	std::string value;
	std::vector<NODE> child;
	int line;
};

class AST
{
public:
	NODE parsecall(std::vector<Token>& tokens, int& i);
	NODE parseprint(std::vector<Token>& tokens, int& i);
	NODE parseVar(std::vector<Token>& tokens, int& i);
	NODE parseExpr(std::vector<Token>& tokens, int& i);
	NODE parsePrimary(std::vector<Token>& tokens, int& i);
	NODE parseReassign(std::vector<Token>& tokens, int& i);

private:
	errors errorHandler;
};
