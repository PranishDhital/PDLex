#include "lexer.h"

void lexer::print(const Token &tok)
{
    switch (tok.type)
    {
    case TOKENTYPE::NUMBER:
        std::cout << "NUMBER        : " << tok.value << "\n";
        break;

    case TOKENTYPE::MINUS:
        std::cout << "MINUS         : " << tok.value << "\n";
        break;

    case TOKENTYPE::PLUS:
        std::cout << "PLUS          : " << tok.value << "\n";
        break;

    case TOKENTYPE::DIVIDE:
        std::cout << "DIVIDE        : " << tok.value << "\n";
        break;

    case TOKENTYPE::MULTIPLY:
        std::cout << "MULTIPLY      : " << tok.value << "\n";
        break;

    case TOKENTYPE::SEMI:
        std::cout << "SEMI          : " << tok.value << "\n";
        break;

    case TOKENTYPE::IDENT:
        std::cout << "IDENT         : " << tok.value << "\n";
        break;

    case TOKENTYPE::LPAREN:
        std::cout << "LPAREN        : " << tok.value << "\n";
        break;

    case TOKENTYPE::RPAREN:
        std::cout << "RPAREN        : " << tok.value << "\n";
        break;

    case TOKENTYPE::EQUALSTO:
        std::cout << "EQUALSTO      : " << tok.value << "\n";
        break;

    case TOKENTYPE::END:
        std::cout << "END           : " << tok.value << "\n";
        break;

    case TOKENTYPE::UNDEFINED:
        std::cout << "UNDEFINED     : " << tok.value << "\n";
        break;

    case TOKENTYPE::INT:
        std::cout << "INT           : " << tok.value << "\n";
        break;

    case TOKENTYPE::COMMA:
        std::cout << "COMMA         : " << tok.value << " \n";
        break;
    case TOKENTYPE::EXCLAMATION:
        std::cout << "EXCLAMATION   : " << tok.value << "\n";
        break;

    default:
        std::cout << "UNKNOWN       : " << tok.value << "\n";
        break;
    };
}

Token lexer::getnextToken(std::ifstream &file)
{
    char ch;

    while (file.get(ch) && std::isspace(static_cast<unsigned char>(ch)))
        ;

    if (!file)
    {
        return {TOKENTYPE::END, " "};
    }

    switch (ch)
    {
    case ',':
        return {TOKENTYPE::COMMA, ","};
    case '+':
        return {TOKENTYPE::PLUS, "+"};
    case '-':
        return {TOKENTYPE::MINUS, "-"};
    case '*':
        return {TOKENTYPE::MULTIPLY, "*"};
    case '(':
        return {TOKENTYPE::LPAREN, "("};
    case ')':
        return {TOKENTYPE::RPAREN, ")"};
    case ';':
        return {TOKENTYPE::SEMI, ";"};
    case '=':
        return {TOKENTYPE::EQUALSTO, "="};
    case '!':
        if (file.peek() == '=')
        {
            file.get(ch);
            return {TOKENTYPE::NOTEQUAL, "!="};
        }
        return {TOKENTYPE::EXCLAMATION, "!"};
    }

    // for the comments and divide
    if (ch == '/')
    {
        // this is for the single line comments
        if (file.peek() == '/')
        {
            while (file.get(ch) && ch != '\n')
            {
            }
            return getnextToken(file);
        }
        // for the multiline comments
        else if (file.peek() == '*')
        {
            file.get(ch);
            char prev = 0;
            while (file.get(ch))
            {
                if (prev == '*' && ch == '/')
                {
                    break;
                }
                prev = ch;
            }
            return getnextToken(file);
        }

        return {TOKENTYPE::DIVIDE, "/"};
    }

    // this is for characters
    if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '_')
    {
        std::string ident(1, ch);
        while (
            file.get(ch) &&
            (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_'))
        {
            ident += ch;
        }

        if (!file.eof())
        {
            file.unget();
        }

        if (ident == "int")
        {
            return {TOKENTYPE::INT, ident};
        }

        if (ident == "print")
        {
            return {TOKENTYPE::PRINT, ident};
        }

        return {TOKENTYPE::IDENT, ident};
    }

    // for the numbers
    if (std::isdigit(static_cast<unsigned char>(ch)))
    {
        std::string num(1, ch);

        while (file.get(ch) && std::isdigit(static_cast<unsigned char>(ch)))
        {
            num += ch;
        }

        if (!file.eof())
        {
            file.unget();
        }

        return {TOKENTYPE::NUMBER, num};
    }

    return {TOKENTYPE::UNDEFINED, std::string(1, ch)};
}
