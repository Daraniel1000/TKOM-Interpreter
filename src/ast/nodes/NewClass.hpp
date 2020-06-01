#ifndef TKOM_AST_NODES_NEWCLASS
#define TKOM_AST_NODES_NEWCLASS

#include "../AST.hpp"
#include <string>

using Node = ast::Node;

namespace ast
{
    class NewClass: public Call
    {
    public:
        void setName(const std::string& name)
        {
            //this->className = name;
            //this->name = "Constructor";
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

    };
}

#endif


