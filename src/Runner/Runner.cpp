
#include "Runner.h"
#include <string>
#include <exception>
#include "../Token/Token_Types.h"

using TokenTypes = tokenUtils::TokenTypes;

void Runner::runProgram(ast::Program& program)
{
    for(auto it: program.classes)
    {
        if(definedClasses.find(it->name) != definedClasses.end())
            throw std::runtime_error("Multiple classes named " + it->name);
        definedClasses.insert(std::make_pair(it->name, *it));
        /*ir::ClassVariable ClassTemplate;
        ClassTemplate.setName(it->name);
        ast::FunDefinition* fun;
        for(int j=0; j<it->functions.size(); ++j)
        {
            fun = it->functions[j].second.get();
            ir::Function func(globalScope);
            func.setName(fun->name);
            func.instructions = *(fun->blockNode);
            for(auto parameter: )
            ClassTemplate.functions.insert(std::make_pair())
        }*/
    }
    for(auto it: program.variables)
    {
        if(!globalScope.addVariable(makeVariable(it.get(), globalScope)))
            throw std::runtime_error("Multiple variables named " + it->name);

    }
    for (auto it: program.functions)
    {
        if(definedFunctions.find(it->name) != definedFunctions.end())
            throw std::runtime_error("Multiple functions named " + it->name);
        if(this->getTypeFromString(it->returnType) == ir::varType::CLASS )
        {
            if(definedClasses.find(it->returnType) == definedClasses.end())
                throw std::runtime_error("Function " + it->name + " returns undefined type " + it->returnType);
        }
        definedFunctions.insert(std::make_pair(it->name, *it));
    }
    ir::Scope mainScope;
    mainScope.setUpperScope(&globalScope);
    runStatementBlock(program.main->blockNode.get(), mainScope);
    clearScope(mainScope);
    clearScope(globalScope);
}

int Runner::runExpression(ast::Expression* Expression, ir::Scope& localScope)
{
    int term = runTerm(dynamic_cast<ast::Expression*>(Expression->operands[0].get()), localScope);
    if(Expression->operands.size() == 1) return term;
    int exp = runExpression(dynamic_cast<ast::Expression*>(Expression->operands[1].get()), localScope);
    switch(Expression->operation)
    {
    case TokenTypes::Plus:
        return term + exp;
    case TokenTypes::Minus:
        return term - exp;
    default:
        throw std::runtime_error("Unexpected operator in expression");
    }
}

int Runner::runTerm(ast::Expression* term, ir::Scope& localScope)
{
    int factor = runFactor(term->operands[0].get(), localScope);
    if(term->operands.size() == 1) return factor;
    int term2 = runTerm(dynamic_cast<ast::Expression*>(term->operands[1].get()), localScope);
    switch(term->operation)
    {
    case TokenTypes::Multiply:
        return factor * term2;
    case TokenTypes::Divide:
        return factor/term2;
    default:
        throw std::runtime_error("Unexpected operator in expression");
    }
}

int Runner::runFactor(ast::Node* factor, ir::Scope& localScope)
{

    switch(factor->getType())
    {
    case ast::Node::Type::Expression:
        return runExpression(static_cast<ast::Expression*>(factor), localScope);
    case ast::Node::Type::NumberLiteral:
        return static_cast<ast::NumberLiteral*>(factor)->data;
    case ast::Node::Type::Variable:
        break;
    default:
        throw std::runtime_error("Trying to assign a NaN value to a numeric expression");
    }
    ast::Variable* var = static_cast<ast::Variable*>(factor);
    ir::Variable* variable;
    if(var->className != "")
    {
        variable = localScope.getVariable(var->className);
        if(variable == nullptr)
            throw std::runtime_error("Trying to access not-in-scope class variable " + var->className);
        if(variable->getType() != ir::CLASS)
            throw std::runtime_error("Trying to access non-class variable " + var->className + " as class");
        ir::ClassVariable* classVar = static_cast<ir::ClassVariable*>(variable);
        variable = classVar->variables.getVariable(var->variableName);
        if(variable == nullptr)
            throw std::runtime_error("Trying to access not-in-scope variable " + var->className + "." + var->variableName);
    }
    else
    {
        variable = localScope.getVariable(var->variableName);
        if(variable == nullptr)
            throw std::runtime_error("Trying to access not-in-scope variable " + var->className);
    }
    if(variable->getType() != ir::CHAR && variable->getType() != ir::INT)
    {
        throw std::runtime_error("Trying to assign a NaN value to a numeric expression");
    }
    if(var->indexArg == nullptr) return static_cast<ir::NumericVariable*>(variable)->value[0];
    ir::NumericVariable indexArg;
    assignVariable(&indexArg, var->indexArg.get(), localScope);
    return static_cast<ir::NumericVariable*>(variable)->value[indexArg.value[0]];
}

