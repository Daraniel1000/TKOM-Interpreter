#ifndef TKOM_TOKEN
#define TKOM_TOKEN

#include<string>
#include"Token_Types.h"
using TokenTypes = tokenUtils::TokenTypes;

struct Token
{
	Token() = default;
	Token(const TokenTypes& type) : type(type) {}

	TokenTypes type = TokenTypes::Invalid;
	std::string value = "";
	int valueInt = 0;
	unsigned int line = 0;
	unsigned int pos = 0;
	std::streampos lineStart;
};

#endif
