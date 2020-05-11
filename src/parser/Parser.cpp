#include "Parser.hpp"
#include "../Lekser/Lekser.h"
#include <iostream>
#include <limits>

Parser::Parser(Lekser& lexer):
lexer(lexer)
{}

std::shared_ptr<ast::Program> Parser::parse()
{
    this->tracer.reset();
    this->resetPreviousToken();
    Token token;
    std::shared_ptr<ast::Program> syntaxTree = std::make_shared<ast::Program>();
    std::shared_ptr<ast::FunDefinition> lastFunction;

    this->tracer.enter("Starting parser tracing...");
    do
    {
        token = this->accept({TokenType::Class, TokenType::Main, TokenType::Void, TokenType::Int, TokenType::Char, TokenType::String});
        switch(token.type)
        {
        case TokenType::Class:
            syntaxTree->addClass(this->parseClass());
            break;
        case TokenType::Main:
            syntaxTree->addMain(this->parseMain());
            token = this->accept({TokenType::EndOfFile});
            break;
        case TokenType::Void:
            syntaxTree->addFunction(this->parseFunction(0));
            break;
        default:
            int type = 0;
            switch(token.type)
            {
            case TokenType::Int:
                type = 1;
                break;
            case TokenType::Char:
                type = 2;
                break;
            case TokenType::String:
                type = 3;
                break;
            default:
                break;
            }
            token = this->accept({TokenType::Identifier});
            if(this->peek({TokenType::ParenthOpen}))
            {
                syntaxTree->addFunction(this->parseFunction(type, token.value));
            }
            else
            {
                syntaxTree->addVar(this->parseInitStatement(type, token.value));
            }
            break;
        }
    } while (token.type != TokenType::EndOfFile);
    this->tracer.leave("Trace ended...");

    return syntaxTree;
}

bool Parser::isAcceptable(const Token& token, const std::initializer_list<TokenType>& acceptable) const
{
    for(auto& it: acceptable)
    {
        if (token.type == it)
        {
            return true;
        }
    }
    return false;
}

Token Parser::accept(const std::initializer_list<TokenType>& acceptable)
{
    Token token;
    if (this->hasBufferedToken())
    {
        token = this->previousToken;
        this->resetPreviousToken();
    }
    else
    {
        token = this->lexer.nextToken();
    }
    if (this->isAcceptable(token, acceptable))
    {
        return token;
    }
    else
    {
        throw std::invalid_argument(
            std::string("Unexpected token: ")
                .append(tokenUtils::getTokenTypeName(token.type))
                .append(" (Line: ")
                .append(std::to_string(token.line))
                .append(", Pos: ")
                .append(std::to_string(token.pos))
                .append(")")
                .append("\n")
                .append(this->lexer.getLine(token.lineStart))
                .append("\n")
                .append(this->makeErrorMarker(token.pos))
        );
        return token;
    }
}

bool Parser::peek(const std::initializer_list<TokenType>& acceptable)
{
    if (!this->hasBufferedToken())
    {
        this->previousToken = this->lexer.nextToken();
    }
    return this->isAcceptable(this->previousToken, acceptable);
}

Token Parser::getPeeked()
{
    if (!this->hasBufferedToken())
    {
        throw std::runtime_error(std::string("Nothing peeked"));
    }
    return this->previousToken;
}

const std::string Parser::makeErrorMarker(const unsigned int& pos)
{
    return std::string(pos, ' ').append("^");
}

bool Parser::hasBufferedToken() const
{
    return this->previousToken.type != TokenType::Undefined;
}

void Parser::resetPreviousToken()
{
    this->previousToken.type = TokenType::Undefined;
    this->previousToken.value = "";
    this->previousToken.line = 0;
    this->previousToken.pos = 0;
}