void Runner::copyVariable(ir::Variable* copyTo, ir::Variable* copyFrom, ir::Scope& scope, int indexArgTo, int indexArgFrom)
{
    if(copyTo->getType() == ir::CLASS)
    {
        if(indexArgTo != -1) throw std::runtime_error("Trying to access index in class variable " + copyTo->name);
        if(indexArgFrom != -1) throw std::runtime_error("Trying to access index in class variable " + copyFrom->name);
        ir::ClassVariable *to, *from;
        to = static_cast<ir::ClassVariable*>(copyTo);
        from = static_cast<ir::ClassVariable*>(copyFrom);
        if(to->className != from->className) throw std::runtime_error("Trying to copy " + from->className + " variable " + from->name + " to a " + to->className + " variable " + to->name);
        for(auto it: from->variables.variables)
        {
            copyVariable(to->variables.getVariable(it.first), it.second, scope);
        }
    }
    else if(copyTo->getType() == ir::STRING)
    {
        ir::StringVariable *to, *from;
        to = static_cast<ir::StringVariable*>(copyTo);
        from = static_cast<ir::StringVariable*>(copyFrom);
        if(indexArgFrom == -1 && indexArgTo == -1)
        {
            to->value = from->value;
            return;
        }
        if(indexArgFrom == -1)
        {
            throw std::runtime_error("Trying to copy a string to a letter");
        }
        if(indexArgTo == -1)
        {
            to->value = from->value[indexArgFrom];
            return;
        }
        to->value[indexArgTo] = from->value[indexArgFrom];
    }
    else    //numeric
    {
        ir::NumericVariable *to, *from;
        to = static_cast<ir::NumericVariable*>(copyTo);
        from = static_cast<ir::NumericVariable*>(copyFrom);
        if(indexArgFrom<0) indexArgFrom=0;
        if(indexArgTo<0) indexArgTo=0;
        to->value[indexArgTo] = from->value[indexArgFrom];
    }
}

void Runner::copyVariable(ir::Variable* copyTo, ast::Variable* copyFrom, ir::Scope& scope, int indexArg)
{
    ir::Variable* var;// = scope.getVariable(copyFrom->variableName);
    if(copyFrom->className != "")
    {
        ir::ClassVariable* classVar = dynamic_cast<ir::ClassVariable*>(scope.getVariable(copyFrom->className));
        if(classVar == nullptr) throw std::runtime_error("Trying to copy from not-in-scope class variable " + copyFrom->className);
        var = classVar->variables.getVariable(copyFrom->variableName);
        if(var == nullptr) throw std::runtime_error("Trying to copy from not-in-scope variable " + copyFrom->className + "." + copyFrom->variableName);
    }
    else
    {
        var = scope.getVariable(copyFrom->variableName);
        if(var == nullptr) throw std::runtime_error("Trying to copy from not-in-scope variable " + copyFrom->variableName);
    }
    if(copyTo->getType() != var->getType())
    {
        if(!((copyTo->getType() == ir::INT || copyTo->getType() == ir::INT) && (var->getType() == ir::INT || var->getType() == ir::INT)))
        {
            throw std::runtime_error("Trying to copy different type variables: " + var->name + " to " + copyTo->name);
        }
    }
    ir::NumericVariable indexArgFrom;
    indexArgFrom.value[0] = -1;
    if(copyFrom->indexArg != nullptr)
        assignVariable(&indexArgFrom, copyFrom->indexArg.get(), scope);
    copyVariable(copyTo, var, scope, indexArg, indexArgFrom.value[0]);
}

