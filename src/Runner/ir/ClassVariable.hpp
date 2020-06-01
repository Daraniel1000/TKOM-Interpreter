#ifndef TKOM_IR_CLASSVARIABLE
#define TKOM_IR_CLASSVARIABLE

#include <string>
#include <unordered_map>
#include "Variable.hpp"
#include "Scope.hpp"
#include "Function.hpp"

namespace ir
{
    class ClassVariable : public Variable
    {
    public:

        //ClassVariable(Scope* globalScope)
        ClassVariable()
        {
            privateVariables.setUpperScope(&variables);
            //variables.setUpperScope(globalScope);
        }

        ClassVariable(std::string s)
        {
            className = s;
            privateVariables.setUpperScope(&variables);
        }

        varType getType()
        {
            return varType::CLASS;
        }


        Scope variables;
        Scope privateVariables;
        bool initialized = false;
        std::unordered_map<std::string, ast::FunDefinition> definedFunctions;
        std::string className = "";
    };
}

#endif // TKOM_IR_VARIABLE



