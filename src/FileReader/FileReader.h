#ifndef TKOM_FILEREADER
#define TKOM_FILEREADER

#include<istream>
#include<fstream>
#include<string>

class FileReader
{
public:
	FileReader(const std::string& file);
	FileReader(std::istream& stream);
	const char nextSign();
	void rewind();
	const bool hasFinished() const;
	const unsigned int& getCurrentLineNo() const;
	const unsigned int& getCurrentSignPos() const;
	const std::streampos getCurrentLinePos() const;
	const std::string seekSign(const char& c);
	const std::string getLine(const std::streampos& linePos);
private:
    std::ifstream filestream;
	std::istream& fin;
	bool isFile;
	unsigned int currentLine = 1;
	unsigned int currentSignPos = 0;
	std::streampos currentLinePos;
	char previousSign = 0;
};

#endif
