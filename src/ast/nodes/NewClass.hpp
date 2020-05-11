#ifndef TKOM_AST_NODES_NEWCLASS
#define TKOM_AST_NODES_NEWCLASS

#include "../AST.hpp"
#include <string>

using Node = ast::Node;

namespace ast
{
    class NewClass: public Assignable
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        void addArgument(const std::shared_ptr<Assignable>& assignableNode)
        {
            this->arguments.push_back(assignableNode);
        }
        virtual Type getType()
        {
            return Node::Type::NewClass;
        }

        std::string name;
        std::vector<std::shared_ptr<Assignable>> arguments;
    };
}

#endif


