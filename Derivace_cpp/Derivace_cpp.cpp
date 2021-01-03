// Derivace_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>  
#include <fstream>
#include <queue>
#include <stack>

#include "Token.h"
#include "Node.h"
#include "Derivace_cpp.h"

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
	if (token == "x") return true;
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
	char brackets[] = { '(', ')' };
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

char TypeToOperator(int type) {
	if (type == 0) return '+';
	if (type == 1) return '-';
	if (type == 2) return '*';
	if (type == 3) return '/';
}

void ShuntingYard(std::vector<Token>& tokens, std::stack<Token>& output)
{
	stack<Token> operatorStack;
	std::vector<Token>::iterator iterator = tokens.begin();
	for (; iterator != tokens.end(); iterator++) {
		//if number push to queue
		//if operator
		//	if operator on stack has < priority then currently pushing
		//		push to operator stack
		//	else
		//		pop operator while priority >=
		//if paranthesis
		//	pop from operator stact until opening paranthesis
		if (iterator->GetType() == Number) {
			output.push(*iterator);
		}
		if (iterator->GetType() == Operator) {
			int operatorPriority = iterator->GetPriority();
			int stackOperatorPriority = -1;
			if (operatorStack.size() > 0)
				stackOperatorPriority = operatorStack.top().GetPriority();

			if (stackOperatorPriority < operatorPriority) {
				operatorStack.push(*iterator);
			}
			else {
				while (!(stackOperatorPriority < operatorPriority)) {
					Token poped = operatorStack.top();
					if (poped.GetType() == 0)
						output.push(poped);
					operatorStack.pop();
					stackOperatorPriority = -1;
					if (operatorStack.size() > 0)
						stackOperatorPriority = operatorStack.top().GetPriority();
				}
				operatorStack.push(*iterator);
			}
		}
		if (iterator->GetType() == LeftParenthesis) {
			operatorStack.push(*iterator);
		}
		if (iterator->GetType() == RightParenthesis) {
			while (operatorStack.size() > 0 && operatorStack.top().GetType() != 2) {
				Token poped = operatorStack.top();
				if (poped.GetType() == 0)
					output.push(poped);
				operatorStack.pop();
			}
		}
	}
	while (operatorStack.size() > 0) {
		Token poped = operatorStack.top();
		output.push(poped);
		operatorStack.pop();
	}
}

void ExtractTokens(std::string& line, std::vector<Token>& tokens)
{
	stringstream ss(line);
	string token;
	while (getline(ss, token, ' ')) {
		if (IsOperator(token))
		{
			tokens.push_back(OperatorToken(token));
		}
		else if (IsNumber(token))
		{
			tokens.push_back(NumberToken(token));
		}
		else if (IsParenthesis(token)) {
			tokens.push_back(Token(token));
		}
		else {
			tokens.push_back(Token());
		}
	}
}

int main()
{
	std::ifstream file("tests.txt");

	string line;
	while (std::getline(file, line)) {
		//cout << line << endl;
		vector<Token> tokens;
		ExtractTokens(line, tokens);

		/*TEST OUTPUT*/
		/*std::vector<Token>::iterator it = tokens.begin();
		for (; it != tokens.end(); it++)
		{
			if (it->GetType() == 0)
				cout << TypeToOperator(it->operatorType) << " ";
			else {
				if (it->value == -1)
					cout << "x" << " ";
				else
					cout << it->value << " ";
			}
		}
		cout << endl;*/
		/*TEST OUTPUT END*/

		//shunting yard
		stack<Token> output;
		ShuntingYard(tokens, output);

		/*TEST OUTPUT backwards postfix*/
		while (output.size() > 0) {
			if(output.top().GetType() == 0)
				cout << TypeToOperator(output.top().operatorType) << " ";
			else if (output.top().GetType() == 1) {
				if(output.top().value == -1)
					cout << "x" << " ";
				else
					cout << output.top().value << " ";
			}
			else {
				cout << "|";
			}
			output.pop();
		}
		cout << endl << endl;
		/*TEST OUTPUT END*/

		//postfix -> tree
		//start from end
		//root = last token
		//right child = next
		//if operator recursively
		//else
		/*Node root = Node();
		root.token = output.top();
		output.pop();
		root.parent = NULL;

		FillNode(root, output);*/
	}
}

void FillNode(Node& current, std::stack<Token>& output)
{
	Node *newNode = (Node*)malloc(sizeof(Node));
	current.rightChild = newNode;
	current.rightChild->parent = &current;
	current.rightChild->token = output.top();
	output.pop();
	if (current.token.GetType() == Operator) {
		FillNode(*current.rightChild, output);
		FillNode(*current.leftChild, output);
	}
}


