#ifndef TKOM_AST_NODES_ASSIGNMENT
#define TKOM_AST_NODES_ASSIGNMENT

#include "../AST.hpp"

using Node = ast::Node;

namespace ast
{
    class Variable;
    class Assignable;

    class Assignment: public Node
    {
    public:
        void setVariable(const std::shared_ptr<Variable>& variable)
        {
            this->variable = variable;
        }
        void setValue(const std::shared_ptr<Assignable>& value)
        {
            this->value = value;
        }

        virtual Type getType()
        {
            return Node::Type::Assignment;
        }

        std::shared_ptr<Variable> variable;
        std::shared_ptr<Assignable> value;
    };
}

#endif
