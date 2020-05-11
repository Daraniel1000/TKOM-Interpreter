#ifndef TKOM_AST_NODES_PROGRAM
#define TKOM_AST_NODES_PROGRAM

#include "../AST.hpp"
#include <iostream>

using Node = ast::Node;

namespace ast
{
    class MainDefinition;

    class Program: public Node
    {
    public:
        void addFunction(const std::shared_ptr<FunDefinition>& function)
        {
            this->functions.push_back(function);
        }
        void addClass(const std::shared_ptr<ClassDefinition>& Class)
        {
            this->classes.push_back(Class);
        }
        void addMain(const std::shared_ptr<MainDefinition>& mainptr)
        {
            this->main = std::move(mainptr);
        }
        void addVar(const std::shared_ptr<VarDeclaration>& variable)
        {
            this->variables.push_back(variable);
        }
        virtual Type getType()
        {
            return Node::Type::Program;
        }

        std::vector<std::shared_ptr<FunDefinition>> functions = {};
        std::vector<std::shared_ptr<ClassDefinition>> classes = {};
        std::vector<std::shared_ptr<VarDeclaration>> variables = {};
        std::shared_ptr<MainDefinition> main;
    };
}

#endif
