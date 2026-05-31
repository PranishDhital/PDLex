#pragma once

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
	VARIABLE_DECLARATION,
	INPUT_EXPRESSION
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
	static NODE parsecall(const std::vector<Token>& tokens, int& i);
	static NODE parseExpr(const std::vector<Token>& tokens, int& i);
	static NODE parsePrimary( const std::vector<Token> & tokens, int& i);
	static NODE parseprint(const std::vector<Token>& tokens, int& i);
	static NODE parseVar(const std::vector<Token>& tokens, int& i);
	static NODE parseReassign(const std::vector<Token>& tokens, int& i);
	static NODE parseInput(const std::vector<Token>& tokens, int& i);
	static void setFilename(const std::string& fname) { filename = fname; }

private:
	errors errorHandler;
	static std::string filename;
};
