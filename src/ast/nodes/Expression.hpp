#ifndef TKOM_AST_NODES_EXPRESSION
#define TKOM_AST_NODES_EXPRESSION

#include "../AST.hpp"
#include "../../Token/Token_Types.h"

using Node = ast::Node;

namespace ast
{
    class Expression: public Assignable
    {
    public:
        void addOperand(const ast::NodePtr& node)
        {
            this->operands.push_back(node);
        }
        void addOperator(const tokenUtils::TokenTypes& operation)
        {
            this->operations.push_back(operation);
        }
        virtual Type getType()
        {
            return Node::Type::Expression;
        }

        std::vector<tokenUtils::TokenTypes> operations;
        std::vector<ast::NodePtr> operands;
    };
}

#endif
