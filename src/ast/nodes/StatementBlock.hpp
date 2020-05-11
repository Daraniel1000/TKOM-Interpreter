#ifndef TKOM_AST_NODES_STATEMENTBLOCK
#define TKOM_AST_NODES_STATEMENTBLOCK

#include "../AST.hpp"

#include <vector>

using Node = ast::Node;

namespace ast
{
    class StatementBlock: public Node
    {
    public:
        void addInstruction(const ast::NodePtr& node)
        {
            this->instructions.push_back(node);
        }
        virtual Type getType()
        {
            return Node::Type::StatementBlock;
        }

        std::vector<ast::NodePtr> instructions;
    };
}

#endif
