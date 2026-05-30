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
    case TOKENTYPE::BOOL_LITERAL:
        std::cout << "BOOL_LITERAL  : " << tok.value << "\n";
        break;
    case TOKENTYPE::PERCENTAGE:
        std::cout << "PERCENTAGE    : " << tok.value << "\n";
        break;

    default:
        std::cout << "UNKNOWN       : " << tok.value << "\n";
        break;
    };
}

Token lexer::getnextToken(std::ifstream &file, int& line)
{
    char ch;

    while (file.get(ch) && std::isspace(static_cast<unsigned char>(ch)))
    {
        if (ch == '\n')
        {
            line++;
        }
    }

    if (!file)
    {
        return {TOKENTYPE::END, " ", line};
    }

    switch (ch)
    {
    case ',':
        return {TOKENTYPE::COMMA, ",", line};
    case '+':
        return {TOKENTYPE::PLUS, "+", line};
    case '-':
        return {TOKENTYPE::MINUS, "-",line};
    case '*':
        return {TOKENTYPE::MULTIPLY, "*",line};
    case '(':
        return {TOKENTYPE::LPAREN, "(", line};
    case ')':
        return {TOKENTYPE::RPAREN, ")", line};
    case ';':
        return {TOKENTYPE::SEMI, ";", line};
    case '=':
        return {TOKENTYPE::EQUALSTO, "=",line};

    case '%':
        return {TOKENTYPE::PERCENTAGE, "%", line};

    case '!':
        if (file.peek() == '=')
        {
            file.get(ch);
            return {TOKENTYPE::NOTEQUAL, "!=", line};
        }
        return {TOKENTYPE::EXCLAMATION, "!", line};

    case '"':
    {
        std::string str;
        while (file.get(ch))
        {
            if (ch == '"')
            {
                break;
            }
            if (ch == '\\')
            {
                if (!file.get(ch))
                {
                    break;
                }
                switch (ch)
                {
                case 'n':
                    str.push_back('\n');
                    break;
                case 't':
                    str.push_back('\t');
                    break;
                case '"':
                    str.push_back('"');
                    break;
                case '\\':
                    str.push_back('\\');
                    break;
                default:
                    str.push_back(ch);
                    break;
                }
                continue;
            }
            str.push_back(ch);
        }
        return {TOKENTYPE::STRING_LITERAL, str, line};
    }
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
            if (ch == '\n')
            {
                line++;
            }
            return getnextToken(file, line);
        }
        // for the multiline comments
        else if (file.peek() == '*')
        {
            file.get(ch);
            char prev = 0;
            while (file.get(ch))
            {
                if (ch == '\n')
                {
                    line++;
                }
                if (prev == '*' && ch == '/')
                {
                    break;
                }
                prev = ch;
            }
            return getnextToken(file, line);
        }

        return {TOKENTYPE::DIVIDE, "/", line};
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
            return {TOKENTYPE::INT, ident, line};
        }
        else if (ident == "double")
        {
            return {TOKENTYPE::DOUBLE, ident, line};
        }
        else if (ident == "string")
        {
            return {TOKENTYPE::STRING, ident, line};
        }
        else if (ident == "bool")
        {
            return {TOKENTYPE::BOOLEAN, ident, line};
        }
        else if (ident == "true" || ident == "false")
        {
            return {TOKENTYPE::BOOL_LITERAL, ident, line};
        }
        else if (ident == "print")
        {
            return {TOKENTYPE::PRINT, ident, line};
        }
        else if (ident == "input")
        {
            return { TOKENTYPE::INPUT, ident, line };
        }

        return {TOKENTYPE::IDENT, ident, line};
    }

    // for the numbers
    if (std::isdigit(static_cast<unsigned char>(ch)))
    {
        std::string num(1, ch);

        while (file.get(ch) && std::isdigit(static_cast<unsigned char>(ch)))
        {
            num += ch;
        }

        // Handle decimal point
        if (ch == '.' && file.peek() != EOF)
        {
            num += ch;
            while (file.get(ch) && std::isdigit(static_cast<unsigned char>(ch)))
            {
                num += ch;
            }
        }

        if (!file.eof())
        {
            file.unget();
        }

        return {TOKENTYPE::NUMBER, num, line};
    }

    return {TOKENTYPE::UNDEFINED, std::string(1, ch), line};
}
