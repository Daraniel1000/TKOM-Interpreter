#ifndef TKOM_AST_NODES_WHILESTATEMENT
#define TKOM_AST_NODES_WHILESTATEMENT

#include "../AST.hpp"

using Node = ast::Node;

namespace ast
{
    class WhileStatement: public Node
    {
    public:
        void setCondition(const std::shared_ptr<Condition>& conditionNode)
        {
            this->conditionNode = conditionNode;
        }
        void setBlock(const std::shared_ptr<StatementBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::WhileStatement;
        }

        std::shared_ptr<Condition> conditionNode;
        std::shared_ptr<StatementBlock> blockNode;
    };
}

#endif
