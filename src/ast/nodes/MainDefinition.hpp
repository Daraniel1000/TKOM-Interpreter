#ifndef TKOM_AST_NODES_MAINDEFINITION
#define TKOM_AST_NODES_MAINDEFINITION

#include "../AST.hpp"

using Node = ast::Node;

namespace ast
{
    class MainDefinition: public Node
    {
    public:
        void setBlock(const std::shared_ptr<StatementBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::Main;
        }

        std::shared_ptr<StatementBlock> blockNode;
    };
}

#endif

