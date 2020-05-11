#ifndef TKOM_PARSER
#define TKOM_PARSER

#include <initializer_list>
#include "../Token/Token.h"
#include "../Token/Token_Types.h"
#include "../tracer/Tracer.hpp"
#include "../ast/AST.hpp"

using TokenType = tokenUtils::TokenTypes;

class Lekser;

class Parser
{
public:
    Parser(Lekser& lexer);

    std::shared_ptr<ast::Program> parse();

private:
    Tracer tracer = Tracer(true);

    Lekser& lexer;
    Token previousToken;

    bool isAcceptable(const Token& token, const std::initializer_list<TokenType>& acceptable) const;
    Token accept(const std::initializer_list<TokenType>& acceptable);
    bool peek(const std::initializer_list<TokenType>& acceptable);
    Token getPeeked();
    void peekFail();
    const std::string makeErrorMarker(const unsigned int& pos);

    bool hasBufferedToken() const;
    void resetPreviousToken();

    // Decomposition procedures
    std::shared_ptr<ast::MainDefinition> parseMain();
    std::shared_ptr<ast::ClassDefinition> parseClass();
    std::shared_ptr<ast::FunDefinition> parseFunction(int type, string name = "");
    std::vector<std::pair<int, std::string>> parseParameters();
    std::shared_ptr<ast::DeleteStatement> parseDeleteStatement();
    std::shared_ptr<ast::StatementBlock> parseStatementBlock();

    std::shared_ptr<ast::SwitchStatement> parseSwitchStatement();
    std::shared_ptr<ast::StatementBlock> parseSwitchBlock();
    std::shared_ptr<ast::WhileStatement> parseWhileStatement();
    std::shared_ptr<ast::ReturnStatement> parseReturnStatement();
    std::shared_ptr<ast::VarDeclaration> parseInitStatement(int type = 0, string name = "");
    std::shared_ptr<ast::VarDeclaration> parseClassInitStatement(string type = "");
    ast::NodePtr parseAssignmentOrFunCall();
    std::shared_ptr<ast::LoopJump> parseLoopJump();

    std::shared_ptr<ast::NewClass> parseNewClass();
    std::shared_ptr<ast::Assignable> parseAssignable();
    std::shared_ptr<ast::Call> parseFunCall(const std::string& identifier, const std::string& className = "");
    std::shared_ptr<ast::Variable> parseVariable(const Token& firstToken = Token(TokenType::Undefined), const std::string& className = "");
    std::shared_ptr<ast::NumberLiteral> parseLiteral();

    std::shared_ptr<ast::Expression> parseExpression(const Token& firstToken = Token(TokenType::Undefined), const std::string& className = "");
    std::shared_ptr<ast::Expression> parseTerm(const Token& firstToken = Token(TokenType::Undefined), const std::string& className = "");
    ast::NodePtr parseFactor(const Token& firstToken = Token(TokenType::Undefined), const std::string& className = "");

    std::shared_ptr<ast::Condition> parseCondition();
};

#endif
