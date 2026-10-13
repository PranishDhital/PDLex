#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "AST.h"
#include "error/error.h"
#include "interpreter.h"
#include "lexer.h"

// returns true if filename has .pd
bool hasPdExtension(const std::string& filename) {
    if (filename.size() < 3) {
        return false;
    }
    return filename.substr(filename.size() - 3) == ".pd";
}

// returns version
std::string getVersion() { return "ppi v1.4.2 "; }

void processTokens(const std::vector<Token>& tokens, AST& ast, interpreter& interp) {
    int i = 0;
    while (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::END) {
        NODE tree = ast.parseStatement(tokens, i);
        interp.interpret(tree);
    }
}

int main(int argc, char** argv) {
    AST ast;
    interpreter interp;
    lexer lex;

    if (argc >= 2) {
        std::string arg = argv[1];
        if (arg == "--version" || arg == "--v" || arg == "--V" || arg == "--Version") {
            std::cout << getVersion() << "\n";
            return 0;
        }
    }

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>.pd\n";
        return 1;
    }

    std::string filename = argv[1];

    if (!hasPdExtension(filename)) {
        std::cerr << "Error! Only .pd files are supported\n";
        return 1;
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << "\n";
        return 1;
    }

    ast.setFilename(filename);

    Token tok;
    std::vector<Token> tokens;
    int line = 1;

    do {
        tok = lex.getnextToken(file, line);
        tokens.push_back(tok);
    } while (tok.type != TOKENTYPE::END);

    try {
        processTokens(tokens, ast, interp);
    } catch (const SyntaxError& e) {
        std::cerr << e.getFormattedMessage() << "\n";
        return 1;
    } catch (const RuntimeError& e) {
        std::cerr << e.getFormattedMessage() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
