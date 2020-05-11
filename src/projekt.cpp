#include<iostream>
#include<exception>
#include"Lekser/Lekser.h"
#include"parser/Parser.hpp"
using namespace std;
using TokenType = tokenUtils::TokenTypes;

int main(int argc, const char** argv)
{
    string filename;
    if ( argc == 1 )
	{
	    cout << "Podaj nazwe pliku: ";
        cin >> filename;
	}
	else filename = string(argv[1]);
	//TODO zrobić to lepiej
	try
	{
		Lekser lekser(filename);
		Parser parser(lekser);
        auto i = parser.parse();

		/*Lekser lekser(filename);
		Token token = lekser.nextToken();
		while (token.type != tokenUtils::TokenTypes::EndOfFile)
		{
			if (token.type == tokenUtils::TokenTypes::Invalid) cout << "################# BLEDNY TOKEN #################\n";
			cout << "Typ: " << tokenUtils::getTokenTypeName(token.type);
			if (token.value != "")
				cout << " zawartosc: " << token.value;
			cout << endl;
			token = lekser.nextToken();
		}*/
	}
	catch (runtime_error e)
	{
		cout << "Blad! " << e.what() << endl;
		return 0;
	}
}
