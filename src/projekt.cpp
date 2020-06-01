#include<iostream>
#include<exception>
#include"Lekser/Lekser.h"
#include"parser/Parser.h"
#include"Runner/Runner.h"
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
	try
	{
		Lekser lekser(filename);
		Parser parser(lekser);
		ast::Program tree;
		try
		{
			tree = *parser.parse();
		}
		catch (invalid_argument& e)
		{
			cout << e.what();
		}
		Runner runner;
		try
		{
		    runner.runProgram(tree);
		}
		catch( std::runtime_error& e)
		{
		    cout << e.what();
		}
	}
	catch (runtime_error& e)
	{
		cout << "Blad! " << e.what() << endl;
		return 0;
	}
}
