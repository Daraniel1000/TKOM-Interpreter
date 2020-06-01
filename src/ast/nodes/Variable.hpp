#ifndef TKOM_AST_NODES_VARIABLE
#define TKOM_AST_NODES_VARIABLE

#include "../AST.hpp"

using Node = ast::Node;

namespace ast
{
    class Variable: public Node
    {
    public:
        void setName(const std::string& name)
        {
            this->variableName = name;
        }
        void setClassName(const std::string& name)
        {
            this->className = name;
        }
        void setIndexArg(const std::shared_ptr<Assignable>& argument)
        {
            this->indexArg = std::move(argument);
        }
        virtual Type getType()
        {
            return Node::Type::Variable;
        }

        bool returning = false;
        std::string variableName;
        std::string className = "";
        std::shared_ptr<Assignable> indexArg = nullptr;
    };
}

#endif
