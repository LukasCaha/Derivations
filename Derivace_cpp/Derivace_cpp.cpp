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
#include "Derivace_cpp.h"

using namespace std;

typedef struct Node
{
public:
	Token token;
	Node* leftChild, * rightChild, * parent;
};

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

void ShuntingYard(std::vector<Token>& tokens, std::queue<Token>& output)
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

Node* NewNode(Token t)
{
	Node* temp = new Node;
	temp->parent = NULL;
	temp->leftChild = NULL;
	temp->rightChild = NULL;
	temp->token = t;
	return temp;
};

Node* ConstructTree(std::queue<Token>& output)
{
	stack<Node*> st;
	Node* t, * t1, * t2;

	// Traverse through every character of
	// input expression
	int cacheOutputSize = output.size();
	//cout << "size:" << cacheOutputSize << "\t\t";
	for (int i = 0; i < cacheOutputSize; i++)
	{
		Token current = output.front();
		output.pop();
		// If operand, simply push into stack
		if (current.GetType() == Number)
		{
			//cout << current.value << " ";
			t = NewNode(current);
			st.push(t);
		}
		else if (current.GetType() == Operator) // operator
		{
			//cout << TypeToOperator(current.GetOperator()) << " ";
			t = NewNode(current);

			if (st.size() > 1) {
				// Pop two top nodes
				t1 = st.top(); // Store top
				st.pop();      // Remove top
				t2 = st.top();
				st.pop();

				//  make them children
				t->rightChild = t1;
				t->leftChild = t2;

				// Add this subexpression to stack
				st.push(t);
			}
			else {
				//cout << "error";
			}
		}
		else {
			//cout << "error3";
		}
	}

	//  only element will be root of expression
	// tree
	if (st.size() > 0) {
		t = st.top();
		st.pop();
		return t;
	}
	else {
		//cout << "error2";
		return NULL;
	}

}

int main()
{
	std::ifstream file("tests.txt");

	string line;
	while (std::getline(file, line)) {
		if (line[0] == 'c') break;
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
		queue<Token> output;
		ShuntingYard(tokens, output);

		/*TEST OUTPUT backwards postfix*/
		/*while (output.size() > 0) {
			if (output.top().GetType() == 0)
				cout << TypeToOperator(output.top().operatorType) << " ";
			else if (output.top().GetType() == 1) {
				if (output.top().value == -1)
					cout << "x" << " ";
				else
					cout << output.top().value << " ";
			}
			else {
				cout << "|";
			}
			output.pop();
		}
		cout << endl << endl;*/
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

		Node* root = ConstructTree(output);
		/*cout << " type:" << root->token.GetType() << endl;
		cout << " type:" << root->leftChild->token.GetType() << endl;
		cout << " type:" << root->rightChild->token.GetType() << endl;*/
	}
}






