#include <iostream>
#include <fstream>

#include "error.h"

std::string errors::getLineFromFile(const std::string& filename, const int lineNum)
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