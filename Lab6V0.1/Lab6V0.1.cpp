// Lab6V0.1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Interpreter.h"
#include <iostream>
#include <fstream>
bool readFromFile(Interpreter& i, std::string fileName) 
{
	std::string line;
	std::ifstream myfile(fileName);
	if (myfile.is_open()) {
		while (std::getline(myfile, line))
		{
			i.evaluate(line);
		}
		myfile.close();
		return true;
	}
	else
	{
		std::cout << "\nError reading file\n";
		return false;
	}
}

int main()
{

	Interpreter i;
	readFromFile(i, "mgramma.txt");
	//i.evaluate("config dec");
	//i.evaluate("x1 = ( ( 1 + 100 ) / 2 ) + 1 * 2");
	//i.evaluate("print x1 - 20");
	std::cin.get();
    return 0;
}
