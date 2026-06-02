#include <iostream>
#include <fstream>

#include "error.h"

// Helper function to read a specific line from a file
static std::string getLineFromFileHelper(const std::string& filename, int lineNum)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		return "";
	}

	std::string line;
	int currentLine = 1;
	while (std::getline(file, line))
	{
		if (currentLine == lineNum)
		{
			file.close();
			return line;
		}
		currentLine++;
	}
	file.close();
	return "";
}

// SyntaxError Implementation
SyntaxError::SyntaxError(const std::string& msg, int line, const std::string& filename)
	: std::runtime_error(msg), m_line(line), m_filename(filename)
{
}

std::string SyntaxError::getLineFromFile(const std::string& filename, int lineNum)
{
	return getLineFromFileHelper(filename, lineNum);
}

std::string SyntaxError::getFormattedMessage() const
{
	std::string formatted = "Syntax Error: " + std::string(what()) + " at line " + std::to_string(m_line);

	if (!m_filename.empty())
	{
		const std::string errorLine = getLineFromFile(m_filename, m_line);
		if (!errorLine.empty())
		{
			formatted += "\n  " + std::to_string(m_line) + " | " + errorLine + "\n";
			formatted += "    | ";
			for (size_t i = 0; i < errorLine.length(); i++)
			{
				formatted += "^";
			}
		}
	}

	return formatted;
}

// RuntimeError Implementation
RuntimeError::RuntimeError(const std::string& msg, int line, const std::string& filename)
	: std::runtime_error(msg), m_line(line), m_filename(filename)
{
}

std::string RuntimeError::getLineFromFile(const std::string& filename, int lineNum)
{
	return getLineFromFileHelper(filename, lineNum);
}

std::string RuntimeError::getFormattedMessage() const
{
	std::string formatted = "Runtime Error: " + std::string(what()) + " at line " + std::to_string(m_line);

	if (!m_filename.empty())
	{
		const std::string errorLine = getLineFromFile(m_filename, m_line);
		if (!errorLine.empty())
		{
			formatted += "\n  " + std::to_string(m_line) + " | " + errorLine + "\n";
			formatted += "    | ";
			for (size_t i = 0; i < errorLine.length(); i++)
			{
				formatted += "^";
			}
		}
	}

	return formatted;
}

// Deprecated errors class - kept for backward compatibility
std::string errors::getLineFromFile(const std::string& filename, const int lineNum)
{
	return getLineFromFileHelper(filename, lineNum);
}

void errors::displayErrorWithLine(const std::string& msg, const int line, const std::string& filename)
{
	const std::string errorLine = getLineFromFile(filename, line);
	std::cerr << msg << " at line " << line << "\n";

	if (!errorLine.empty())
	{
		std::cerr << "  " << line << " | " << errorLine << "\n";
		std::cerr << "    | ";
		for (size_t i = 0; i < errorLine.length(); i++)
		{
			std::cerr << "^";
		}
		std::cerr << "\n";
	}
	std::exit(1);
}

void errors::runtimeError(const std::string& msg, const int line)
{
	std::cerr << msg << " at line " << line << "\n";
	std::exit(1);
}

void errors::runtimeError(const std::string& msg, const int line, const std::string& filename)
{
	displayErrorWithLine(msg, line, filename);
}

void errors::syntaxError(const std::string& msg, const int line)
{
	std::cerr << msg << " at line " << line << "\n";
	std::exit(1);
}

void errors::syntaxError(const std::string& msg, const int line, const std::string& filename)
{
	displayErrorWithLine(msg, line, filename);
}