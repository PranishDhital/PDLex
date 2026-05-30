#pragma once
#include <iostream>
#include <string>



class errors {
public:
	void runtimeError(const std::string& msg, int line);
	void syntaxError(const std::string& msg, int line);

private:

};