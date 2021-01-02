// Derivace_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>  
#include <fstream>

#include "Token.h"

using namespace std;

bool IsOperator(string token) {
	char operators[] = { '+', '-', '*', '/' };
	if (token.length() == 1)
	{
		for (int i = 0; i < strlen(operators); i++)
		{
			if (token[0] == operators[i]) {
				return true;
			}
		}
	}
	return false;
}

bool IsNumber(string token) {
	bool hasDot = false;
	for (int i = 0; i < token.length(); i++)
	{
		if (!isdigit(token[i]))
		{
			if (token[i] == '.' && !hasDot)
			{
				hasDot = true;
			}
			else {
				return false;
			}
		}
	}
	return true;
}

bool IsParenthesis(string token) {
	char brackets[] = { '(', ')'};
	if (token.length() == 1)
	{
		for (int i = 0; i < strlen(brackets); i++)
		{
			if (token[0] == brackets[i]) {
				return true;
			}
		}
	}
	return false;
}

int main()
{
	std::ifstream file("tests.txt");

	string line;
	while (file >> line) {
		vector<Token> tokens;
		stringstream ss(line);
		string token;
		while (getline(ss, token, ' ')) {
			if (IsOperator(token))
			{
				tokens.push_back(OperatorToken(token));
			}
			if (IsNumber(token))
			{
				tokens.push_back(NumberToken(token));
			}
			if (IsParenthesis(token)) {
				tokens.push_back(Token(token));
			}
		}
		std::vector<Token>::iterator it = tokens.begin();
		for (;it != tokens.end(); it++)
		{
			cout << ((NumberToken)it)->value;
		}
	}
}


