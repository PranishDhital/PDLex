#include "interpreter.h"

void interpreter::interpret(const NODE &node)
{
    if (node.nodetype == NODETYPE::VARIABLE_DECLARATION)
    {
        int value = 0;
        if (!node.child.empty())
        {
            const std::string &raw = node.child[0].value;
            try
            {
                size_t pos = 0;
                int parsed = std::stoi(raw, &pos);
                if (pos == raw.size())
                {
                    value = parsed;
                }
                else
                {
                    if (variables.find(raw) == variables.end())
                    {
                        std::cerr << "Runtime Error: undefined variable " << raw << "\n";
                        return;
                    }
                    value = variables[raw];
                }
            }
            catch (...)
            {
                if (variables.find(raw) == variables.end())
                {
                    std::cerr << "Runtime Error: undefined variable " << raw << "\n";
                    return;
                }
                value = variables[raw];
            }
        }

        variables[node.value] = value;
        return;
    }

    if (node.nodetype == NODETYPE::PRINT_STATEMENT)
    {
        for (size_t i = 0; i < node.child.size(); i++)
        {
            const NODE &arg = node.child[i];
            if (arg.nodetype == NODETYPE::IDENT)
            {
                if (variables.find(arg.value) == variables.end())
                {
                    std::cerr << "Runtime Error: undefined variable " << arg.value << "\n";
                    return;
                }
                std::cout << variables[arg.value];
            }
            else
            {
                std::cout << arg.value;
            }
            if (i + 1 < node.child.size())
            {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}
