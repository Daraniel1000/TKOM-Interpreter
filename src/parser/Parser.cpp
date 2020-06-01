#include "Parser.h"
#include "../Lekser/Lekser.h"
#include <iostream>
#include <limits>

Parser::Parser(Lekser& lexer, bool b): lexer(lexer), tracer(b)
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
        token = this->accept({TokenTypes::Class, TokenTypes::Main, TokenTypes::Identifier});
        switch(token.type)
        {
        case TokenTypes::Class:
            syntaxTree->addClass(this->parseClass());
            break;
        case TokenTypes::Main:
            syntaxTree->addMain(this->parseMain());
            token = this->accept({TokenTypes::EndOfFile});
            break;
        default:
            std::string type = token.value;
            token = this->accept({TokenTypes::Identifier});
            if(this->peek({TokenTypes::ParenthOpen}))
            {
                syntaxTree->addFunction(this->parseFunction(type, token.value));
            }
            else
            {
                syntaxTree->addVar(this->parseInitStatement(type, token.value));
            }
            break;
        }
    } while (token.type != TokenTypes::EndOfFile);
    this->tracer.leave("Trace ended...");

    return syntaxTree;
}

bool Parser::isAcceptable(const Token& token, const std::initializer_list<TokenTypes>& acceptable) const
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

Token Parser::accept(const std::initializer_list<TokenTypes>& acceptable)
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

bool Parser::peek(const std::initializer_list<TokenTypes>& acceptable)
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
    return this->previousToken.type != TokenTypes::Undefined;
}

