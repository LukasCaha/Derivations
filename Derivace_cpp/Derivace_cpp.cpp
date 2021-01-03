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

struct Node
{
public:
	Token token;
	Node* leftChild, * rightChild, * parent;
};

bool IsOperator(string token) {
	char operators[] = { '+', '-', '*', '/' };
	if (token.length() == 1)
	{
		for (int i = 0; i < sizeof(operators); i++)
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
		for (int i = 0; i < sizeof(brackets); i++)
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
	return '?';
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

Node* Derivate(Node* node) {

	Node* newNode;
	if (node->token.GetType() == Number) {
		//CONSTANT
		if (node->token.value != -1) {
			newNode = NewNode(NumberToken("0"));
			return newNode;
		}
		//VARIABLE X
		if (node->token.value == -1) {
			newNode = NewNode(NumberToken("1"));
			return newNode;
		}
	}
	if (node->token.GetType() == Operator) {
		//PLUS
		//return node X with + OP
		//X.left = Derivate(node.left)
		//X.right = Derivate(node.right)
		if (node->token.GetOperator() == Plus) {
			newNode = NewNode(OperatorToken("+"));
			newNode->leftChild = Derivate(node->leftChild);
			newNode->rightChild = Derivate(node->rightChild);
			return newNode;
		}
		//MINUS
		//return node X with - OP
		//X.left = Derivate(node.left)
		//X.right = Derivate(node.right)
		if (node->token.GetOperator() == Minus) {
			newNode = NewNode(OperatorToken("-"));
			newNode->leftChild = Derivate(node->leftChild);
			newNode->rightChild = Derivate(node->rightChild);
			return newNode;
		}
		//MULTIPLY
		//return node X with + OP
		//X.left = * OP
		//X.right = * OP
		//X.left.left = Derivate(node.left)
		//X.left.right = node.right
		//X.right.left = node.left
		//X.right.right = Derivate(node.right)
		if (node->token.GetOperator() == Multiply) {
			newNode = NewNode(OperatorToken("+"));
			Node* newLeftNode = NewNode(OperatorToken("*"));
			newLeftNode->leftChild = Derivate(node->leftChild);
			newLeftNode->rightChild = node->rightChild;
			newNode->leftChild = newLeftNode;
			Node* newRightNode = NewNode(OperatorToken("*"));
			newRightNode->leftChild = node->leftChild;
			newRightNode->rightChild = Derivate(node->rightChild);
			newNode->rightChild = newRightNode;
			return newNode;
		}
		//DIVIDE
		//return node X with / OP
		//X.left = Y with - OP
		//Y.left = * OP
		//Y.right = * OP
		//Y.left.left = Derivate(node.left)
		//Y.left.right = node.right
		//Y.right.left = node.left
		//Y.right.right = Derivate(node.right)
		//X.right with * OP
		//X.right.left = node.right
		//X.right.right = node.right
		if (node->token.GetOperator() == Divide) {
			newNode = NewNode(OperatorToken("/"));
			//left
			Node* newLeftNode = NewNode(OperatorToken("-"));
			//A' * B
			Node* newLeftExpr = NewNode(OperatorToken("*"));
			Node* newLeftLeftNode = Derivate(node->leftChild);
			Node* newLeftRightNode = node->rightChild;
			newLeftExpr->leftChild = newLeftLeftNode;
			newLeftExpr->rightChild = newLeftRightNode;
			//A * B'
			Node* newRightExpr = NewNode(OperatorToken("*"));
			Node* newRightLeftNode = node->leftChild;
			Node* newRightRightNode = Derivate(node->rightChild);
			newRightExpr->leftChild = newRightLeftNode;
			newRightExpr->rightChild = newRightRightNode;

			newLeftNode->leftChild = newLeftExpr;
			newLeftNode->rightChild = newRightExpr;
			//right
			Node* newRightNode = NewNode(OperatorToken("*"));
			newRightNode->leftChild = node->rightChild;
			newRightNode->rightChild = node->rightChild;

			newNode->leftChild = newLeftNode;
			newNode->rightChild = newRightNode;
			return newNode;
		}
	}
	return node;
}

Node* Simplify(Node* node) {
	Node* newNode;
	if (node->token.GetType() == Operator && node->leftChild->token.GetType() == Number && node->rightChild->token.GetType() == Number) {
		//simplify const
		if (node->leftChild->token.value != -1 && node->rightChild->token.value != -1) {
			double left = node->leftChild->token.value;
			double right = node->rightChild->token.value;
			switch (node->token.GetOperator())
			{
			case Plus:
				return NewNode(NumberToken(to_string(left + right)));
			case Minus:
				return NewNode(NumberToken(to_string(left - right)));
			case Multiply:
				return NewNode(NumberToken(to_string(left * right)));
			case Divide:
				return NewNode(NumberToken(to_string(left / right)));
			default:
				break;
			}
		}

	}
	if (node->token.GetType() == Operator) {
		//PLUS
		//if one of simplified operands are 0 => result is 0
		if (node->token.GetOperator() == Plus) {
			newNode = NewNode(OperatorToken("+"));
			Node* left = Simplify(node->leftChild);
			Node* right = Simplify(node->rightChild);
			if (left->token.GetType() == Number) {
				if (left->token.value == 0) {
					newNode = right;
					return newNode;
				}
			}
			if (right->token.GetType() == Number) {
				if (right->token.value == 0) {
					newNode = left;
					return newNode;
				}
			}
			newNode->leftChild = left;
			newNode->rightChild = right;
			return newNode;
		}
		//MINUS
		//if simplified rightchild is 0 => result is 0
		if (node->token.GetOperator() == Minus) {
			newNode = NewNode(OperatorToken("-"));
			Node* left = Simplify(node->leftChild);
			Node* right = Simplify(node->rightChild);
			if (right->token.GetType() == Number) {
				if (right->token.value == 0) {
					newNode = left;
					return newNode;
				}
			}
			newNode->leftChild = left;
			newNode->rightChild = right;
			return newNode;
		}
		//MULTIPLY
		//0 multiplication => result 0
		//1 multiplication => result other child
		if (node->token.GetOperator() == Multiply) {
			newNode = NewNode(OperatorToken("*"));
			Node* left = Simplify(node->leftChild);
			Node* right = Simplify(node->rightChild);
			if (left->token.GetType() == Number) {
				if (left->token.value == 0) {
					newNode = NewNode(NumberToken("0"));
					return newNode;
				}
			}
			if (right->token.GetType() == Number) {
				if (right->token.value == 0) {
					newNode = NewNode(NumberToken("0"));
					return newNode;
				}
			}
			if (left->token.GetType() == Number) {
				if (left->token.value == 1) {
					newNode = right;
					return newNode;
				}
			}
			if (right->token.GetType() == Number) {
				if (right->token.value == 1) {
					newNode = left;
					return newNode;
				}
			}
			newNode->leftChild = left;
			newNode->rightChild = right;

			return newNode;
		}
		//DIVIDE
		if (node->token.GetOperator() == Divide) {
			newNode = NewNode(OperatorToken("/"));
			Node* left = Simplify(node->leftChild);
			Node* right = Simplify(node->rightChild);
			if (left->token.GetType() == Number) {
				if (left->token.value == 0) {
					newNode = NewNode(NumberToken("0"));
					return newNode;
				}
			}
			if (right->token.GetType() == Number) {
				if (right->token.value == 1) {
					newNode = left;
					return newNode;
				}
			}
			newNode->leftChild = left;
			newNode->rightChild = right;
			return newNode;
		}
	}
	return node;
}

void TreeOut(Node* node) {
	//cout << node->token.GetType()<<endl;
	if (node->token.GetType() == Operator)
	{
		cout << "(";
		TreeOut(node->leftChild);
		cout << " " << TypeToOperator(node->token.GetOperator()) << " ";
		TreeOut(node->rightChild);
		cout << ")";
	}
	if (node->token.GetType() == Number) {
		if (node->token.value == -1) {
			cout << "x";
		}
		else {
			cout << node->token.value;
		}
	}
}

int GetTreeDepth(Node* node) {
	if (node->token.tokenType != Number)
		return max(GetTreeDepth(node->leftChild), GetTreeDepth(node->rightChild)) + 1;
	else
		return 1;
}

bool CheckForErrors(std::vector<Token>& tokens) {
	//if two same typed tokens in row => error
	//if other token => error
	//if first type is 0 (=operator) => error
	int lastTokenType = 0;
	int paranthesisDepth = 0;
	std::vector<Token>::iterator it = tokens.begin();
	for (; it != tokens.end(); it++)
	{
		//two operators next to each other | two numbers nex to each other | other tokenTypes
		if (it->GetType() == lastTokenType)
			return false;
		lastTokenType = it->GetType();
		//other token present
		if (it->GetType() == 4) {
			return false;
		}
		//count '('
		if (it->GetType() == 2) {
			paranthesisDepth++;
		}
		//count ')'
		if (it->GetType() == 3) {
			paranthesisDepth--;
		}
		//more ')' then '('
		if (paranthesisDepth < 0)
			return false;
		//paranthesis next to operator '+)'
		if ((it->GetType() == 2 || it->GetType() == 3) && lastTokenType == 0)
			return false;
		//paranthesis next to operator '(+'
		if ((lastTokenType == 2 || lastTokenType == 3) && it->GetType() == 0)
			return false;
	}
	//more '(' then ')'
	if (paranthesisDepth > 0)
		return false;
	//if last token is operator => error
	if (lastTokenType == 0)
		return false;
	return true;
}

int main()
{

	std::ifstream file("tests.txt");

	string line;
	while (std::getline(file, line)) {
		//input -> infix
		vector<Token> tokens;
		ExtractTokens(line, tokens);
		bool ok = CheckForErrors(tokens);
		if (!ok) {
			cout << "Input error"<<endl;
			continue;
		}

		//infix -> postfix
		queue<Token> output;
		ShuntingYard(tokens, output);

		//postfix -> tree
		Node* root = ConstructTree(output);
		//pre-derivation simplification
		/*int depth = GetTreeDepth(root);
		for (int i = 0; i < depth; i++)
		{
			root = Simplify(root);
		}*/

		//tree -> derivative tree
		Node* resultRoot = Derivate(root);

		//simplification
		Node* simplified = resultRoot;
		int depth = GetTreeDepth(simplified);
		for (int i = 0; i < depth; i++)
		{
			simplified = Simplify(simplified);
		}


		//output
		TreeOut(simplified);
		cout << endl;
	}
}







