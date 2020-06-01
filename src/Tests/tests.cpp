#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../Lekser/Lekser.h"
#include "../parser/Parser.h"
#include<sstream>
using namespace std;
using TokenTypes = tokenUtils::TokenTypes;

TEST_CASE( "[lekser] eof is caught", "[eof]" ) {
    istringstream s("");
    Lekser l(s);
    REQUIRE( l.nextToken().type == TokenTypes::EndOfFile );
}

TEST_CASE( "[lekser] comment is ignored", "[comment]" ) {
    istringstream s("123//main111\n123");
    Lekser l(s);
    REQUIRE( l.nextToken().type == TokenTypes::NumberLiteral );
    REQUIRE( l.nextToken().type == TokenTypes::NumberLiteral );
    REQUIRE( l.nextToken().type == TokenTypes::EndOfFile );
}

TEST_CASE( "[lekser] keyword token types are recognized", "[keywords]" ) {
    istringstream s("class main constructor destructor new delete switch default if while return break public private void int char string");
    Lekser l(s);
    REQUIRE( l.nextToken().type == TokenTypes::Class );
    REQUIRE( l.nextToken().type == TokenTypes::Main );
    REQUIRE( l.nextToken().type == TokenTypes::Constructor );
    REQUIRE( l.nextToken().type == TokenTypes::Destructor );
    REQUIRE( l.nextToken().type == TokenTypes::New );
    REQUIRE( l.nextToken().type == TokenTypes::Delete );
    REQUIRE( l.nextToken().type == TokenTypes::Switch );
    REQUIRE( l.nextToken().type == TokenTypes::Default );
    REQUIRE( l.nextToken().type == TokenTypes::If );
    REQUIRE( l.nextToken().type == TokenTypes::While );
    REQUIRE( l.nextToken().type == TokenTypes::Return );
    REQUIRE( l.nextToken().type == TokenTypes::Break );
    REQUIRE( l.nextToken().type == TokenTypes::Public );
    REQUIRE( l.nextToken().type == TokenTypes::Private );
    REQUIRE( l.nextToken().value == "void" );
    REQUIRE( l.nextToken().value == "int" );
    REQUIRE( l.nextToken().value == "char" );
    REQUIRE( l.nextToken().value == "string" );
    REQUIRE( l.nextToken().type == TokenTypes::EndOfFile );
}

TEST_CASE( "[lekser] identifiers are recognized", "[identifiers]" ) {
    istringstream s("asd Class words");
    Lekser l(s);
    auto t = l.nextToken();
    REQUIRE( t.type == TokenTypes::Identifier );
    REQUIRE( t.value == "asd");
    REQUIRE( l.nextToken().type == TokenTypes::Class );
    t = l.nextToken();
    REQUIRE( t.type == TokenTypes::Identifier );
    REQUIRE( t.value == "words");
}

TEST_CASE( "[lekser] literals are recognized", "[literals]" ) {
    istringstream s("1234 \"asdfASDF1234\"12345 ");
    Lekser l(s);
    auto t = l.nextToken();
    REQUIRE( t.type == TokenTypes::NumberLiteral );
    REQUIRE( t.valueInt == 1234 );
    t = l.nextToken();
    REQUIRE( t.type == TokenTypes::StringLiteral );
    REQUIRE( t.value == "asdfASDF1234" );
    t = l.nextToken();
    REQUIRE( t.type == TokenTypes::NumberLiteral );
    REQUIRE( t.valueInt == 12345 );
    REQUIRE( l.nextToken().type == TokenTypes::EndOfFile );
}

TEST_CASE( "[lekser] signs are recognized", "[signs]" ) {
    istringstream s("===<=<>=>!=!&&&|||(){}[],;+-*/.:");
    Lekser l(s);
    REQUIRE( l.nextToken().type == TokenTypes::Equality );
    REQUIRE( l.nextToken().type == TokenTypes::Assignment );
    REQUIRE( l.nextToken().type == TokenTypes::LessOrEqual );
    REQUIRE( l.nextToken().type == TokenTypes::Less );
    REQUIRE( l.nextToken().type == TokenTypes::GreaterOrEqual );
    REQUIRE( l.nextToken().type == TokenTypes::Greater );
    REQUIRE( l.nextToken().type == TokenTypes::Inequality );
    REQUIRE( l.nextToken().type == TokenTypes::Negation );
    REQUIRE( l.nextToken().type == TokenTypes::And );
    REQUIRE( l.nextToken().type == TokenTypes::Invalid );
    REQUIRE( l.nextToken().type == TokenTypes::Or );
    REQUIRE( l.nextToken().type == TokenTypes::Invalid );
    REQUIRE( l.nextToken().type == TokenTypes::ParenthOpen );
	REQUIRE( l.nextToken().type == TokenTypes::ParenthClose );
	REQUIRE( l.nextToken().type == TokenTypes::BracketOpen );
	REQUIRE( l.nextToken().type == TokenTypes::BracketClose );
	REQUIRE( l.nextToken().type == TokenTypes::SquareBracketOpen );
	REQUIRE( l.nextToken().type == TokenTypes::SquareBracketClose );
	REQUIRE( l.nextToken().type == TokenTypes::Comma );
	REQUIRE( l.nextToken().type == TokenTypes::Semicolon );
	REQUIRE( l.nextToken().type == TokenTypes::Plus );
	REQUIRE( l.nextToken().type == TokenTypes::Minus );
	REQUIRE( l.nextToken().type == TokenTypes::Multiply );
	REQUIRE( l.nextToken().type == TokenTypes::Divide );
	REQUIRE( l.nextToken().type == TokenTypes::Dot );
	REQUIRE( l.nextToken().type == TokenTypes::Colon );
    REQUIRE( l.nextToken().type == TokenTypes::EndOfFile );
}

TEST_CASE( "[parser] program structure", "[structure]")
{
    istringstream s("class name{} int fun(){} int var; main{}");
    Lekser l(s);
    Parser p(l, false);
    auto tree = p.parse();
    REQUIRE( tree->functions.size() == 1 );
    REQUIRE( tree->classes.size() == 1 );
    REQUIRE( tree->variables.size() == 1 );
    REQUIRE( tree->functions.front()->name == "fun");
    REQUIRE( tree->functions.front()->returnType == "int");
}


TEST_CASE( "[parser] expression", "[structure]")
{
    istringstream s("main{while(1 * 3 + 2 <= 1 * 3 + 2){break;}}");
    Lekser l(s);
    Parser p(l, false);
    auto tree = p.parse();
    return;
}
