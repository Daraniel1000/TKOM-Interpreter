#ifndef TKOM_TOKEN
#define TKOM_TOKEN

#include<string>
#include"Token_Types.h"
using TokenType = tokenUtils::TokenType;

struct Token
{
	Token() = default;
	Token(const TokenType& type) : type(type) {}

	TokenType type = TokenType::Invalid;
	std::string value = "";
	unsigned int line = 0;
	unsigned int pos = 0;
	std::streampos lineStart;
};

#endif