void Runner::clearScope(ir::Scope& scope)
{
    for(auto it: scope.variables)
    {
        if(it.second->getType() == ir::CLASS)
        {
            deleteVar(it.first, scope);
        }
    }
}

void Runner::deleteVar(std::string toDelete, ir::Scope& scope)
{
    ir::Variable* var = scope.getVariable(toDelete);
    if(var == nullptr) throw std::runtime_error("Trying to delete nonexistent variable " + var->name);
    if(var->getType() == ir::CLASS)
    {
        ir::ClassVariable* classVar = static_cast<ir::ClassVariable*>(var);
        if(!classVar->initialized) return;  //no error on destroying uninitialized class, but nothing happens
        classVar->variables.setUpperScope(&globalScope);
        ast::Call emptyCall;
        if(classVar->definedFunctions.find("Destructor") != classVar->definedFunctions.end())
        {
            if(!classVar->definedFunctions.at("Destructor").parameters.empty()) throw std::runtime_error("Destructor of class " + classVar->className + " takes arguments");
            callFunction(&classVar->definedFunctions.at("Destructor"), &emptyCall, scope, classVar->privateVariables);
        }
        classVar->variables.setUpperScope(nullptr);
        classVar->initialized = false;
    }
    else if(var->getType() == ir::STRING)
    {
        static_cast<ir::StringVariable*>(var)->value = "";
    }
    else    //numeric
    {
        static_cast<ir::NumericVariable*>(var)->value.resize(1);
        static_cast<ir::NumericVariable*>(var)->value[0] = 0;
    }
}

int Runner::runStatementBlock(ast::StatementBlock* block, ir::Scope& localScope, ir::Variable* returnval, bool breakable)
{
    for(auto it: block->instructions)
    {
        switch(it->getType())
        {
        case ast::Node::Type::VarDeclaration:
            if(!localScope.addVariable(makeVariable(static_cast<ast::VarDeclaration*>(it.get()), localScope)))
                throw std::runtime_error("Multiple variables named " + static_cast<ast::VarDeclaration*>(it.get())->name);
            break;
        case ast::Node::Type::SwitchStatement:
            if(runSwitchStatement(static_cast<ast::SwitchStatement*>(it.get()), localScope, returnval) == 1)
            {
                return 1;
            }
            break;
        case ast::Node::Type::WhileStatement:
            if(runWhileStatement(static_cast<ast::WhileStatement*>(it.get()), localScope, returnval) == 1)
                return 1;
            break;
        case ast::Node::Type::ReturnStatement:
            if(returnval == nullptr)
            {
                if(static_cast<ast::ReturnStatement*>(it.get())->assignableNode != nullptr)
                    throw std::runtime_error("Trying to return a value in a void function or main");
                return 1;
            }
            assignVariable(returnval, static_cast<ast::ReturnStatement*>(it.get())->assignableNode.get(), localScope);
            returnval->returned = true;
            return 1;
        case ast::Node::Type::Assignment:
            assignVariable(static_cast<ast::Assignment*>(it.get()), localScope);
            break;
        case ast::Node::Type::Call:
            callFunction(static_cast<ast::Call*>(it.get()), localScope, globalScope);
            break;
        case ast::Node::Type::LoopJump:
            if(breakable) return 2;
            throw std::runtime_error("Trying to break out of non-breakable block");
            break;
        case ast::Node::Type::DeleteStatement:
            deleteVar(static_cast<ast::DeleteStatement*>(it.get())->name, localScope);
            break;
        default:
            throw std::runtime_error("Unrecognized node in statement block, should be impossible");
            break;
        }
    }
    return 0;
}

bool Runner::runConditionStatement(ast::Condition* condition, ir::Scope& localScope)
{
    int exp1, exp2;
    exp1 = runExpression(condition->operand1.get(), localScope);
    exp2 = runExpression(condition->operand2.get(), localScope);
    switch(condition->operation)
    {
    case TokenTypes::Equality:
        return exp1 == exp2;
    case TokenTypes::Inequality:
        return exp1 != exp2;
    case TokenTypes::Less:
        return exp1 < exp2;
    case TokenTypes::Greater:
        return exp1 > exp2;
    case TokenTypes::LessOrEqual:
        return exp1 <= exp2;
    case TokenTypes::GreaterOrEqual:
        return exp1 >= exp2;
    default:
        throw std::runtime_error("Unexpected operation token in condition statement");  //nie powinno byæ mo¿liwe
        break;
    }
    return false;
}

