#ifndef TKOM_AST_NODES_CONDITION
#define TKOM_AST_NODES_CONDITION

#include "../AST.hpp"
#include "../../Token/Token_Types.h"

using Node = ast::Node;
using TokenType = tokenUtils::TokenTypes;

namespace ast
{
    class Condition: public Node
    {
    public:
        void setOp1(const std::shared_ptr<ast::Expression>& node)
        {
            this->operand1 = std::move(node);
        }
        void setOp2(const std::shared_ptr<ast::Expression>& node)
        {
            this->operand2 = std::move(node);
        }
        void setOperator(const TokenType& operation)
        {
            this->operation = operation;
        }
        virtual Type getType()
        {
            return Node::Type::Condition;
        }

        TokenType operation = TokenType::Undefined;
        std::shared_ptr<ast::Expression>  operand1, operand2;
    };
}

#endif
