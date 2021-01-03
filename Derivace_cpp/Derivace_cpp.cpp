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
			Node* newLeftNode = NewNode(OperatorToken("-"));
			Node* newLeftExpr = NewNode(OperatorToken("*"));
			Node* newLeftLeftNode = Derivate(node->leftChild);
			Node* newLeftRightNode = node->rightChild;
			newLeftExpr->leftChild = newLeftLeftNode;
			newLeftExpr->rightChild = newLeftRightNode;
			Node* newRightExpr = NewNode(OperatorToken("*"));
			Node* newRightLeftNode = node->leftChild;
			Node* newRightRightNode = Derivate(node->rightChild);
			newRightExpr->leftChild = newRightLeftNode;
			newRightExpr->rightChild = newRightRightNode;
			newLeftNode->leftChild = Derivate(node->leftChild);
			newLeftNode->rightChild = node->rightChild;

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
	if (node->token.GetType() == Number) {
		newNode = node;
		return node;
	}
	if (node->token.GetType() == Operator) {
		//PLUS
		//if one of simplified operands are 0 => result is 0
		if (node->token.GetOperator() == Plus) {
			newNode = NewNode(OperatorToken("+"));
			newNode->leftChild = Simplify(node->leftChild);
			newNode->rightChild = Simplify(node->rightChild);
			if (newNode->leftChild->token.GetType() == Number) {
				if (newNode->leftChild->token.value == 0) {
					newNode = newNode->rightChild;
				}
			}
			if (newNode->rightChild->token.GetType() == Number) {
				if (newNode->rightChild->token.value == 0) {
					newNode = newNode->leftChild;
				}
			}
			return newNode;
		}
		//MINUS
		//if simplified rightchild is 0 => result is 0
		if (node->token.GetOperator() == Minus) {
			newNode = NewNode(OperatorToken("-"));
			newNode->leftChild = Simplify(node->leftChild);
			newNode->rightChild = Simplify(node->rightChild);
			if (newNode->rightChild->token.GetType() == Number) {
				if (newNode->rightChild->token.value == 0) {
					newNode = newNode->leftChild;
				}
			}
			return newNode;
		}
		//MULTIPLY
		//0 multiplication => result 0
		//1 multiplication => result other child
		if (node->token.GetOperator() == Multiply) {
			newNode = NewNode(OperatorToken("*"));
			newNode->leftChild = Simplify(node->leftChild);
			newNode->rightChild = Simplify(node->rightChild);
			if (newNode->leftChild->token.GetType() == Number) {
				if (newNode->leftChild->token.value == 0) {
					newNode = NewNode(NumberToken("0"));
				}
			}
			if (newNode->rightChild->token.GetType() == Number) {
				if (newNode->rightChild->token.value == 0) {
					newNode = NewNode(NumberToken("0"));
				}
			}
			if (newNode->leftChild->token.GetType() == Number) {
				if (newNode->leftChild->token.value == 1) {
					newNode = newNode->rightChild;
				}
			}
			if (newNode->rightChild->token.GetType() == Number) {
				if (newNode->rightChild->token.value == 1) {
					newNode = newNode->leftChild;
				}
			}

			return newNode;
		}
		//DIVIDE
		if (node->token.GetOperator() == Divide) {
			newNode = NewNode(OperatorToken("/"));
			newNode->leftChild = Simplify(node->leftChild);
			newNode->rightChild = Simplify(node->rightChild);
			if (newNode->leftChild->token.GetType() == Number) {
				if (newNode->leftChild->token.value == 0) {
					newNode = NewNode(NumberToken("0"));
				}
			}
			if (newNode->rightChild->token.GetType() == Number) {
				if (newNode->rightChild->token.value == 1) {
					newNode = newNode->leftChild;
				}
			}
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
		cout <<" "<< TypeToOperator( node->token.GetOperator())<<" ";
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
	
		Node* resultRoot = Derivate(root);
		TreeOut(resultRoot);
		cout << endl;
		//Node* simplifiedResultRoot = Simplify(resultRoot);
		//TreeOut(simplifiedResultRoot);
		//cout << endl;
	}
}







