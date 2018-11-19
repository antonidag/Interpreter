#pragma once
#include <vector>
#include <string>
#include <list>
#include <map>
class Interpreter
{
	enum eConfig
	{
		hex = 1,
		dec = 2,
		bin = 3
	};
	int config;
	std::map<std::string, int> varibles;
	std::list<std::string> t;
	std::vector<std::string> tokens;
	void prasePrint();
	void praseConfig();
	void praseVariable();
	void tokenInitialization(std::string line);
	int math();
	void insertSum(int index, int sum);
	bool isNumber(const char c);
	bool isNumber(const std::string s);
	bool isOperator(const char c);
	bool isVariable(const std::string token);

	int makeInt(const std::string token);
	void insertVariable(std::string var, int number);
	void errorCheck();
	void print(int sum);

	friend std::ostream&operator<< (std::ostream& out, const std::vector<std::string> rhs);

public:
	Interpreter();
	~Interpreter();
	void evaluate(const std::string& line);

};