int Runner::runSwitchStatement(ast::SwitchStatement* statement, ir::Scope& localScope, ir::Variable* returnVal)
{
    int condition = runExpression(statement->conditionNode.get(), localScope);
    int b;
    for(auto it: statement->caseNodes)
    {
        if(condition == runExpression(it.first.get(), localScope))
        {
            ir::Scope innerScope;
            innerScope.setUpperScope(&localScope);
            b = runStatementBlock(it.second.get(), innerScope, returnVal, false);
            clearScope(innerScope);
            return b;
        }
    }
    if(statement->defaultNode != nullptr)
    {
        ir::Scope innerScope;
        innerScope.setUpperScope(&localScope);
        b = runStatementBlock(statement->defaultNode.get(), innerScope, returnVal, false);
        clearScope(innerScope);
        return b;
    }
    return 0;
}

int Runner::runWhileStatement(ast::WhileStatement* statement, ir::Scope& localScope, ir::Variable* returnVal)
{
    int b;
    while(runConditionStatement(statement->conditionNode.get(), localScope))
    {
        ir::Scope whileScope;
        whileScope.setUpperScope(&localScope);
        b = runStatementBlock(statement->blockNode.get(), whileScope, returnVal, true);
        clearScope(whileScope);
        if(b) return b;
    }
    return 0;
}

void Runner::assignVariable(ast::Assignment* assignment, ir::Scope& scope)
{
    if(assignment->variable->className != "")
    {
        if(!scope.hasVariable(assignment->variable->className))
            throw std::runtime_error("Trying to assign to nonexistent variable " + assignment->variable->className);
        ir::ClassVariable* classVar = dynamic_cast<ir::ClassVariable*>(scope.getVariable(assignment->variable->className));
        if(classVar == nullptr)
            throw std::runtime_error("Trying to access non-class variable " + assignment->variable->className + " as a class");
        if(!classVar->initialized)
            throw std::runtime_error("Trying to access inner variables of non-initialized class variable " + assignment->variable->className);
        if(!classVar->variables.hasVariable(assignment->variable->variableName))
            throw std::runtime_error("Class " + classVar->className + " has no public variable named " + assignment->variable->variableName);
        ir::NumericVariable indexArg;
        indexArg.value[0] = -1;
        if(assignment->variable->indexArg != nullptr)
        {
            assignVariable(&indexArg, assignment->variable->indexArg.get(), scope);
            if(indexArg.value[0]<0) throw std::runtime_error("Trying to access negative index");
        }
        assignVariable(classVar->variables.getVariable(assignment->variable->variableName), assignment->value.get(), scope, indexArg.value[0]);
    }
    else
    {
        if(!scope.hasVariable(assignment->variable->variableName))
            throw std::runtime_error("No variable in scope named " + assignment->variable->variableName);
        ir::NumericVariable indexArg;
        if(assignment->variable->indexArg != nullptr)
            assignVariable(&indexArg, assignment->variable->indexArg.get(), scope);
        if(indexArg.value[0]<0) throw std::runtime_error("Trying to access negative index");
        indexArg.value[0] = -1;
        if(assignment->variable->indexArg != nullptr)
        {
            assignVariable(&indexArg, assignment->variable->indexArg.get(), scope);
            if(indexArg.value[0]<0) throw std::runtime_error("Trying to access negative index");
        }
        assignVariable(scope.getVariable(assignment->variable->variableName), assignment->value.get(), scope, indexArg.value[0]);
    }
}

