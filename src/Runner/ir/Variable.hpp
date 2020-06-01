#ifndef TKOM_IR_VARIABLE
#define TKOM_IR_VARIABLE

#include <string>

namespace ir
{
    enum varType
    {
        VOID = 0,
        INT = 1,
        STRING = 2,
        CHAR = 3,
        CLASS = 4
    };

    class Variable
    {
    public:
        virtual ~Variable(){};
        virtual varType getType(){
        return VOID;
        };

        std::string name;
        bool returned = false;

    };
}

#endif