std::shared_ptr<ast::MainDefinition> Parser::parseMain()
{
    std::shared_ptr<ast::MainDefinition> node = std::make_shared<ast::MainDefinition>();
    this->tracer.enter("Parsing main");
    node->setBlock(this->parseStatementBlock());
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::ClassDefinition> Parser::parseClass()
{
    std::shared_ptr<ast::ClassDefinition> node = std::make_shared<ast::ClassDefinition>();
    this->tracer.enter("Parsing class");
    auto tempToken = this->accept({ TokenType::Identifier });
    node->setName(tempToken.value);
    this->accept({ TokenType::BracketOpen });
    tempToken = this->accept({ TokenType::BracketClose, TokenType::Void, TokenType::Int,
                             TokenType::Char, TokenType::String, TokenType::Constructor,
                             TokenType::Destructor, TokenType::Public, TokenType::Private });
    bool isPublic;
    while(tempToken.type != TokenType::BracketClose)
    {
        isPublic = true;
        if(tempToken.type == TokenType::Public || tempToken.type == TokenType::Private)
        {
            if(tempToken.type == TokenType::Private) isPublic = false;
            tempToken = this->accept({TokenType::Void, TokenType::Int, TokenType::Char,
                                      TokenType::String, TokenType::Constructor, TokenType::Destructor});
        }
        switch(tempToken.type)
        {
        case TokenType::Constructor:
            node->addFunction(isPublic, this->parseFunction(0, "Constructor"));
            break;
        case TokenType::Destructor:
            node->addFunction(isPublic, this->parseFunction(0, "Destructor"));
            break;
        case TokenType::Void:
            node->addFunction(isPublic, this->parseFunction(0));
            break;
        default:
            int type = 0;
            switch(tempToken.type)
            {
            case TokenType::Int:
                type = 1;
                break;
            case TokenType::Char:
                type = 2;
                break;
            case TokenType::String:
                type = 3;
                break;
            default:
                break;
            }
            tempToken = this->accept({TokenType::Identifier});
            if(this->peek({TokenType::ParenthOpen}))
            {
                node->addFunction(isPublic, this->parseFunction(type, tempToken.value));
            }
            else
            {
                node->addVar(isPublic, this->parseInitStatement(type, tempToken.value));
            }
            break;
        }
        tempToken = this->accept({ TokenType::BracketClose, TokenType::Void, TokenType::Int,
                             TokenType::Char, TokenType::String, TokenType::Constructor,
                             TokenType::Destructor, TokenType::Public, TokenType::Private });
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::FunDefinition> Parser::parseFunction(int type, string name)
{
    std::shared_ptr<ast::FunDefinition> node = std::make_shared<ast::FunDefinition>();
    this->tracer.enter("Parsing function");
    if(name == "")
    {
        auto tempToken = this->accept({ TokenType::Identifier });
        name = tempToken.value;
    }
    node->setName(name);
    node->setReturnType(type);
    node->setParameters(this->parseParameters());
    node->setBlock(this->parseStatementBlock());
    this->tracer.leave();
    return node;
}

std::vector<std::pair<int, std::string>> Parser::parseParameters()
{
    std::vector<std::pair<int, std::string>> parametersNames;
    this->tracer.enter("Parsing parameters");
    Token tempToken;
    this->accept({ TokenType::ParenthOpen });
    int type;
    tempToken = this->accept({ TokenType::ParenthClose, TokenType::Int, TokenType::Char, TokenType::String });
    while(tempToken.type != TokenType::ParenthClose)
    {
        switch(tempToken.type)
        {
        case TokenType::Int:
            type = 1;
            break;
        case TokenType::Char:
            type = 2;
            break;
        case TokenType::String:
            type = 3;
            break;
        default:
            break;
        }
        tempToken = this->accept({TokenType::Identifier});
        parametersNames.push_back(std::make_pair(type,tempToken.value));

        tempToken = this->accept({ TokenType::ParenthClose, TokenType::Comma });
        if(tempToken.type == TokenType::Comma)
            tempToken = this->accept({ TokenType::Int, TokenType::Char, TokenType::String });
    }
    this->tracer.leave();
    return parametersNames;
}

std::shared_ptr<ast::DeleteStatement> Parser::parseDeleteStatement()
{
    std::shared_ptr<ast::DeleteStatement> node = std::make_shared<ast::DeleteStatement>();
    this->tracer.enter("Parsing delete statement");
    this->accept({ TokenType::Delete });
    node->setName(this->accept({ TokenType::Identifier }).value);
    this->accept({ TokenType::Semicolon });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::StatementBlock> Parser::parseStatementBlock()
{
    std::shared_ptr<ast::StatementBlock> node = std::make_shared<ast::StatementBlock>();
    this->tracer.enter("Parsing statement block");
    this->accept({ TokenType::BracketOpen });
    Token tempToken;
    while (true)
    {
        if (!this->peek({
            TokenType::Switch,
            TokenType::While,
            TokenType::Return,
            TokenType::Int,
            TokenType::Char,
            TokenType::String,
            TokenType::Identifier,
            TokenType::Break,
            TokenType::Delete
        }))
        {
            break;
        }
        tempToken = this->getPeeked();
        if(tempToken.type == TokenType::String || tempToken.type == TokenType::Int || tempToken.type == TokenType::Char)
        {
            node->addInstruction(this->parseInitStatement());
        }
        else
        {
            switch (tempToken.type)
            {
                case TokenType::Switch:
                    node->addInstruction(this->parseSwitchStatement());
                    break;
                case TokenType::While:
                    node->addInstruction(this->parseWhileStatement());
                    break;
                case TokenType::Return:
                    node->addInstruction(this->parseReturnStatement());
                    break;
                case TokenType::Identifier:
                    node->addInstruction(this->parseAssignmentOrFunCall());
                    break;
                case TokenType::Break:
                    node->addInstruction(this->parseLoopJump());
                    break;
                case TokenType::Delete:
                    node->addInstruction(this->parseDeleteStatement());
                    break;
                default:
                    break;
            }
        }
    }
    this->accept({ TokenType::BracketClose });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::SwitchStatement> Parser::parseSwitchStatement()
{
    std::shared_ptr<ast::SwitchStatement> node = std::make_shared<ast::SwitchStatement>();
    this->tracer.enter("Parsing switch statement");
    this->accept({ TokenType::Switch });
    this->accept({ TokenType::ParenthOpen });
    node->setCondition(this->parseExpression());
    this->accept({ TokenType::ParenthClose });
    this->accept({ TokenType::BracketOpen });
    bool hasDefault = false;
    while(!this->peek({ TokenType::BracketClose }))
    {
        if(!hasDefault)
        {
            auto tempToken = this->accept({ TokenType::Default, TokenType::If });
            if(tempToken.type == TokenType::Default)
            {
                node->setDefaultBlock(this->parseSwitchBlock());
                hasDefault = true;
                continue;
            }
        }
        else
        {
            this->accept({TokenType::If});
        }
        std::shared_ptr<ast::Expression> Case = std::move(this->parseExpression());
        node->addCaseBlock(Case, this->parseSwitchBlock());
    }
    this->accept({ TokenType::BracketClose });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::StatementBlock> Parser::parseSwitchBlock()
{
    std::shared_ptr<ast::StatementBlock> node = std::make_shared<ast::StatementBlock>();
    this->tracer.enter("Parsing case statement block");
    this->accept({ TokenType::Colon });
    Token tempToken;
    bool b = true;
    while (b)
    {
        if (!this->peek({
            TokenType::Switch,
            TokenType::While,
            TokenType::Return,
            TokenType::Int,
            TokenType::Char,
            TokenType::String,
            TokenType::Identifier,
            TokenType::Delete
        }))
        {
            break;
        }
        tempToken = this->getPeeked();
        if(tempToken.type == TokenType::String || tempToken.type == TokenType::Int || tempToken.type == TokenType::Char)
        {
            node->addInstruction(this->parseInitStatement());
        }
        else
        {
            switch (tempToken.type)
            {
                case TokenType::Switch:
                    node->addInstruction(this->parseSwitchStatement());
                    break;
                case TokenType::While:
                    node->addInstruction(this->parseWhileStatement());
                    break;
                case TokenType::Return:
                    node->addInstruction(this->parseReturnStatement());
                    b = false;
                    break;
                case TokenType::Identifier:
                    node->addInstruction(this->parseAssignmentOrFunCall());
                    break;
                case TokenType::Delete:
                    node->addInstruction(this->parseDeleteStatement());
                    break;
                default:
                    break;
            }
        }
    }
    if(b)
    {
        this->accept({ TokenType::Break });
        this->accept({ TokenType::Semicolon });
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::WhileStatement> Parser::parseWhileStatement()
{
    std::shared_ptr<ast::WhileStatement> node = std::make_shared<ast::WhileStatement>();
    this->tracer.enter("Parsing while statement");
    this->accept({ TokenType::While });
    this->accept({ TokenType::ParenthOpen });
    node->setCondition(this->parseCondition());
    this->accept({ TokenType::ParenthClose });
    node->setBlock(this->parseStatementBlock());
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::ReturnStatement> Parser::parseReturnStatement()
{
    std::shared_ptr<ast::ReturnStatement> node = std::make_shared<ast::ReturnStatement>();
    this->tracer.enter("Parsing return statement");
    this->accept({ TokenType::Return });
    if(!this->peek({ TokenType::Semicolon }))
        node->setValue(this->parseAssignable());
    this->accept({ TokenType::Semicolon });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::VarDeclaration> Parser::parseInitStatement(int type, string name)
{
    std::shared_ptr<ast::VarDeclaration> node = std::make_shared<ast::VarDeclaration>();
    this->tracer.enter("Parsing init statement");
    Token tempToken;
    if(type==0)
    {
        tempToken = this->accept({ TokenType::Int, TokenType::Char, TokenType::String });
        switch(tempToken.type)
        {
        case TokenType::Int:
            type = 1;
            break;
        case TokenType::Char:
            type = 2;
            break;
        case TokenType::String:
            type = 3;
            break;
        default:
            break;
        }
    }
    node->setVarType(type);
    if(name=="")
    {
        tempToken = this->accept({ TokenType::Identifier });
        name = tempToken.value;
    }
    node->setName(name);
    if (this->peek({ TokenType::Assignment }))
    {
        this->accept({ TokenType::Assignment });
        node->setValue(this->parseAssignable());
    }
    this->accept({ TokenType::Semicolon });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::VarDeclaration> Parser::parseClassInitStatement(string type)
{
    std::shared_ptr<ast::VarDeclaration> node = std::make_shared<ast::VarDeclaration>();
    this->tracer.enter("Parsing init statement");
    Token tempToken;
    if(type=="")
    {
        tempToken = this->accept({ TokenType::Identifier });
        type = tempToken.value;
    }
    node->setClassType(type);
    tempToken = this->accept({ TokenType::Identifier });
    node->setName(tempToken.value);
    if (this->peek({ TokenType::Assignment }))
    {
        this->accept({ TokenType::Assignment });
        node->setValue(this->parseAssignable());
    }
    this->accept({ TokenType::Semicolon });
    this->tracer.leave();
    return node;
}

ast::NodePtr Parser::parseAssignmentOrFunCall()
{
    ast::NodePtr node;
    this->tracer.enter("Parsing assignment or function call or class init");
    auto tempToken = this->accept({ TokenType::Identifier });
    std::string className = "";
    if(this->peek({ TokenType::Identifier }))  //class declaration
    {
        node = this->parseClassInitStatement(tempToken.value);
        this->tracer.leave();
        return node;
    }
    if(this->peek({ TokenType::Dot }))  //part of class
    {
        this->accept({ TokenType::Dot });
        className = tempToken.value;
        tempToken = this->accept({ TokenType::Identifier });
    }
    node = this->parseFunCall(tempToken.value, className);
    if (node == nullptr)
    {
        std::shared_ptr<ast::Assignment> assignmentNode = std::make_shared<ast::Assignment>();
        assignmentNode->setVariable(this->parseVariable(tempToken, className));
        this->accept({ TokenType::Assignment });
        assignmentNode->setValue(this->parseAssignable());
        node = assignmentNode;
    }
    this->accept({ TokenType::Semicolon });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::LoopJump> Parser::parseLoopJump()
{
    std::shared_ptr<ast::LoopJump> node = std::make_shared<ast::LoopJump>();
    this->tracer.enter("Parsing loop jump");
    this->accept({ TokenType::Break });
    this->accept({ TokenType::Semicolon });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::NewClass> Parser::parseNewClass()
{
    std::shared_ptr<ast::NewClass> node = std::make_shared<ast::NewClass>();
    this->tracer.enter("Parsing new class call");
    auto tempToken = this->accept({ TokenType::Identifier });
    node->setName(tempToken.value);
    this->accept({ TokenType::ParenthOpen });
    if (this->peek({ TokenType::ParenthClose }))
    {
        this->accept({ TokenType::ParenthClose });
        this->tracer.leave("  + new class call");
        return node;
    }
    do
    {
        node->addArgument(this->parseAssignable());
        tempToken = this->accept({ TokenType::ParenthClose, TokenType::Comma });
    } while(tempToken.type == TokenType::Comma);
    this->tracer.leave("  + new class call");
    return node;
}

std::shared_ptr<ast::Assignable> Parser::parseAssignable()
{
    std::shared_ptr<ast::Assignable> node;
    this->tracer.enter("Parsing assignable");
    if(this->peek({ TokenType::New }))
    {
        this->accept({ TokenType::New });
        if(this->peek({ TokenType::Identifier }))   //new class
        {
            node = this->parseNewClass();
        }
        else    //new var
        {
            int type;
            switch(this->accept({ TokenType::Int, TokenType::Char, TokenType::String }).type)
            {
            case TokenType::Int:
                type = 1;
                break;
            case TokenType::Char:
                type = 2;
                break;
            case TokenType::String:
                type = 3;
                break;
            default:
                type = 0;
                break;
            }
            bool isArray = false;
            std::shared_ptr<ast::Expression> capacity = nullptr;
            if(this->peek({ TokenType::SquareBracketOpen }))
            {
                this->accept({ TokenType::SquareBracketOpen });
                capacity = this->parseExpression();
                this->accept({ TokenType::SquareBracketClose });
                isArray = true;
            }
            node = std::make_shared<ast::NewVariable>(type, isArray, capacity);
        }

    }
    else if(this->peek({ TokenType::StringLiteral }))
    {
        node = std::make_shared<ast::StringLiteral>(this->accept({ TokenType::StringLiteral }).value);
    }
    else if (this->peek({ TokenType::Identifier }))
    {
        auto tempToken = this->accept({ TokenType::Identifier });
        string className = "";
        if(this->peek({ TokenType::Dot }))
        {
            className = tempToken.value;
            this->accept({ TokenType::Dot });
            auto tempToken = this->accept({ TokenType::Identifier });
        }
        node = this->parseFunCall(tempToken.value, className);
        if (!node)
        {
            node = this->parseExpression(tempToken, className);
        }
    }
    else
    {
        node = this->parseExpression();
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::Call> Parser::parseFunCall(const std::string& identifier, const std::string& className)
{
    std::shared_ptr<ast::Call> node = std::make_shared<ast::Call>();
    this->tracer.enter("Parsing function call");
    Token tempToken;
    if (!this->peek({ TokenType::ParenthOpen }))
    {
        this->tracer.leave("  - not a function call");
        return nullptr;
    }
    node->setName(identifier);
    node->setClassName(className);
    this->accept({ TokenType::ParenthOpen });
    if (this->peek({ TokenType::ParenthClose }))
    {
        this->accept({ TokenType::ParenthClose });
        this->tracer.leave("  + function call");
        return node;
    }
    do
    {
        node->addArgument(this->parseAssignable());
        tempToken = this->accept({ TokenType::ParenthClose, TokenType::Comma });
    } while(tempToken.type == TokenType::Comma);
    this->tracer.leave("  + function call");
    return node;
}

std::shared_ptr<ast::Variable> Parser::parseVariable(const Token& identifierToken, const std::string& className)
{
    std::shared_ptr<ast::Variable> node = std::make_shared<ast::Variable>();
    this->tracer.enter("Parsing variable");
    if (identifierToken.type != TokenType::Identifier)
    {
        auto tempToken = this->accept({ TokenType::Identifier });
        if(className == "" && this->peek({ TokenType::Dot }))
        {
            this->accept({ TokenType::Dot });
            node->setClassName(tempToken.value);
            tempToken = this->accept({ TokenType::Identifier });
        }
        node->setName(tempToken.value);
    }
    else
    {
        Token tempToken = identifierToken;
        if(className == "" && this->peek({ TokenType::Dot }))
        {
            this->accept({ TokenType::Dot });
            node->setClassName(tempToken.value);
            tempToken = this->accept({ TokenType::Identifier });
        }
        node->setClassName(className);
        node->setName(tempToken.value);
    }
    if (this->peek({ TokenType::SquareBracketOpen }))
    {
        this->accept({ TokenType::SquareBracketOpen });
        node->setIndexArg(this->parseAssignable());
        this->accept({ TokenType::SquareBracketClose });
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::NumberLiteral> Parser::parseLiteral()
{
    std::shared_ptr<ast::NumberLiteral> node = std::make_shared<ast::NumberLiteral>();
    this->tracer.enter("Parsing number literal");
    bool negative = false;
    if (this->peek({ TokenType::Minus }))
    {
        this->accept({ TokenType::Minus });
        negative = true;
    }
    auto tempToken = this->accept({ TokenType::NumberLiteral });
    if (negative)
    {
        node->setValue(-tempToken.valueInt);
    }
    else
    {
        node->setValue(tempToken.valueInt);
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::Expression> Parser::parseExpression(const Token& firstToken, const std::string& className)
{
    std::shared_ptr<ast::Expression> node = std::make_shared<ast::Expression>();
    this->tracer.enter("Parsing expression");
    node->addOperand(this->parseTerm(firstToken, className));
    if(this->peek({ TokenType::Plus, TokenType::Minus }))
    {
        auto tempToken = this->accept({ TokenType::Plus, TokenType::Minus });
        node->addOperator(tempToken.type);
        node->addOperand(this->parseExpression());
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::Expression> Parser::parseTerm(const Token& firstToken, const std::string& className)
{
    std::shared_ptr<ast::Expression> node = std::make_shared<ast::Expression>();
    this->tracer.enter("Parsing multiplicative expression");
    node->addOperand(this->parseFactor(firstToken, className));
    if(this->peek({ TokenType::Multiply, TokenType::Divide }))
    {
        auto tempToken = this->accept({ TokenType::Multiply, TokenType::Divide });
        node->addOperator(tempToken.type);
        node->addOperand(this->parseTerm());
    }
    this->tracer.leave();
    return node;
}

ast::NodePtr Parser::parseFactor(const Token& firstToken, const std::string& className)
{
    this->tracer.enter("Parsing primary expression");
    this->tracer.info(std::string("First Token type = ").append(tokenUtils::getTokenTypeName(firstToken.type)));
    if (firstToken.type != TokenType::Undefined)
    {
        auto node = this->parseVariable(firstToken, className);
        this->tracer.leave();
        return node;
    }
    if (this->peek({ TokenType::ParenthOpen }))
    {
        this->accept({ TokenType::ParenthOpen });
        auto node = this->parseExpression();
        this->accept({ TokenType::ParenthClose });
        this->tracer.leave();
        return node;
    }
    if (this->peek({ TokenType::Identifier }))
    {
        auto node = this->parseVariable();
        this->tracer.leave();
        return node;
    }
    auto node = this->parseLiteral();
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::Condition> Parser::parseCondition()
{
    std::shared_ptr<ast::Condition> node = std::make_shared<ast::Condition>();
    this->tracer.enter("Parsing condition");
    node->setOp1(this->parseExpression());
    auto tempToken = this->accept({ TokenType::Equality, TokenType::Inequality, TokenType::Less,
                                  TokenType::Greater, TokenType::LessOrEqual, TokenType::GreaterOrEqual });
    node->setOperator(tempToken.type);
    node->setOp2(this->parseExpression());
    this->tracer.leave();
    return node;
}
