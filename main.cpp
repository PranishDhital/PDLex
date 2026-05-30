#include "lexer.h"
#include "interpreter.h"
#include "AST.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool pdExtension(const std::string &filename)
{
    if (filename.size() < 3)
        return false;
    return filename.substr(filename.size() - 3) == ".pd";
}

int main(int argc, char **argv)
{
    lexer Lexer;
    AST ast;
    interpreter Interpert;

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>.pd\n";
        return 1;
    }

    std::string filename = argv[1];
    if (!pdExtension(filename))
    {
        std::cerr << "Error! Only .pd files are supported\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << argv[1] << "\n";
        return 1;
    }

    Token tok;
    std::vector<Token> tokens;
    int line = 1;
    do
    {
        tok = Lexer.getnextToken(file, line);
        tokens.push_back(tok);
    } while (tok.type != TOKENTYPE::END);

    int i = 0;
    while (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::END)
    {
        // variable declaration:  int x = 10;
        if (tokens[i].type == TOKENTYPE::INT ||
            tokens[i].type == TOKENTYPE::DOUBLE ||
            tokens[i].type == TOKENTYPE::STRING ||
            tokens[i].type == TOKENTYPE::BOOLEAN)
        {
            NODE tree = ast.parseVar(tokens, i);
            Interpert.interpret(tree);
            continue;
        }

        // print statement:  print(x);
        if (tokens[i].type == TOKENTYPE::PRINT)
        {
            NODE tree = ast.parseprint(tokens, i);
            Interpert.interpret(tree);
            continue;
        }

        // ident — reassignment or function call
        if (tokens[i].type == TOKENTYPE::IDENT)
        {
            // reassignment:  a = 20;
            if (i + 1 < static_cast<int>(tokens.size()) &&
                tokens[i + 1].type == TOKENTYPE::EQUALSTO)
            {
                NODE tree = ast.parseReassign(tokens, i);
                Interpert.interpret(tree);
                continue;
            }

            // function call:  myFunc(x);
            if (i + 1 < static_cast<int>(tokens.size()) &&
                tokens[i + 1].type == TOKENTYPE::LPAREN)
            {
                NODE tree = ast.parsecall(tokens, i);
                Interpert.interpret(tree);
                continue;
            }
        }

        // unknown token
        std::cerr << "Warning: unexpected token '" << tokens[i].value << "\n";
        i++;
    }

    return 0;
}