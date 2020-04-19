#ifndef TKOM_TOKEN_TYPES
#define TKOM_TOKEN_TYPES

#include<unordered_map>
#include<string>
using namespace std;

namespace tokenUtils
{
	enum class TokenType : unsigned int
	{
		Main				= 0,
		Class				= 1,
		ParenthOpen			= 2,
		ParenthClose		= 3,
		BracketOpen			= 4,
		BracketClose		= 5,
		SquareBracketOpen	= 6,
		SquareBracketClose	= 7,
		Comma				= 8,
		Semicolon			= 9,
		Colon				= 10,
		If					= 11,
		While				= 12,
		Else				= 13,
		Return				= 14,
		Break				= 15,
		Switch				= 16,
		Default				= 17,
		Void				= 18,
		Int					= 19,
		Char				= 20,
		Negation			= 21,
		Assignment			= 22,
		Public				= 23,
		Private				= 24,
		Equality			= 25,
		Inequality			= 26,
		Less				= 27,
		Greater				= 28,
		LessOrEqual			= 29,
		GreaterOrEqual		= 30,
		Plus				= 31,
		Minus				= 32,
		Multiply			= 33,
		Divide				= 34,
		Dot					= 35,
		Identifier			= 36,
		NumberLiteral		= 37,
		New					= 38,
		Delete				= 39,
		Constructor			= 40,
		Destructor			= 41,
		And					= 42,
		Or					= 43,
		Invalid 			= 44,
		EndOfFile			= 45,
		Undefined			= 46
	};

	const unordered_map<unsigned int, string> tokenTypeNames =
	{
		{ 0,  "Main" },
		{ 1,  "Class" },
		{ 2,  "ParenthOpen" },
		{ 3,  "ParenthClose" },
		{ 4,  "BracketOpen" },
		{ 5,  "BracketClose" },
		{ 6,  "SquareBracketOpen" },
		{ 7,  "SquareBracketClose" },
		{ 8,  "Comma" },
		{ 9,  "Semicolon" },
		{ 10,  "Colon" },
		{ 11, "If" },
		{ 12, "While" },
		{ 13, "Else" },
		{ 14, "Return" },
		{ 15, "Break" },
		{ 16, "Switch" },
		{ 17, "Default" },
		{ 18, "Void" },
		{ 19, "Int" },
		{ 20, "Char" },
		{ 21, "Negation" },
		{ 22, "Assignment" },
		{ 23, "Public" },
		{ 24, "Private" },
		{ 25, "Equality" },
		{ 26, "Inequality" },
		{ 27, "Less" },
		{ 28, "Greater" },
		{ 29, "LessOrEqual" },
		{ 30, "GreaterOrEqual" },
		{ 31, "Plus" },
		{ 32, "Minus" },
		{ 33, "Multiply" },
		{ 34, "Divide" },
		{ 35, "Dot" },
		{ 36, "Identifier" },
		{ 37, "NumberLiteral" },
		{ 38, "New" },
		{ 39, "Delete" },
		{ 40, "Constructor" },
		{ 41, "Destructor" },
		{ 42, "And" },
		{ 43, "Or" },
		{ 44, "Invalid" },
		{ 45, "EndOfFile" },
		{ 46, "Undefined" }
	};

	inline const string getTokenTypeName(const TokenType& tokenType)
	{
		return tokenTypeNames.at(static_cast<unsigned int>(tokenType));
	};
}

#endif
