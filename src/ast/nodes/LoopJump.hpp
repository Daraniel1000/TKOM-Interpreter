#ifndef TKOM_AST_NODES_LOOPJUMP
#define TKOM_AST_NODES_LOOPJUMP

#include "../AST.hpp"

using Node = ast::Node;

namespace ast
{
    class LoopJump: public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::LoopJump;
        }
    };
}

#endif
