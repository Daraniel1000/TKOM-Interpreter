#ifndef TKOM_IR_STRINGVARIABLE
#define TKOM_IR_STRINGVARIABLE

#include "Variable.hpp"
#include <string>

namespace ir
{
    class StringVariable : public Variable
    {
    public:

        varType getType()
        {
            return STRING;
        }

        std::string value = "";
    private:
    };
}

#endif


