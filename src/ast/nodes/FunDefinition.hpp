#ifndef TKOM_AST_NODES_FUNDEFINITION
#define TKOM_AST_NODES_FUNDEFINITION

#include "../AST.hpp"

#include <vector>
#include <string>

using Node = ast::Node;

namespace ast
{
    class StatementBlock;

    class FunDefinition: public Node
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        void setParameters(const std::vector<std::pair<std::string, std::string>>& parameters)
        {
            this->parameters = std::move(parameters);
        }
        void setBlock(const std::shared_ptr<StatementBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        void setReturnType(const std::string type)
        {
            this->returnType = type;
        }
        virtual Type getType()
        {
            return Node::Type::FunDefinition;
        }

        bool isPublic = true;
        std::string name;
        std::string returnType;
        std::vector<std::pair<std::string, std::string>> parameters;
        std::shared_ptr<StatementBlock> blockNode;
    };
}

#endif