void Runner::assignVariable(ir::Variable* variable, ast::Assignable* assignable, ir::Scope& scope, int indexArg)
{
    ir::ClassVariable* classVar;
    ast::Expression* ex;
    ast::NewVariable* newVar;
    switch(variable->getType())
    {
    case ir::CLASS:
        if(indexArg != -1)
            throw std::runtime_error("Trying to access class as an array");
        switch(assignable->getType())
        {
        case ast::Node::Type::NewClass:
            classVar = static_cast<ir::ClassVariable*>(variable);
            if(static_cast<ast::NewClass*>(assignable)->name != classVar->className)
                throw std::runtime_error("Trying to initialize class variable " + variable->name + " with a different class type");
            classVar->variables.setUpperScope(&globalScope);
            if(classVar->definedFunctions.find("Constructor") != classVar->definedFunctions.end())
                callFunction(&classVar->definedFunctions.at("Constructor"), static_cast<ast::Call*>(assignable), scope, classVar->privateVariables);
            classVar->variables.setUpperScope(nullptr);
            classVar->initialized = true;
            break;
        case ast::Node::Type::Call:
            classVar = callClassFunction(static_cast<ast::Call*>(assignable), scope, globalScope);
            if(classVar->className != static_cast<ir::ClassVariable*>(variable)->className)
            {
                delete classVar;
                throw std::runtime_error("Trying to assign a wrong type to variable " + variable->name);
            }
            std::swap(classVar->variables, static_cast<ir::ClassVariable*>(variable)->variables);
            std::swap(classVar->privateVariables, static_cast<ir::ClassVariable*>(variable)->privateVariables);
            delete classVar;
            static_cast<ir::ClassVariable*>(variable)->initialized = true;
            break;
        case ast::Node::Type::Expression:
            ex = static_cast<ast::Expression*>(assignable);
            if(ex->operands.size() == 1)
            {
                ex = dynamic_cast<ast::Expression*>(ex->operands.front().get());
                if(ex->operands.size() == 1 && ex->operands.front()->getType() == ast::Node::Type::Variable)
                    copyVariable(variable, dynamic_cast<ast::Variable*>(ex->operands.front().get()), scope);
            }
            else throw std::runtime_error("Trying to assign non-class expression to class variable " + variable->name);
            break;
        default:
            throw std::runtime_error("Trying to assign non-class assignable to class variable " + variable->name);
            break;
        }
        break;
    case ir::STRING:
        if(indexArg != -1)
            throw std::runtime_error("Trying to access string as an array");
        switch(assignable->getType())
        {
        case ast::Node::Type::StringLiteral:
            static_cast<ir::StringVariable*>(variable)->value = static_cast<ast::StringLiteral*>(assignable)->content;
            break;
        case ast::Node::Type::Call:
            static_cast<ir::StringVariable*>(variable)->value = callStringFunction(static_cast<ast::Call*>(assignable), scope, globalScope);
            break;
        case ast::Node::Type::Expression:
            ex = static_cast<ast::Expression*>(assignable);
            if(ex->operands.size() == 1)
            {
                ex = dynamic_cast<ast::Expression*>(ex->operands.front().get());
                if(ex->operands.size() == 1 && ex->operands.front()->getType() == ast::Node::Type::Variable)
                    copyVariable(variable, dynamic_cast<ast::Variable*>(ex->operands.front().get()), scope, indexArg);
            }
            else throw std::runtime_error("Trying to assign non-string expression to string variable " + variable->name);
            break;
        default:
            throw std::runtime_error("Trying to assign non-string assignable to string variable " + variable->name);
            break;
        }
        break;
    default:    //numeric
        switch(assignable->getType())
        {
        case ast::Node::Type::Call:
            static_cast<ir::NumericVariable*>(variable)->value[indexArg<0 ? 0: indexArg] = callNumericFunction(static_cast<ast::Call*>(assignable), scope, globalScope);
            break;
        case ast::Node::Type::NewVariable:
            if(indexArg != -1) throw std::runtime_error("Trying to assign new variable to array part");
            newVar = static_cast<ast::NewVariable*>(assignable);
            if(getTypeFromString(newVar->varType) != variable->getType())
                throw std::runtime_error("Trying to assign wrong type to numeric variable " + variable->name);
            if(newVar->isArray)
                static_cast<ir::NumericVariable*>(variable)->value.resize(runExpression(newVar->Capacity.get(), scope), 0);
            break;
        case ast::Node::Type::Expression:
            static_cast<ir::NumericVariable*>(variable)->value[indexArg<0 ? 0: indexArg] = runExpression(static_cast<ast::Expression*>(assignable), scope);
            break;
        default:
            throw std::runtime_error("Trying to assign non-numeric assignable to numeric variable " + variable->name);
            break;
        }
        break;
    }
}

