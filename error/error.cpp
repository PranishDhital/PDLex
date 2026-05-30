#include "error.h"

void errors::runtimeError(const std::string& msg, int line)
{
	std::cerr << msg << " at line " << line << "\n";
	std::exit(1);
}


void errors::syntaxError(const std::string& msg, int line)
{
	std::cerr << msg << " at line " << line << "\n";
	std::exit(1);
}