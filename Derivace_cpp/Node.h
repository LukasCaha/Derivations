#pragma once
#include "Token.h"
class Node
{
public:
	Token token;
	Node* parent;
	Node* leftChild;
	Node* rightChild;

};

