#ifndef TKOM_IR_FUNCTION
#define TKOM_IR_FUNCTION

#include <string>
#include "Variable.hpp"
#include "../../ast/nodes/StatementBlock.hpp"

namespace ir
{
    class Function
    {
    public:

        varType getType()
        {
            return type;
        }

        void setName(const std::string& n)
        {
            name = n;
        }

        void setReturnType(const std::string& t)
        {
            if(t=="void") return;
            if(t=="int") this->type = varType::INT;
            else if(t=="string") this->type = varType::STRING;
            else if(t=="char") this->type = varType::CHAR;
            else
            {
                this->type = varType::CLASS;
                className = t;
            }

        }

        std::string getName() const
        {
            return name;
        }

        std::vector<std::pair<std::string, std::string>> parameters;
        ast::StatementBlock instructions;
        std::string className = "";
    private:
        varType type = varType::VOID;
        std::string name = "";
    };
}

#endif // TKOM_IR_FUNCTION




