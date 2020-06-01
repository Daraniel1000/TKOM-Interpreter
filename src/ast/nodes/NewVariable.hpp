#ifndef TKOM_AST_NODES_NEWVARIABLE
#define TKOM_AST_NODES_NEWVARIABLE

#include "../AST.hpp"
#include <string>

using Node = ast::Node;

namespace ast
{
    class NewVariable: public Assignable
    {
    public:
        virtual Type getType()
        {
            return Node::Type::NewVariable;
        }
        NewVariable(const std::string type, bool array=false, const std::shared_ptr<ast::Expression>& cap=nullptr)
        {
            varType = type;
            if(array)
            {
                isArray = true;
                Capacity = cap;
            }
        }

        std::string varType = "";
        bool isArray = false;
        std::shared_ptr<ast::Expression> Capacity;
    };
}

#endif

