#ifndef TKOM_AST_NODES_RETURNSTATEMENT
#define TKOM_AST_NODES_RETURNSTATEMENT

#include "../AST.hpp"

using Node = ast::Node;

namespace ast
{
    class ReturnStatement: public Node
    {
    public:
        void setValue(const std::shared_ptr<Assignable>& assignableNode)
        {
            this->assignableNode = assignableNode;
        }
        virtual Type getType()
        {
            return Node::Type::ReturnStatement;
        }

        std::shared_ptr<Assignable> assignableNode = nullptr;
    };
}

#endif
