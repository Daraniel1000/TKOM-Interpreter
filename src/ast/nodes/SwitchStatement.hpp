#ifndef TKOM_AST_NODES_IFSTATEMENT
#define TKOM_AST_NODES_IFSTATEMENT

#include "../AST.hpp"
#include <vector>

using Node = ast::Node;

namespace ast
{
    class StatementBlock;

    class SwitchStatement: public Node
    {
    public:
        void setCondition(const std::shared_ptr<Expression>& conditionNode)
        {
            this->conditionNode = std::move(conditionNode);
        }
        void setDefaultBlock(const std::shared_ptr<StatementBlock>& defaultBlockNode)
        {
            this->defaultNode = std::move(defaultBlockNode);
        }
        void addCaseBlock(const std::shared_ptr<Expression>& conditionNode, const std::shared_ptr<StatementBlock>& caseBlockNode)
        {
            this->caseNodes.push_back(std::make_pair(conditionNode, caseBlockNode));
        }
        virtual Type getType()
        {
            return Node::Type::SwitchStatement;
        }

        std::shared_ptr<Expression> conditionNode;
        std::shared_ptr<StatementBlock> defaultNode = nullptr;
        std::vector<std::pair<std::shared_ptr<Expression> ,std::shared_ptr<StatementBlock>>> caseNodes; //first - condition, second - actions
    };
}

#endif
