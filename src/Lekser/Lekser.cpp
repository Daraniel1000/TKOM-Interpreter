#include"Lekser.h"
#include<cctype>
#include<algorithm>
#include<istream>
#include"../Token/Keywords.h"
using namespace std;

Lekser::Lekser(const string& file): reader(file)
{}

Lekser::Lekser(istream& stream): reader(stream)
{}

const Token Lekser::nextToken()
{
	Token token;
	auto sign = reader.nextSign();
	while (isspace(sign))
	{
		sign = reader.nextSign();
	}
	token.line = reader.getCurrentLineNo();
	token.pos = reader.getCurrentSignPos() - 1;
	token.lineStart = reader.getCurrentLinePos();
	if (reader.hasFinished() || sign==0)
	{
		//EOF
		token.type = TokenType::EndOfFile;
		return token;
	}
	if (isalpha(sign))
	{
		string buffer;
		do
		{
			buffer.push_back(sign);
			sign = reader.nextSign();
		} while (isalnum(sign));
		reader.rewind();
		auto tempBuff = buffer;
		transform(tempBuff.begin(), tempBuff.end(), tempBuff.begin(), ::tolower);
		if (tokenUtils::keywords.find(tempBuff) != tokenUtils::keywords.end())
		{
			//sł. kluczowe
			token.type = tokenUtils::keywords.at(tempBuff);
		}
		else
		{
			//nazwa
			token.type = TokenType::Identifier;
			token.value = buffer;
		}
	}
	else if (isdigit(sign))
	{
		//Liczba
		string buffer;
		do
		{
			buffer.push_back(sign);
			sign = reader.nextSign();
		} while (isdigit(sign));
		reader.rewind();
		token.type = TokenType::NumberLiteral;
		token.valueInt = atoi(buffer.c_str());
	}
	else
	{
		//Znaki
		switch (sign)
		{
			case '=':
			{
				if (reader.nextSign() == '=')
				{
					token.type = TokenType::Equality;
				}
				else
				{
					reader.rewind();
					token.type = TokenType::Assignment;
				}
				break;
			}
			case '<':
			{
				if (reader.nextSign() == '=')
				{
					token.type = TokenType::LessOrEqual;
				}
				else
				{
					reader.rewind();
					token.type = TokenType::Less;
				}
				break;
			}
			case '>':
			{
				if (reader.nextSign() == '=')
				{
					token.type = TokenType::GreaterOrEqual;
				}
				else
				{
					reader.rewind();
					token.type = TokenType::Greater;
				}
				break;
			}
			case '!':
			{
				if (reader.nextSign() == '=')
				{
					token.type = TokenType::Inequality;
				}
				else
				{
					reader.rewind();
					token.type = TokenType::Negation;
				}
				break;
			}
			case '&':
			{
				if (reader.nextSign() == '&')
				{
					token.type = TokenType::And;
				}
				else
				{
					reader.rewind();
					token.type = TokenType::Invalid;
				}
				break;
			}
			case '|':
			{
				if (reader.nextSign() == '|')
				{
					token.type = TokenType::Or;
				}
				else
				{
					reader.rewind();
					token.type = TokenType::Invalid;
				}
				break;
			}
			case '/':
			{
				if (reader.nextSign() == '/')
				{
				    reader.seekSign('\n');
				    reader.rewind();
					token.type = TokenType::Comment;
				}
				else
				{
					reader.rewind();
					token.type = TokenType::Divide;
				}
				break;
			}
			case '"':
			{
			    string s = reader.seekSign('"');
			    if(s == "\0")
                    token.type = TokenType::Invalid;
			    else
                {
                    token.type = TokenType::StringLiteral;
                    token.value = s;
                }
				break;
			}
			default:
			{
				if (tokenUtils::simpleSigns.count(sign) == 1)
				{
					token.type = tokenUtils::simpleSigns.at(sign);
				}
				else
				{
					token.type = TokenType::Invalid;
				}
				break;
			}
		}
	}
	if(token.type == TokenType::Comment)
        return this->nextToken();
	return token;
}

const string Lekser::getLine(const streampos& linePos)
{
	return reader.getLine(linePos);
}
