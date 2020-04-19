#ifndef TKOM_KEYWORDS
#define TKOM_KEYWORDS

#include<unordered_map>
#include"Token_Types.h"
using TokenType = utils::TokenType;

namespace tokenUtils
{
	const std::unordered_map<std::string, TokenType>& keywords =
	{
		{ "class", TokenType::Class },
		{ "main", TokenType::Main },
		{ "constructor", TokenType::Constructor},
		{ "destructor", TokenType::Destructor },
		{ "new", TokenType::New },
		{ "delete", TokenType::Delete},
		{ "switch", TokenType::Switch },
		{ "default", TokenType::Default },
		{ "if", TokenType::If },
		{ "while", TokenType::While },
		{ "return", TokenType::Return },
		{ "break", TokenType::Break },
		{ "public", TokenType::Public },
		{ "private", TokenType::Private },
		{ "void", TokenType::Void },
		{ "int", TokenType::Int },
		{ "char", TokenType::Char },
		{ "break", TokenType::Break }
	};

	const std::unordered_map<char, TokenType>& simpleSigns = {
		{ '(', TokenType::ParenthOpen },
		{ ')', TokenType::ParenthClose },
		{ '{', TokenType::BracketOpen },
		{ '}', TokenType::BracketClose },
		{ '[', TokenType::SquareBracketOpen },
		{ ']', TokenType::SquareBracketClose },
		{ ',', TokenType::Comma },
		{ ';', TokenType::Semicolon },
		{ '+', TokenType::Plus },
		{ '-', TokenType::Minus },
		{ '*', TokenType::Multiply },
		{ '/', TokenType::Divide },
		{ '.', TokenType::Dot },
		{ ':', TokenType::Colon }
	};

}

#endif
