#pragma once
#include <string>
#include <stdexcept>

// Exception class for syntax errors during parsing
class SyntaxError : public std::runtime_error
{
public:
	/// <summary>
	/// prints syntax error message
	/// </summary>
	/// <param name="msg"></param>
	/// <param name="line"></param>
	/// <param name="filename"></param>
	SyntaxError(const std::string& msg, int line, const std::string& filename = "");
	int getLine() const { return m_line; }
	const std::string& getFilename() const { return m_filename; }
	std::string getFormattedMessage() const;

private:
	int m_line;
	std::string m_filename;
	static std::string getLineFromFile(const std::string& filename, int lineNum);
};

// Exception class for runtime errors during interpretation
class RuntimeError : public std::runtime_error
{
public:
	/// <summary>
	/// Prints Runtime error message
	/// </summary>
	/// <param name="msg"></param>
	/// <param name="line"></param>
	/// <param name="filename"></param>
	RuntimeError(const std::string& msg, int line, const std::string& filename = "");
	int getLine() const { return m_line; }
	const std::string& getFilename() const { return m_filename; }
	std::string getFormattedMessage() const;

private:
	int m_line;
	std::string m_filename;
	static std::string getLineFromFile(const std::string& filename, int lineNum);
};

// Deprecated: kept for backward compatibility (replaced by exceptions)
class errors
{
public:
	static void runtimeError(const std::string& msg, int line);
	static void syntaxError(const std::string& msg, int line);
	static void runtimeError(const std::string& msg, int line, const std::string& filename);
	static void syntaxError(const std::string& msg, int line, const std::string& filename);

private:
	static std::string getLineFromFile(const std::string& filename, int lineNum);
	static void displayErrorWithLine(const std::string& msg, int line, const std::string& filename);
};