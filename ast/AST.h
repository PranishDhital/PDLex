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
	POSTFIX_INCREMENT,
	POSTFIX_DECREMENT,
	PRINT_STATEMENT,
	VARIABLE_DECLARATION,
	ASSIGNMENT,
	INPUT_EXPRESSION,
	IF_STATEMENT,
	BLOCK,
	FOR_LOOP
};

struct NODE
{
	NODETYPE nodetype = NODETYPE::NUMBER_LITERAL;
	std::string value = "";
	std::vector<NODE> child;
	int line = 1;
};

class AST
{
public:
	NODE parsecall(const std::vector<Token>& tokens, int& i);
	NODE parseExpr(const std::vector<Token>& tokens, int& i);
	NODE parseEquality(const std::vector<Token>& tokens, int& i);
	NODE parseComparison(const std::vector<Token>& tokens, int& i);
	NODE parseTerm(const std::vector<Token>& tokens, int& i);
	NODE parseUnary(const std::vector<Token>& tokens, int& i);
	NODE parseFactor(const std::vector<Token>& tokens, int& i);
	NODE parsePrimary(const std::vector<Token>& tokens, int& i);
	NODE parseIncrement(const std::vector<Token>& tokens, int& i);
	NODE parseDecrement(const std::vector<Token>& tokens, int& i);
	NODE parseprint(const std::vector<Token>& tokens, int& i);
	NODE parseVar(const std::vector<Token>& tokens, int& i);
	NODE ns_parseVar(const std::vector<Token>& tokens, int& i);		// No semicolen
	NODE parseReassign(const std::vector<Token>& tokens, int& i);
	NODE parseInput(const std::vector<Token>& tokens, int& i);
	NODE parseIfStatement(const std::vector<Token>& tokens, int& i);
	NODE parseForLoop(const std::vector<Token>& tokens, int& i);
	NODE parseForUpdate(const std::vector<Token>& tokens, int& i);
	NODE parseStatement(const std::vector<Token>& tokens, int& i);
	void setFilename(const std::string& fname) { filename = fname; }

private:
	errors errorHandler;
	std::string filename;
};
