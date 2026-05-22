#include "interpreter.h"

void interpreter::interpret(const NODE &node)
{
    if (node.nodetype == NODETYPE::PRINT_STATEMENT)
    {
        for (size_t i = 0; i < node.child.size(); i++)
        {
            std::cout << node.child[i].value;
            if (i + 1 < node.child.size())
                std::cout << " ";
        }
        std::cout << "\n";
    }
}
