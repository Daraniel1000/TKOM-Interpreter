#ifndef TKOM_AST_NODES_DELETESTATEMENT
#define TKOM_AST_NODES_DELETESTATEMENT

#include "../AST.hpp"
#include <string>

using Node = ast::Node;

namespace ast
{
    class DeleteStatement: public Node
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        virtual Type getType()
        {
            return Node::Type::DeleteStatement;
        }

        std::string name;
    };
}

#endif

