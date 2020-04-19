#ifndef TKOM_LEKSER
#define TKOM_LEKSER

#include"FileReader.h"
#include"Token.h"

class Lekser
{
public:
	Lekser(const std::string& file);
	const Token nextToken();
	const std::string getLine(const std::streampos& linePos);
private:
	FileReader reader;
};

#endif