ir::varType Runner::getTypeFromString(std::string s)
{
    if(s=="int") return ir::varType::INT;
    if(s=="char") return ir::varType::CHAR;
    if(s=="string") return ir::varType::STRING;
    if(s=="void") return ir::varType::VOID;
    return ir::varType::CLASS;
}

ir::Variable* Runner::makeVariable(ast::VarDeclaration* var, ir::Scope& scope)
{
    switch(this->getTypeFromString(var->varType))
    {
    case ir::varType::STRING:
        return makeStringVariable(var, scope);
    case ir::varType::CLASS:
        return makeClassVariable(var, scope);
    case ir::varType::VOID:
        throw std::runtime_error("Trying to create a void variable named " + var->name);
    default:
        return makeNumericVariable(var, scope);
    }
}

ir::Variable* Runner::makeClassVariable(ast::VarDeclaration* var, ir::Scope& scope)
{
    if(definedClasses.find(var->varType) == definedClasses.end())
                throw std::runtime_error("Variable " + var->name + " is of undefined type " + var->varType);
    ast::ClassDefinition& classdef = definedClasses.at(var->varType);
    ir::ClassVariable* variable = new ir::ClassVariable();
    variable->name = var->name;
    variable->className = var->varType;
    bool b;
    for(auto it: classdef.variables)
    {
        b = true;
        if(it.first)
        {
            if(variable->privateVariables.hasVariable(it.second->name)) b = false;
            else b = variable->variables.addVariable(makeVariable(it.second.get(), variable->privateVariables));
        }
        else
        {
            b = variable->privateVariables.addVariable(makeVariable(it.second.get(), variable->privateVariables));
        }
        if(!b)
        {
            delete variable;
            throw std::runtime_error("Multiple variables named " + it.second->name + " in class " + var->varType);
        }
    }
    for(auto it: classdef.functions)
    {
        if(variable->definedFunctions.find(it.second->name) != variable->definedFunctions.end())
        {
            delete variable;
            throw std::runtime_error("Multiple functions named " + it.second->name + " in class " + var->varType);
        }
        it.second->isPublic = it.first;
        variable->definedFunctions.insert(std::make_pair(it.second->name, *(it.second)));
    }
    if(var->assignableNode != nullptr)
    {
        try
        {
            this->assignVariable(variable, var->assignableNode.get(), scope);
        }
        catch(std::runtime_error& e)
        {
            delete variable;
            throw e;
        }
    }
    return variable;
}

ir::Variable* Runner::makeNumericVariable(ast::VarDeclaration* var, ir::Scope& scope)
{
    ir::NumericVariable* variable = new ir::NumericVariable();
    variable->name = var->name;
    variable->setType(this->getTypeFromString(var->varType));
    if(var->assignableNode != nullptr)
    {
        try
        {
            this->assignVariable(variable, var->assignableNode.get(), scope);
        }
        catch(std::runtime_error& e)
        {
            delete variable;
            throw e;
        }
    }
    return variable;
}

ir::Variable* Runner::makeStringVariable(ast::VarDeclaration* var, ir::Scope& scope)
{
    ir::StringVariable* variable = new ir::StringVariable();
    variable->name = var->name;
    if(var->assignableNode != nullptr)
    {
        try
        {
            this->assignVariable(variable, var->assignableNode.get(), scope);
        }
        catch(std::runtime_error& e)
        {
            delete variable;
            throw e;
        }
        /*if(var->assignableNode->getType() == ast::Node::Type::StringLiteral)
        {
            variable->value = static_cast<ast::StringLiteral*>(var->assignableNode.get())->content;
        }
        else throw std::runtime_error("Trying to assign non-string value to a string variable");*/
    }
    return variable;
}

