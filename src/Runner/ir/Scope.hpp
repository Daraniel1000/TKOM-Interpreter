#ifndef TKOM_IR_SCOPE
#define TKOM_IR_SCOPE

#include "Variable.hpp"
#include <unordered_map>
#include <memory>

namespace ir
{
    class Scope
    {
    public:

        ~Scope()
        {
            for(auto it: variables)
            {
                delete it.second;
            }
        };

        Scope* upperScope = nullptr;

        std::unordered_map<std::string, Variable*> variables;

        void setUpperScope(Scope* s)
        {
            upperScope = s;
        }

        Variable* getVariable(const std::string& name, bool checkUpper = true)
        {
            auto it = this->variables.find(name);
            if (it != this->variables.end())
            {
                return it->second;//.get();
            }

            if (this->upperScope != nullptr && checkUpper)
            {
                return this->upperScope->getVariable(name);
            }

            return nullptr;
        }

        bool addVariable(Variable* var)
        {
            if (this->hasVariable(var->name, false))
            {
                delete var;
                return false;
            }
            this->variables.insert(std::make_pair(var->name, var));
            return true;
        }

         bool hasVariable(const std::string& name, bool checkUpper = true)
        {
            return this->getVariable(name, checkUpper) != nullptr;
        }


    };
}

#endif // TKOM_IR_SCOPE




