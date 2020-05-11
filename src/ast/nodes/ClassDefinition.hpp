#ifndef TKOM_AST_NODES_CLASSDEFINITION
#define TKOM_AST_NODES_CLASSDEFINITION

#include "../AST.hpp"
#include <string>

using Node = ast::Node;

namespace ast
{

    class ClassDefinition: public Node
    {
    public:
        void addFunction(bool isPublic, const std::shared_ptr<FunDefinition>& function)
        {
            this->functions.push_back(std::make_pair(isPublic, std::move(function)));
        }
        void addVar(bool isPublic, const std::shared_ptr<VarDeclaration>& variable)
        {
            this->variables.push_back(std::make_pair(isPublic, std::move(variable)));
        }
        void setName(const std::string& name)
        {
            this->name = name;
        }
        virtual Type getType()
        {
            return Node::Type::Program;
        }

        std::string name;
        std::vector<std::pair<bool, std::shared_ptr<FunDefinition>>> functions = {};
        std::vector<std::pair<bool, std::shared_ptr<VarDeclaration>>> variables = {};
    };
}

#endif

