#include"FileReader.h"
#include<exception>
using namespace std;

FileReader::FileReader(const string& file): filestream(file), fin(filestream)       //inicjalizacja za pomoc¹ pliku
{
	if (fin.fail())
	{
		throw runtime_error("Plik " + file + " nie istnieje");
	}
	currentLinePos = fin.tellg();
}

FileReader::FileReader(std::istream& stream): fin(stream)                           //inicjalizacja za pomoc¹ strumienia
{
	if (fin.fail())
	{
		throw runtime_error("Blad strumienia");
	}
	currentLinePos = fin.tellg();
}

const char FileReader::nextSign()   //zwraca kolejny znak, pilnuje miejsca w linii i nowe linie
{
    //if(fin.eof()) return 0;       //while powinien to zlapac
	char sign;
	while (!fin.eof())
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
	return 0;
}

void FileReader::rewind()       //cofa znak
{
    if(fin.eof()) return;
	fin.unget().unget();        //TODO psuje się na windowsie
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

const string FileReader::seekSign(const char& c)    //dla pomijania komentarzy i brania string literali
{
    string s = "";
    char sign = fin.get();
    while(!fin.eof())
    {
        if(sign == c)
        {
            return s;
        }
        s += sign;
        previousSign = sign;
        sign = fin.get();
    }
    return "\0";
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
