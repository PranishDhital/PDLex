#include "lexer.h"
#include "interpreter.h"
#include "AST.h"
#include "error/error.h"

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

void processTokens(const std::vector<Token> &tokens, AST &ast, interpreter &interp)
{
    int i = 0;
    while (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::END)
    {
        NODE tree = ast.parseStatement(tokens, i);
        interp.interpret(tree);
    }
}

int main(int argc, char **argv)
{
    AST ast;
    interpreter Interpert;
    lexer lex;

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

    ast.setFilename(filename);

    Token tok;
    std::vector<Token> tokens;
    int line = 1;

    do
    {
        tok = lex.getnextToken(file, line);
        tokens.push_back(tok);
    } while (tok.type != TOKENTYPE::END);

    try
    {
        processTokens(tokens, ast, Interpert);
    }
    catch (const SyntaxError &e)
    {
        std::cerr << e.getFormattedMessage() << "\n";
        return 1;
    }
    catch (const RuntimeError &e)
    {
        std::cerr << e.getFormattedMessage() << "\n";
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
