#ifndef TKOM_IR_NUMERICVARIABLE
#define TKOM_IR_NUMERICVARIABLE

#include "Variable.hpp"
#include <vector>

namespace ir
{
    class NumericVariable : public Variable
    {
    public:

        NumericVariable(): value({0}){};

        varType getType()
        {
            return type;
        }

        void setType(const varType& t)
        {
            type = t;
        }

        std::vector<int> value;

    private:
        varType type = INT;
    };
}

#endif // TKOM_IR_VARIABLE

