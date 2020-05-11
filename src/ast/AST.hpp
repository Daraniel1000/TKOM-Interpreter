#ifndef TKOM_AST
#define TKOM_AST

#include <memory>

namespace ast
{
    class Node
    {
    protected:
        std::weak_ptr<Node> parent;

    public:
        enum class Type {
            Main,
            Assignment,
            Call,
            Condition,
            Expression,
            FunDefinition,
            SwitchStatement,
            LoopJump,
            Program,
            ReturnStatement,
            StatementBlock,
            VarDeclaration,
            Variable,
            WhileStatement,
            StringLiteral,
            NumberLiteral,
            NewVariable,
            NewClass,
            DeleteStatement
        };

        virtual Type getType() = 0;
    };

    typedef std::shared_ptr<Node> NodePtr;
}

#include "nodes/Assignable.hpp"
#include "nodes/Assignment.hpp"
#include "nodes/Call.hpp"
#include "nodes/Expression.hpp"
#include "nodes/Condition.hpp"
#include "nodes/StringLiteral.hpp"
#include "nodes/NewVariable.hpp"
#include "nodes/NewClass.hpp"
#include "nodes/DeleteStatement.hpp"
#include "nodes/FunDefinition.hpp"
#include "nodes/SwitchStatement.hpp"
#include "nodes/LoopJump.hpp"
#include "nodes/NumberLiteral.hpp"
#include "nodes/VarDeclaration.hpp"
#include "nodes/ClassDefinition.hpp"
#include "nodes/Program.hpp"
#include "nodes/ReturnStatement.hpp"
#include "nodes/StatementBlock.hpp"
#include "nodes/MainDefinition.hpp"
#include "nodes/Variable.hpp"
#include "nodes/WhileStatement.hpp"

#endif
