#ifndef TKOM_MODULES_RUNNER
#define TKOM_MODULES_RUNNER

#include <unordered_map>
#include "../parser/Parser.h"
#include "../ast/AST.hpp"
#include "ir/includes.hpp"

class Runner
{
public:
    void runProgram(ast::Program& program);

private:

    ir::Scope globalScope;
    std::unordered_map<std::string, ast::FunDefinition> definedFunctions;
    std::unordered_map<std::string, ast::ClassDefinition> definedClasses;
    ir::Variable* makeVariable(ast::VarDeclaration* var, ir::Scope& scope);
    ir::Variable* makeNumericVariable(ast::VarDeclaration* var, ir::Scope& scope);
    ir::Variable* makeStringVariable(ast::VarDeclaration* var, ir::Scope& scope);
    ir::Variable* makeClassVariable(ast::VarDeclaration* var, ir::Scope& scope);
    ir::Function makeFunction(ast::FunDefinition* fun);
    void assignVariable(ast::Assignment* assignment, ir::Scope& scope);
    void assignVariable(ir::Variable* variable, ast::Assignable* assignable, ir::Scope& scope, int indexArg = -1);
    void copyVariable(ir::Variable* copyTo, ast::Variable* copyFrom, ir::Scope& scope, int indexArg = -1);
    void copyVariable(ir::Variable* copyTo, ir::Variable* copyFrom, ir::Scope& scope, int indexArgTo = -1, int indexArgFrom = -1);
    void deleteVar(std::string toDelete, ir::Scope& scope);

    ir::varType getTypeFromString(std::string s);
    void clearScope(ir::Scope& scope);
    ast::FunDefinition* getFunDefinitionFromCall(ast::Call* call, ir::Scope& outerScope);
    void printValues(ast::Call* call, ir::Scope& outerScope);
    void printVariable(ast::Variable* var, ir::Scope& scope);

    void callFunction(ast::FunDefinition* function, ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope, ir::Variable* returnVal);
    void callFunction(ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope);
    void callFunction(ast::FunDefinition* function, ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope);
    ir::ClassVariable* callClassFunction(ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope);
    int callNumericFunction(ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope);
    std::string callStringFunction(ast::Call* call, ir::Scope& outerScope, ir::Scope& parentScope);
    int runExpression(ast::Expression* expression, ir::Scope& localScope);
    int runTerm(ast::Expression* term, ir::Scope& localScope);
    int runFactor(ast::Node* factor, ir::Scope& localScope);
    int runStatementBlock(ast::StatementBlock* block, ir::Scope& localScope, ir::Variable* returnval = nullptr, bool breakable = false);
    int runSwitchStatement(ast::SwitchStatement* statement, ir::Scope& localScope, ir::Variable* returnval);
    int runWhileStatement(ast::WhileStatement* statement, ir::Scope& localScope, ir::Variable* returnval);
    bool runConditionStatement(ast::Condition* condition, ir::Scope& localScope);

};

#endif
