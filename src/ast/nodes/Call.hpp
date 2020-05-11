#ifndef TKOM_AST_NODES_CALL
#define TKOM_AST_NODES_CALL

#include "../AST.hpp"
#include <vector>

using Assignable = ast::Assignable;

namespace ast
{
    class Call: public Assignable
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        void setClassName(const std::string& name)
        {
            this->className = name;
        }
        void addArgument(const std::shared_ptr<Assignable>& assignableNode)
        {
            this->arguments.push_back(assignableNode);
        }
        virtual Type getType()
        {
            return Node::Type::Call;
        }

        std::string name;
        std::string className = "";
        std::vector<std::shared_ptr<Assignable>> arguments;
    };
}

#endif
