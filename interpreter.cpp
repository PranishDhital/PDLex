#include "interpreter.h"

void interpreter::interpret(const NODE &node)
{
    if (node.nodetype == NODETYPE::PRINT_STATEMENT)
    {
        for (const auto &arg : node.child)
        {
            std::cout << arg.value << " ";
        }
        std::cout << "\n";
    }
}