ast::FunDefinition* Runner::getFunDefinitionFromCall(ast::Call* call, ir::Scope& outerScope)
{
    ast::FunDefinition* fun;
    if(call->className != "")
    {
        ir::Variable* var = outerScope.getVariable(call->className);
        if(var == nullptr)
        {
            throw std::runtime_error("Trying to call nonexistent function " + call->className + "." + call->name);
        }
        if(var->getType() != ir::CLASS)
        {
            throw std::runtime_error("Trying to call nonexistent function " + call->className + "." + call->name);
        }
        ir::ClassVariable* cvar = static_cast<ir::ClassVariable*>(var);
        if(cvar->definedFunctions.find(call->name) == cvar->definedFunctions.end())
        {
            throw std::runtime_error("Trying to call nonexistent function " + call->className + "." + call->name);
        }
        fun = &cvar->definedFunctions.at(call->name);
    }
    else
    {
        if(definedFunctions.find(call->name) == definedFunctions.end())
        {
            throw std::runtime_error("Trying to call nonexistent function " + call->name);
        }
        fun = &definedFunctions.at(call->name);
    }
    return fun;
}

void Runner::callFunction(ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope)
{
    if(call->name == "print")
    {
        printValues(call, outerScope);
        return;
    }
    callFunction(getFunDefinitionFromCall(call, outerScope), call, outerScope, parentScope);
}

void Runner::callFunction(ast::FunDefinition* function, ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope)
{
    ir::Variable* dummyVar = nullptr;
    switch(getTypeFromString(function->returnType))
    {
    case ir::CLASS:
        dummyVar = new ir::ClassVariable(function->returnType);
        break;
    case ir::STRING:
        dummyVar = new ir::StringVariable();
        break;
    case ir::VOID:
        break;
    default:
        dummyVar = new ir::NumericVariable();
        break;
    }
    try
    {
        callFunction(function, call, outerScope, parentScope, dummyVar);
    }
    catch(std::runtime_error& e)
    {
        delete dummyVar;
        throw e;
    }
    if(dummyVar!= nullptr)
    {
        if(!dummyVar->returned)
        {
            delete dummyVar;
            throw std::runtime_error("Reached end of non-void function " + function->name + " without returning a value");
        }
    }
    delete dummyVar;
}

void Runner::callFunction(ast::FunDefinition* function, ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope, ir::Variable* returnVal)
{
    if(function==nullptr) return;
    ir::Scope funScope;
    for(int i=0; i<function->parameters.size(); ++i)
    {
        ast::VarDeclaration declaration;
        declaration.setName(function->parameters[i].second);
        declaration.setVarType(function->parameters[i].first);
        declaration.setValueCopy(call->arguments[i]);
        if(!funScope.addVariable(makeVariable(&declaration, outerScope)))
        {
            throw std::runtime_error("Multiple arguments called " + declaration.name +" in function " + function->name);
        }
    }
    funScope.setUpperScope(&parentScope);
    if(call->className != "")
    {
        ir::ClassVariable* cvar = static_cast<ir::ClassVariable*>(outerScope.getVariable(call->className));
        funScope.setUpperScope(&cvar->privateVariables);
    }
    runStatementBlock(function->blockNode.get(), funScope, returnVal);
    clearScope(funScope);
}

int Runner::callNumericFunction(ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope)
{
    ir::NumericVariable returnVal;
    callFunction(getFunDefinitionFromCall(call, outerScope), call, outerScope, parentScope, &returnVal);
    if(!returnVal.returned)
    {
        throw std::runtime_error("Reached end of non-void function " + call->name + " without returning a value");
    }
    return returnVal.value[0];
}

std::string Runner::callStringFunction(ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope)
{
    ir::StringVariable returnVal;
    callFunction(getFunDefinitionFromCall(call, outerScope), call, outerScope, parentScope, &returnVal);
    if(!returnVal.returned)
    {
        throw std::runtime_error("Reached end of non-void function " + call->name + " without returning a value");
    }
    return returnVal.value;
}

ir::ClassVariable* Runner::callClassFunction(ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope)
{
    ir::ClassVariable* returnVal;
    ast::FunDefinition* fun = getFunDefinitionFromCall(call, outerScope);
    ast::VarDeclaration declaration;
    declaration.setName(fun->name);
    declaration.setVarType(fun->returnType);
    returnVal = static_cast<ir::ClassVariable*>(makeClassVariable(&declaration, outerScope));
    try
    {
        callFunction(fun, call, outerScope, parentScope, returnVal);
    }
    catch(std::runtime_error& e)
    {
        delete returnVal;
        throw e;
    }
    if(!returnVal->returned)
    {
        delete returnVal;
        throw std::runtime_error("Reached end of non-void function " + call->name + " without returning a value");
    }
    return returnVal;
}

