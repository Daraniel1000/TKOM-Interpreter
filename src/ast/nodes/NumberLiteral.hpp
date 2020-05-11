#ifndef TKOM_AST_NODES_NUMBERLITERAL
#define TKOM_AST_NODES_NUMBERLITERAL

#include "../AST.hpp"

using Node = ast::Node;

namespace ast
{
    class NumberLiteral: public Node
    {
    public:
        void setValue(const int value)
        {
            data = value;
        }
        virtual Type getType()
        {
            return Node::Type::NumberLiteral;
        }

        int data = 0;
    };
}

#endif
