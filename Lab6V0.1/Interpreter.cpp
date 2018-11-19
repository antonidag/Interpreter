#include "stdafx.h"
#include "Interpreter.h"
#include <regex>
#include <iostream>
#include <stack>
#include <queue>
#include <bitset>
#include <thread>
#include <chrono>
#include <iostream>
#include <stdexcept>  
Interpreter::Interpreter()
{
	config = 2;
}


Interpreter::~Interpreter()
{
}

void Interpreter::evaluate(const std::string& line)
{
	tokenInitialization(line);
	
	if (tokens[0] == "print") {
		prasePrint();
	}
	else if(tokens[0] == "config")
	{
		praseConfig();
	}
	else
	{
		praseVariable();
	}

}


void Interpreter::prasePrint()
{
	tokens.erase(tokens.begin());
	int sum = math();
	print(sum);
}

void Interpreter::praseConfig()
{
	if (tokens.size() > 1)
	{
		if (tokens[1] == "hex")
			config = 1;
		else if (tokens[1] == "dec")
			config = 2;
		else if (tokens[1] == "bin")
			config = 3;
	}
	else
	{
		std::cout << "Config error no/wrong argument" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		std::exit(-1);
	}
}

void Interpreter::praseVariable()
{

	std::string var = tokens[0];
	if (isVariable(tokens[0])) 
	{
		tokens.erase(tokens.begin());
		if (tokens[0] == "=")
		{
			tokens.erase(tokens.begin());
			int sum = math();
			insertVariable(var, sum);
		}
		else
		{
			//throw std::runtime_error("Variable prase error: No assign operator '='\n");
			std::cout << "Variable prase : Error Missing assign operator '='" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			std::exit(-1);
		}
	}
	else
	{
		std::cout << "Variable prase : Error variable cant be: " << var << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		std::exit(-1);
	}
}


void Interpreter::tokenInitialization(std::string line)
{
	tokens.clear();
	std::string token;
	line.push_back(' ');
	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] == ' ') {
			tokens.push_back(token);
			token = "";
			continue;
		}
		token.push_back(line[i]);
	}
}

int Interpreter::math()
{
	bool done = false;
	int count = 0;
	while (!done)
	{

		int start = 0;
		int end = tokens.size();
		int sum = -999;
		bool operPrioOne = false;

		// Priority one 
		// Checks for '(' and ')'
		for (size_t i = 0; i < end; i++)
		{
			if (tokens[i] == "(") {
				start = i;
			}

			if (tokens[i] == ")") {
				end = i;
				break;
			}
		}
		int res = end - start;
		if (res == 2)
		{
			std::cout << "Syntax error at :" <<std::endl;
			for (size_t i = start; i < end; i++)
			{
				std::cout << tokens[i] << " ";
			}
			std::this_thread::sleep_for(std::chrono::seconds(5));
			std::exit(-1);
		}
		// Priority second 
		// Checks for '*' and '/'
		for (size_t i = start; i < end; i++)
		{
			if (tokens[i] == "*") {
				sum = makeInt(tokens[i - 1]) * makeInt(tokens[i + 1]);
				insertSum(i, sum);
				operPrioOne = true;
				break;
			}
			else if (tokens[i] == "/")
			{
				sum = makeInt(tokens[i - 1]) / makeInt(tokens[i + 1]);
				insertSum(i, sum);
				operPrioOne = true;
				break;
			}
		}

		// If this is true 
		// then--> continue
		if (operPrioOne)
			continue;

		// Priority third
		// Checks '+' and '-'
		for (size_t i = start; i < end; i++)
		{
			if (tokens[i] == "+") {
				sum = makeInt(tokens[i - 1]) + makeInt(tokens[i + 1]);
				insertSum(i, sum);
				break;
			}
			else if (tokens[i] == "-")
			{
				sum = makeInt(tokens[i - 1]) - makeInt(tokens[i + 1]);
				insertSum(i, sum);
				break;
			}
		}

		// If last element end loop
		if (tokens.size() == 1)
			done = true;
		if (count > tokens.size()) 
		{
			std::cout << "void Math : Syntax error at: " << tokens << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			std::exit(-1);
		}
		count++;
	}
	
	return makeInt(tokens[0]);
}

void Interpreter::insertSum(int index, int sum)
{
	// Counter
	int c = 0;

	// Removes three signs
	do
	{
		tokens.erase(tokens.begin() + index - 1);
		c++;
	} while (c != 3);

	// Checks if insert or push_back
	if (index - 1 == tokens.size() || tokens.size() == 0)
		tokens.push_back(std::to_string(sum));
	else
		tokens.insert(tokens.begin() + index - 1, std::to_string(sum));

	c = 0;
	int remove = 0;
	// Checks if the sides if sum are open and close, if so then remove them.
	if (tokens.size() > 1 && (index - 2) >= 0) {
		if (tokens[index - 2] == "(" && tokens[index] == ")") {
			do
			{
				tokens.erase(tokens.begin() + index - remove);
				remove = 2 + remove;
				c++;
			} while (c != 2);
		}
	}

}

bool Interpreter::isNumber(const char c)
{
	if (isdigit(c)) {
		return true;
	}
	return false;
}

bool Interpreter::isNumber(const std::string s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

bool Interpreter::isOperator(const char c)
{
	if (c == '=' || c == '+' || c == '-' || c == '(' || c == ')' || c == '/' || c == '*' || c == '"' || c == '\'' || c == '@' || c == '#' || c == '£' || c == '¤') {
		return true;
	}
	return false;
}

bool Interpreter::isVariable(const std::string token)
{	

	if (isNumber(token[0]))
		return false;
	if (isOperator(token[0]))
		return false;
	return true;
	
}

int Interpreter::makeInt(const std::string token)
{
	if (isVariable(token) && token[0] != '-')
	{
		std::map<std::string, int>::iterator it;
		it = varibles.find(token);
		if (it != varibles.end())
		{
			return varibles[token];
		}
		else
		{
			//throw std::runtime_error("Varible: " + token + " dose not exist\n");
			std::cout << "int makeInt : Varible: " << token << " dose not exist" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			std::exit(-1);
		}	
	}

	//std::string::size_type sz;
	//int i_dec = std::stoi(token, &sz);
	if (isNumber(token))
	{
		int i_dec = std::atoi(token.c_str());
		return i_dec;
	}
	else
	{
		std::cout << "int makeInt : Syntax error at: " << token << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		std::exit(-1);
	}

}

void Interpreter::insertVariable(std::string var, int number)
{

	std::pair<std::map<std::string, int>::iterator, bool> ret;
	ret = varibles.insert(std::pair<std::string, int>(var, number));
	if (ret.second == false) {
		std::cout << "element "<< var <<" already existed";
		std::cout << " with a value of " << ret.first->second << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(5));
		std::exit(-1);
		//throw std::runtime_error("element: " + var + " already existed with a value of : " + ret.first->second + "\n"");
	}
}

void Interpreter::errorCheck()
{

}

void Interpreter::print(int sum)
{
	std::string binary = std::bitset<8>(sum).to_string();
	switch (config)
	{
	case hex:
		std::cout << std::hex << sum << std::endl;
		break;
	case dec:
		std::cout << sum << std::endl;
		break;

	case bin:
		std::cout << binary << std::endl;
		break;

	default:
		break;
	}
}


std::ostream & operator<<(std::ostream & out, const std::vector<std::string> rhs)
{
	for (size_t i = 0; i < rhs.size(); i++)
	{
		out << rhs[i] << " ";
	}
	return out;
}
