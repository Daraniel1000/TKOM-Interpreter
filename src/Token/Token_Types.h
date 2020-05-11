#ifndef TKOM_TOKEN_TYPES
#define TKOM_TOKEN_TYPES

#include<unordered_map>
#include<string>
using namespace std;

namespace tokenUtils
{
	enum class TokenTypes : unsigned int
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
		String				= 21,
		Negation			= 22,
		Assignment			= 23,
		Public				= 24,
		Private				= 25,
		Equality			= 26,
		Inequality			= 27,
		Less				= 28,
		Greater				= 29,
		LessOrEqual			= 30,
		GreaterOrEqual		= 31,
		Plus				= 32,
		Minus				= 33,
		Multiply			= 34,
		Divide				= 35,
		Dot					= 36,
		Identifier			= 37,
		NumberLiteral		= 38,
		StringLiteral		= 39,
		New					= 40,
		Delete				= 41,
		Constructor			= 42,
		Destructor			= 43,
		And					= 44,
		Or					= 45,
		Comment             = 46,
		Invalid 			= 47,
		EndOfFile			= 48,
		Undefined			= 49
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
		{ 10, "Colon" },
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
		{ 21, "String" },
		{ 22, "Negation" },
		{ 23, "Assignment" },
		{ 24, "Public" },
		{ 25, "Private" },
		{ 26, "Equality" },
		{ 27, "Inequality" },
		{ 28, "Less" },
		{ 29, "Greater" },
		{ 30, "LessOrEqual" },
		{ 31, "GreaterOrEqual" },
		{ 32, "Plus" },
		{ 33, "Minus" },
		{ 34, "Multiply" },
		{ 35, "Divide" },
		{ 36, "Dot" },
		{ 37, "Identifier" },
		{ 38, "NumberLiteral" },
		{ 39, "StringLiteral" },
		{ 40, "New" },
		{ 41, "Delete" },
		{ 42, "Constructor" },
		{ 43, "Destructor" },
		{ 44, "And" },
		{ 45, "Or" },
		{ 46, "Comment" },
		{ 47, "Invalid" },
		{ 48, "EndOfFile" },
		{ 49, "Undefined" }
	};

	inline const string getTokenTypeName(const TokenTypes& tokenType)
	{
		return tokenTypeNames.at(static_cast<unsigned int>(tokenType));
	};
}

#endif
