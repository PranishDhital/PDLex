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
	FOR_LOOP,
	WHILE_LOOP,
	PRINT_NL_STATEMENT
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
	/**
	 * Parses a function call expression.
	 */
	NODE parsecall(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a full expression.
	 */
	NODE parseExpr(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses equality operators.
	 */
	NODE parseEquality(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses comparison operators.
	 */
	NODE parseComparison(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses addition and subtraction.
	 */
	NODE parseTerm(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses unary operators.
	 */
	NODE parseUnary(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses multiplication, division, and modulus.
	 */
	NODE parseFactor(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a primary expression.
	 */
	NODE parsePrimary(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a postfix increment expression.
	 */
	NODE parseIncrement(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a postfix decrement expression.
	 */
	NODE parseDecrement(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a print statement.
	 */
	NODE parseprint(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a variable declaration.
	 */
	NODE parseVar(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a variable declaration without a semicolon.
	 */
	NODE ns_parseVar(const std::vector<Token> &tokens, int &i); // No semicolon
	/**
	 * Parses a reassignment statement.
	 */
	NODE parseReassign(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses an input expression.
	 */
	NODE parseInput(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses an if statement.
	 */
	NODE parseIfStatement(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a for loop.
	 */
	NODE parseForLoop(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses the update part of a for loop.
	 */
	NODE parseForUpdate(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a single statement.
	 */
	NODE parseStatement(const std::vector<Token> &tokens, int &i);
	/**
	 * Parses a while loop.
	 */
	NODE parseWhileLoop(const std::vector<Token> &tokens, int &i);

	/**
	 * Parses the print statement that does not append a newline.
	 */
	NODE parseprintNL(const std::vector<Token> &tokens, int &i);
	/**
	 * Sets the current filename for error reporting.
	 */
	void setFilename(const std::string &fname) { filename = fname; }

private:
	errors errorHandler;
	std::string filename;
};