void Runner::printValues(ast::Call* call, ir::Scope& outerScope)
{
    ast::Expression* ex;
    ast::FunDefinition* fun;
    for(auto it: call->arguments)
    {
        switch(it->getType())
        {
        case ast::Node::Type::Expression:
            ex = static_cast<ast::Expression*>(it.get());
            if(ex->operands.size() == 1)
            {
                ex = dynamic_cast<ast::Expression*>(ex->operands.front().get());
                if(ex->operands.size() == 1 && ex->operands.front()->getType() == ast::Node::Type::Variable)
                {
                    printVariable(dynamic_cast<ast::Variable*>(ex->operands.front().get()), outerScope);
                    continue;
                }
            }
            std::cout<<runExpression(ex, outerScope)<<" ";
            break;
        case ast::Node::Type::StringLiteral:
            std::cout<<static_cast<ast::StringLiteral*>(it.get())->content;
            break;
        case ast::Node::Type::Call:
            fun = getFunDefinitionFromCall(static_cast<ast::Call*>(it.get()), outerScope);
            switch(getTypeFromString(fun->returnType))
            {
            case ir::CLASS:
                throw std::runtime_error("Print doesn't take class variables");
            case ir::STRING:
                std::cout<<callStringFunction(static_cast<ast::Call*>(it.get()), outerScope, globalScope)<<" ";
                break;
            case ir::VOID:
                std::cout<<" ";
                break;
            case ir::INT:
                std::cout<<callNumericFunction(static_cast<ast::Call*>(it.get()), outerScope, globalScope)<<" ";
                break;
            case ir::CHAR:
                std::cout<<(char)callNumericFunction(static_cast<ast::Call*>(it.get()), outerScope, globalScope)<<" ";
                break;
            }
            break;
        default:
            throw std::runtime_error("Print accepts only literals, expressions and function calls");
            break;

        }
    }
}

void Runner::printVariable(ast::Variable* var, ir::Scope& scope)
{
    ir::Variable* variable;
    ir::NumericVariable index;
    if(var->className != "")
    {
        variable = scope.getVariable(var->className);
        if(variable == nullptr) throw std::runtime_error("Trying to access not-in-scope variable " + var->className + "." + var->variableName);
        if(variable->getType()!=ir::CLASS) throw std::runtime_error("Trying to access non-class variable " + var->className + " as class");
        ir::ClassVariable* cvar = static_cast<ir::ClassVariable*>(variable);
        variable = cvar->variables.getVariable(var->variableName);
        if(variable == nullptr) throw std::runtime_error("Trying to access not-in-scope variable " + var->className + "." + var->variableName);
    }
    else
    {
        variable = scope.getVariable(var->variableName);
        if(variable == nullptr) throw std::runtime_error("Trying to access not-in-scope variable " + var->variableName);
    }
    switch(variable->getType())
    {
    case ir::CLASS:
        throw std::runtime_error("Print doesn't take class variables");
    case ir::STRING:
        if(var->indexArg == nullptr)
            std::cout<<static_cast<ir::StringVariable*>(variable)->value<<" ";
        else
        {
            assignVariable(&index, var->indexArg.get(), scope);
            std::cout<<static_cast<ir::StringVariable*>(variable)->value[index.value[0]]<<" ";
        }
        break;
    case ir::INT:
        if(var->indexArg == nullptr)
            std::cout<<static_cast<ir::NumericVariable*>(variable)->value[0]<<" ";
        else
        {
            assignVariable(&index, var->indexArg.get(), scope);
            std::cout<<static_cast<ir::NumericVariable*>(variable)->value[index.value[0]]<<" ";
        }
        break;
    case ir::CHAR:
        if(var->indexArg == nullptr)
            std::cout<<(char)static_cast<ir::NumericVariable*>(variable)->value[0]<<" ";
        else
        {
            assignVariable(&index, var->indexArg.get(), scope);
            std::cout<<(char)static_cast<ir::NumericVariable*>(variable)->value[index.value[0]]<<" ";
        }
        break;

    }
}
