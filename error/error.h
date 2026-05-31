#pragma once
#include <string>


class errors {
public:
	static void runtimeError(const std::string& msg, int line);
	static void syntaxError(const std::string& msg, int line);
	static void runtimeError(const std::string& msg, int line, const std::string& filename);
	static void syntaxError(const std::string& msg, int line, const std::string& filename);

private:
	static std::string getLineFromFile(const std::string& filename, int lineNum);
	static void displayErrorWithLine(const std::string& msg, int line, const std::string& filename);
};