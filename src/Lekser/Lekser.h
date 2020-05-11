#ifndef TKOM_LEKSER
#define TKOM_LEKSER

#include"../FileReader/FileReader.h"
#include"../Token/Token.h"

class Lekser
{
public:
	Lekser(const std::string& file);
	Lekser(std::istream& stream);
	const Token nextToken();
	const std::string getLine(const std::streampos& linePos);
private:
	FileReader reader;
};

#endif
