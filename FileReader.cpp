#include"FileReader.h"
#include<exception>
using namespace std;

FileReader::FileReader(const string& file) : fin(file)
{
	if (fin.fail())
	{
		throw runtime_error("Plik " + file + " nie istnieje");
	}
	currentLinePos = fin.tellg();
}

const char FileReader::nextSign()
{
	char sign;
	while (true)
	{
		sign = fin.get();
		if (sign == '\n' || sign == '\r')
		{
			if (previousSign != sign && (previousSign == '\n' || previousSign == '\r')) {}
			else
			{
				currentLine++;
				currentSignPos = 0;
				currentLinePos = fin.tellg();
			}
			previousSign = sign;
			continue;
		}
		previousSign = sign;
		currentSignPos++;
		return sign;
	}
}

void FileReader::rewind()
{
	fin.unget().unget();
	previousSign = fin.get();
	auto peek = fin.peek();
	if (peek == '\n' || peek == '\r')
	{
		previousSign = fin.get();
		return;
	}
	--currentSignPos;
}

const bool FileReader::hasFinished() const
{
	return fin.eof();
}

const unsigned int& FileReader::getCurrentLineNo() const
{
	return currentLine;
}

const unsigned int& FileReader::getCurrentSignPos() const
{
	return currentSignPos;
}

const streampos FileReader::getCurrentLinePos() const
{
	return currentLinePos;
}

const string FileReader::getLine(const streampos& linePos)
{
	const streampos currentPos = fin.tellg();
	string line;
	fin.seekg(linePos);
	getline(fin, line);
	fin.seekg(currentPos);
	return line;
}
