#ifndef TKOM_AST_NODES_VARDECLARATION
#define TKOM_AST_NODES_VARDECLARATION

#include "../AST.hpp"

using Node = ast::Node;

namespace ast
{
    class VarDeclaration: public Node
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        void setValue(const std::shared_ptr<Assignable>& assignableNode)
        {
            this->assignableNode = std::move(assignableNode);
        }
        void setVarType(int type)
        {
            this->varType = type;
        }
        void setClassType(const std::string& type)
        {
            this->classType = type;
        }
        virtual Type getType()
        {
            return Node::Type::VarDeclaration;
        }

        std::string name;
        int varType;
        std::string classType = "";
        std::shared_ptr<Assignable> assignableNode;
    };
}

#endif
