#include "parser.h"

NODE parser::parsecall(std::vector<Token> &tokens, int &i)
{
    NODE call;
    call.nodetype = NODETYPE::CALL_EXPR;
    if (i >= static_cast<int>(tokens.size()))
    {
        return call;
    }

    // Must start with an identifier (which is function name).
    if (tokens[i].type != TOKENTYPE::IDENT)
    {
        return call;
    }

    // Only treat  the IDENT as a call if it's followed by the symblo '('.
    // Otherwise, don't consume anything so the caller can decide what to do.
    if (i + 1 >= static_cast<int>(tokens.size()) || tokens[i + 1].type != TOKENTYPE::LPAREN)
    {
        return call;
    }

    // calling the name
    call.value = tokens[i++].value;

    // skip LPAREN
    i++;

    // parse the  arguments

    while (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::RPAREN)
    {
        if (tokens[i].type == TOKENTYPE::NUMBER)
        {
            NODE arg;
            arg.nodetype = NODETYPE::NUMBER_LITERAL;
            arg.value = tokens[i].value;
            call.child.push_back(arg);
            i++;
            continue;
        }

        if (tokens[i].type == TOKENTYPE::IDENT)
        {
            NODE arg;
            arg.nodetype = NODETYPE::IDENT;
            arg.value = tokens[i].value;
            call.child.push_back(arg);
            i++;
            continue;
        }
        i++;
    }

    if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::RPAREN)
    {
        i++; // skip RPAREN i.e :  ')'
    }

    if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::SEMI)
    {
        i++; // skip the  SEMI colen
    }

    return call;
}

NODE parser::parseprint(std::vector<Token> &tokens, int &i)
{
    NODE node;
    node.nodetype = NODETYPE::PRINT_STATEMENT;

    if (i >= static_cast<int>(tokens.size()))
    {
        return node;
    }

    if (tokens[i].type != TOKENTYPE::PRINT)
    {
        return node;
    }

    node.value = tokens[i].value;
    i++;

    // skiping lparen
    if (i >= static_cast<int>(tokens.size()) || tokens[i].type != TOKENTYPE::LPAREN)
    {
        std::cerr << "Error! Expected '(' " << "\n";
        return node;
    }
    else
    {
        i++;
    }

    while (i < static_cast<int>(tokens.size()) && tokens[i].type != TOKENTYPE::RPAREN)
    {
        if (tokens[i].type == TOKENTYPE::END)
        {
            std::cerr << "Error! Expected ')' " << "\n";
            return node;
        }

        if (tokens[i].type == TOKENTYPE::NUMBER)
        {
            NODE arg;
            arg.nodetype = NODETYPE::NUMBER_LITERAL;
            arg.value = tokens[i].value;
            node.child.push_back(arg);
            i++;
            continue;
        }

        if (tokens[i].type == TOKENTYPE::IDENT)
        {
            NODE arg;
            arg.nodetype = NODETYPE::IDENT;
            arg.value = tokens[i].value;
            node.child.push_back(arg);
            i++;
            continue;
        }
        i++;
    }

    if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::RPAREN)
    {
        i++;
    }
    if (i < static_cast<int>(tokens.size()) && tokens[i].type == TOKENTYPE::SEMI)
    {
        i++;
    }

    return node;
}
