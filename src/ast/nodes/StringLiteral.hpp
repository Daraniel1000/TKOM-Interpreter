#ifndef TKOM_AST_NODES_STRINGLITERAL
#define TKOM_AST_NODES_STRINGLITERAL

#include "../AST.hpp"
#include <string>

using Node = ast::Node;

namespace ast
{
    class StringLiteral: public Assignable
    {
    public:
        void setLiteral(const std::string literal)
        {
            this->content = literal;
        }
        virtual Type getType()
        {
            return Node::Type::StringLiteral;
        }
        StringLiteral(std::string s)
        {
            content = s;
        }
        std::string content;
    };
}

#endif