void Parser::resetPreviousToken()
{
    this->previousToken.type = TokenTypes::Undefined;
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
    auto tempToken = this->accept({ TokenTypes::Identifier });
    node->setName(tempToken.value);
    this->accept({ TokenTypes::BracketOpen });
    tempToken = this->accept({ TokenTypes::BracketClose, TokenTypes::Identifier, TokenTypes::Constructor,
                             TokenTypes::Destructor, TokenTypes::Public, TokenTypes::Private });
    bool isPublic;
    while(tempToken.type != TokenTypes::BracketClose)
    {
        isPublic = true;
        if(tempToken.type == TokenTypes::Public || tempToken.type == TokenTypes::Private)
        {
            if(tempToken.type == TokenTypes::Private) isPublic = false;
            tempToken = this->accept({TokenTypes::Identifier, TokenTypes::Constructor, TokenTypes::Destructor});
        }
        switch(tempToken.type)
        {
        case TokenTypes::Constructor:
            node->addFunction(isPublic, this->parseFunction("void", "Constructor"));
            break;
        case TokenTypes::Destructor:
            node->addFunction(isPublic, this->parseFunction("void", "Destructor"));
            break;
        default:
            string type = tempToken.value;
            tempToken = this->accept({TokenTypes::Identifier});
            if(this->peek({TokenTypes::ParenthOpen}))
            {
                node->addFunction(isPublic, this->parseFunction(type, tempToken.value));
            }
            else
            {
                node->addVar(isPublic, this->parseInitStatement(type, tempToken.value));
            }
            break;
        }
        tempToken = this->accept({ TokenTypes::BracketClose, TokenTypes::Identifier, TokenTypes::Constructor,
                             TokenTypes::Destructor, TokenTypes::Public, TokenTypes::Private });
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::FunDefinition> Parser::parseFunction(string type, string name)
{
    std::shared_ptr<ast::FunDefinition> node = std::make_shared<ast::FunDefinition>();
    this->tracer.enter("Parsing function");
    if(name == "")
    {
        auto tempToken = this->accept({ TokenTypes::Identifier });
        name = tempToken.value;
    }
    node->setName(name);
    node->setReturnType(type);
    node->setParameters(this->parseParameters());
    node->setBlock(this->parseStatementBlock());
    this->tracer.leave();
    return node;
}

std::vector<std::pair<std::string, std::string>> Parser::parseParameters()
{
    std::vector<std::pair<std::string, std::string>> parametersNames;
    this->tracer.enter("Parsing parameters");
    Token tempToken;
    this->accept({ TokenTypes::ParenthOpen });
    std::string type;
    tempToken = this->accept({ TokenTypes::ParenthClose, TokenTypes::Identifier });
    while(tempToken.type != TokenTypes::ParenthClose)
    {
        type = tempToken.value;
        tempToken = this->accept({TokenTypes::Identifier});
        parametersNames.push_back(std::make_pair(type,tempToken.value));

        tempToken = this->accept({ TokenTypes::ParenthClose, TokenTypes::Comma });
        if(tempToken.type == TokenTypes::Comma)
            tempToken = this->accept({ TokenTypes::Identifier });
    }
    this->tracer.leave();
    return parametersNames;
}

std::shared_ptr<ast::DeleteStatement> Parser::parseDeleteStatement()
{
    std::shared_ptr<ast::DeleteStatement> node = std::make_shared<ast::DeleteStatement>();
    this->tracer.enter("Parsing delete statement");
    this->accept({ TokenTypes::Delete });
    node->setName(this->accept({ TokenTypes::Identifier }).value);
    this->accept({ TokenTypes::Semicolon });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::StatementBlock> Parser::parseStatementBlock()
{
    std::shared_ptr<ast::StatementBlock> node = std::make_shared<ast::StatementBlock>();
    this->tracer.enter("Parsing statement block");
    this->accept({ TokenTypes::BracketOpen });
    Token tempToken;
    while (true)
    {
        if (!this->peek({
            TokenTypes::Switch,
            TokenTypes::While,
            TokenTypes::Return,
            TokenTypes::Identifier,
            TokenTypes::Break,
            TokenTypes::Delete
        }))
        {
            break;
        }
        tempToken = this->getPeeked();
        if(tempToken.type == TokenTypes::Identifier && (tempToken.value == "int" || tempToken.value == "string" || tempToken.value == "char"))
        {
            node->addInstruction(this->parseInitStatement());
        }
        else
        {
            switch (tempToken.type)
            {
                case TokenTypes::Switch:
                    node->addInstruction(this->parseSwitchStatement());
                    break;
                case TokenTypes::While:
                    node->addInstruction(this->parseWhileStatement());
                    break;
                case TokenTypes::Return:
                    node->addInstruction(this->parseReturnStatement());
                    break;
                case TokenTypes::Identifier:
                    node->addInstruction(this->parseAssignmentOrFunCall());
                    break;
                case TokenTypes::Break:
                    node->addInstruction(this->parseLoopJump());
                    break;
                case TokenTypes::Delete:
                    node->addInstruction(this->parseDeleteStatement());
                    break;
                default:
                    break;
            }
        }
    }
    this->accept({ TokenTypes::BracketClose });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::SwitchStatement> Parser::parseSwitchStatement()
{
    std::shared_ptr<ast::SwitchStatement> node = std::make_shared<ast::SwitchStatement>();
    this->tracer.enter("Parsing switch statement");
    this->accept({ TokenTypes::Switch });
    this->accept({ TokenTypes::ParenthOpen });
    node->setCondition(this->parseExpression());
    this->accept({ TokenTypes::ParenthClose });
    this->accept({ TokenTypes::BracketOpen });
    bool hasDefault = false;
    while(!this->peek({ TokenTypes::BracketClose }))
    {
        if(!hasDefault)
        {
            auto tempToken = this->accept({ TokenTypes::Default, TokenTypes::If });
            if(tempToken.type == TokenTypes::Default)
            {
                node->setDefaultBlock(this->parseSwitchBlock());
                hasDefault = true;
                continue;
            }
        }
        else
        {
            this->accept({TokenTypes::If});
        }
        std::shared_ptr<ast::Expression> Case = std::move(this->parseExpression());
        node->addCaseBlock(Case, this->parseSwitchBlock());
    }
    this->accept({ TokenTypes::BracketClose });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::StatementBlock> Parser::parseSwitchBlock()
{
    std::shared_ptr<ast::StatementBlock> node = std::make_shared<ast::StatementBlock>();
    this->tracer.enter("Parsing case statement block");
    this->accept({ TokenTypes::Colon });
    Token tempToken;
    bool b = true;
    while (b)
    {
        if (!this->peek({
            TokenTypes::Switch,
            TokenTypes::While,
            TokenTypes::Return,
            TokenTypes::Identifier,
            TokenTypes::Delete
        }))
        {
            break;
        }
        tempToken = this->getPeeked();
        if(tempToken.type == TokenTypes::Identifier && (tempToken.value == "int" || tempToken.value == "string" || tempToken.value == "char"))
        {
            node->addInstruction(this->parseInitStatement());
        }
        else
        {
            switch (tempToken.type)
            {
                case TokenTypes::Switch:
                    node->addInstruction(this->parseSwitchStatement());
                    break;
                case TokenTypes::While:
                    node->addInstruction(this->parseWhileStatement());
                    break;
                case TokenTypes::Return:
                    node->addInstruction(this->parseReturnStatement());
                    b = false;
                    break;
                case TokenTypes::Identifier:
                    node->addInstruction(this->parseAssignmentOrFunCall());
                    break;
                case TokenTypes::Delete:
                    node->addInstruction(this->parseDeleteStatement());
                    break;
                default:
                    break;
            }
        }
    }
    if(b)
    {
        this->accept({ TokenTypes::Break });
        this->accept({ TokenTypes::Semicolon });
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::WhileStatement> Parser::parseWhileStatement()
{
    std::shared_ptr<ast::WhileStatement> node = std::make_shared<ast::WhileStatement>();
    this->tracer.enter("Parsing while statement");
    this->accept({ TokenTypes::While });
    this->accept({ TokenTypes::ParenthOpen });
    node->setCondition(this->parseCondition());
    this->accept({ TokenTypes::ParenthClose });
    node->setBlock(this->parseStatementBlock());
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::ReturnStatement> Parser::parseReturnStatement()
{
    std::shared_ptr<ast::ReturnStatement> node = std::make_shared<ast::ReturnStatement>();
    this->tracer.enter("Parsing return statement");
    this->accept({ TokenTypes::Return });
    if(!this->peek({ TokenTypes::Semicolon }))
        node->setValue(this->parseAssignable());
    this->accept({ TokenTypes::Semicolon });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::VarDeclaration> Parser::parseInitStatement(string type, string name)
{
    std::shared_ptr<ast::VarDeclaration> node = std::make_shared<ast::VarDeclaration>();
    this->tracer.enter("Parsing init statement");
    Token tempToken;
    if(type=="")
    {
        tempToken = this->accept({ TokenTypes::Identifier });
        type = tempToken.value;
    }
    node->setVarType(type);
    if(name=="")
    {
        tempToken = this->accept({ TokenTypes::Identifier });
        name = tempToken.value;
    }
    node->setName(name);
    if (this->peek({ TokenTypes::Assignment }))
    {
        this->accept({ TokenTypes::Assignment });
        node->setValue(this->parseAssignable());
    }
    this->accept({ TokenTypes::Semicolon });
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
        tempToken = this->accept({ TokenTypes::Identifier });
        type = tempToken.value;
    }
    node->setVarType(type);
    tempToken = this->accept({ TokenTypes::Identifier });
    node->setName(tempToken.value);
    if (this->peek({ TokenTypes::Assignment }))
    {
        this->accept({ TokenTypes::Assignment });
        node->setValue(this->parseAssignable());
    }
    this->accept({ TokenTypes::Semicolon });
    this->tracer.leave();
    return node;
}

ast::NodePtr Parser::parseAssignmentOrFunCall()
{
    ast::NodePtr node;
    this->tracer.enter("Parsing assignment or function call or class init");
    auto tempToken = this->accept({ TokenTypes::Identifier });
    std::string className = "";
    if(this->peek({ TokenTypes::Identifier }))  //class declaration
    {
        node = this->parseClassInitStatement(tempToken.value);
        this->tracer.leave();
        return node;
    }
    if(this->peek({ TokenTypes::Dot }))  //part of class
    {
        this->accept({ TokenTypes::Dot });
        className = tempToken.value;
        tempToken = this->accept({ TokenTypes::Identifier });
    }
    node = this->parseFunCall(tempToken.value, className);
    if (node == nullptr)
    {
        std::shared_ptr<ast::Assignment> assignmentNode = std::make_shared<ast::Assignment>();
        assignmentNode->setVariable(this->parseVariable(tempToken, className));
        this->accept({ TokenTypes::Assignment });
        assignmentNode->setValue(this->parseAssignable());
        node = assignmentNode;
    }
    this->accept({ TokenTypes::Semicolon });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::LoopJump> Parser::parseLoopJump()
{
    std::shared_ptr<ast::LoopJump> node = std::make_shared<ast::LoopJump>();
    this->tracer.enter("Parsing loop jump");
    this->accept({ TokenTypes::Break });
    this->accept({ TokenTypes::Semicolon });
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::NewClass> Parser::parseNewClass()
{
    std::shared_ptr<ast::NewClass> node = std::make_shared<ast::NewClass>();
    this->tracer.enter("Parsing new class call");
    auto tempToken = this->accept({ TokenTypes::Identifier });
    node->setName(tempToken.value);
    this->accept({ TokenTypes::ParenthOpen });
    if (this->peek({ TokenTypes::ParenthClose }))
    {
        this->accept({ TokenTypes::ParenthClose });
        this->tracer.leave("  + new class call");
        return node;
    }
    do
    {
        node->addArgument(this->parseAssignable());
        tempToken = this->accept({ TokenTypes::ParenthClose, TokenTypes::Comma });
    } while(tempToken.type == TokenTypes::Comma);
    this->tracer.leave("  + new class call");
    return node;
}

std::shared_ptr<ast::Assignable> Parser::parseAssignable()
{
    std::shared_ptr<ast::Assignable> node;
    this->tracer.enter("Parsing assignable");
    if(this->peek({ TokenTypes::New }))
    {
        this->accept({ TokenTypes::New });
        this->peek({});
        if(this->getPeeked().value != "string" && this->getPeeked().value != "int" && this->getPeeked().value != "char")   //new class
        {
            node = this->parseNewClass();
        }
        else    //new var
        {
            string type = this->accept({ TokenTypes::Identifier }).value;
            bool isArray = false;
            std::shared_ptr<ast::Expression> capacity = nullptr;
            if(this->peek({ TokenTypes::SquareBracketOpen }))
            {
                this->accept({ TokenTypes::SquareBracketOpen });
                capacity = this->parseExpression();
                this->accept({ TokenTypes::SquareBracketClose });
                isArray = true;
            }
            node = std::make_shared<ast::NewVariable>(type, isArray, capacity);
        }

    }
    else if(this->peek({ TokenTypes::StringLiteral }))
    {
        node = std::make_shared<ast::StringLiteral>(this->accept({ TokenTypes::StringLiteral }).value);
    }
    else if (this->peek({ TokenTypes::Identifier }))
    {
        auto tempToken = this->accept({ TokenTypes::Identifier });
        string className = "";
        if(this->peek({ TokenTypes::Dot }))
        {
            className = tempToken.value;
            this->accept({ TokenTypes::Dot });
            tempToken = this->accept({ TokenTypes::Identifier });
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
    if (!this->peek({ TokenTypes::ParenthOpen }))
    {
        this->tracer.leave("  - not a function call");
        return nullptr;
    }
    node->setName(identifier);
    node->setClassName(className);
    this->accept({ TokenTypes::ParenthOpen });
    if (this->peek({ TokenTypes::ParenthClose }))
    {
        this->accept({ TokenTypes::ParenthClose });
        this->tracer.leave("  + function call");
        return node;
    }
    do
    {
        node->addArgument(this->parseAssignable());
        tempToken = this->accept({ TokenTypes::ParenthClose, TokenTypes::Comma });
    } while(tempToken.type == TokenTypes::Comma);
    this->tracer.leave("  + function call");
    return node;
}

std::shared_ptr<ast::Variable> Parser::parseVariable(const Token& identifierToken, const std::string& className)
{
    std::shared_ptr<ast::Variable> node = std::make_shared<ast::Variable>();
    this->tracer.enter("Parsing variable");
    if (identifierToken.type != TokenTypes::Identifier)
    {
        auto tempToken = this->accept({ TokenTypes::Identifier });
        if(className == "" && this->peek({ TokenTypes::Dot }))
        {
            this->accept({ TokenTypes::Dot });
            node->setClassName(tempToken.value);
            tempToken = this->accept({ TokenTypes::Identifier });
        }
        node->setName(tempToken.value);
    }
    else
    {
        Token tempToken = identifierToken;
        if(className == "" && this->peek({ TokenTypes::Dot }))
        {
            this->accept({ TokenTypes::Dot });
            node->setClassName(tempToken.value);
            tempToken = this->accept({ TokenTypes::Identifier });
        }
        node->setClassName(className);
        node->setName(tempToken.value);
    }
    if (this->peek({ TokenTypes::SquareBracketOpen }))
    {
        this->accept({ TokenTypes::SquareBracketOpen });
        node->setIndexArg(this->parseAssignable());
        this->accept({ TokenTypes::SquareBracketClose });
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::NumberLiteral> Parser::parseLiteral()
{
    std::shared_ptr<ast::NumberLiteral> node = std::make_shared<ast::NumberLiteral>();
    this->tracer.enter("Parsing number literal");
    bool negative = false;
    if (this->peek({ TokenTypes::Minus }))
    {
        this->accept({ TokenTypes::Minus });
        negative = true;
    }
    auto tempToken = this->accept({ TokenTypes::NumberLiteral });
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
    if(this->peek({ TokenTypes::Plus, TokenTypes::Minus }))
    {
        auto tempToken = this->accept({ TokenTypes::Plus, TokenTypes::Minus });
        node->addOperator(tempToken.type);
        node->addOperand(this->parseExpression());
    }
    this->tracer.leave();
    return node;
}

std::shared_ptr<ast::Expression> Parser::parseTerm(const Token& firstToken, const std::string& className)
{
    std::shared_ptr<ast::Expression> node = std::make_shared<ast::Expression>();
    this->tracer.enter("Parsing term");
    node->addOperand(this->parseFactor(firstToken, className));
    if(this->peek({ TokenTypes::Multiply, TokenTypes::Divide }))
    {
        auto tempToken = this->accept({ TokenTypes::Multiply, TokenTypes::Divide });
        node->addOperator(tempToken.type);
        node->addOperand(this->parseTerm());
    }
    this->tracer.leave();
    return node;
}

ast::NodePtr Parser::parseFactor(const Token& firstToken, const std::string& className)
{
    this->tracer.enter("Parsing factor");
    this->tracer.info(std::string("First Token type = ").append(tokenUtils::getTokenTypeName(firstToken.type)));
    if (firstToken.type != TokenTypes::Undefined)
    {
        auto node = this->parseVariable(firstToken, className);
        this->tracer.leave();
        return node;
    }
    if (this->peek({ TokenTypes::ParenthOpen }))
    {
        this->accept({ TokenTypes::ParenthOpen });
        auto node = this->parseExpression();
        this->accept({ TokenTypes::ParenthClose });
        this->tracer.leave();
        return node;
    }
    if (this->peek({ TokenTypes::Identifier }))
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
    auto tempToken = this->accept({ TokenTypes::Equality, TokenTypes::Inequality, TokenTypes::Less,
                                  TokenTypes::Greater, TokenTypes::LessOrEqual, TokenTypes::GreaterOrEqual });
    node->setOperator(tempToken.type);
    node->setOp2(this->parseExpression());
    this->tracer.leave();
    return node;
}
