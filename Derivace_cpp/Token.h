#pragma once

#include <iostream>
#include <string>

enum TokenType { Operator, Number, LeftParenthesis, RightParenthesis, Other };
enum OperatorType { Plus, Minus, Multiply, Divide };

class Token
{
public:
	OperatorType operatorType;
	double value = 0;
	TokenType tokenType;
	Token() { tokenType = Other; }
	Token(std::string _tokenType) {
		if (_tokenType[0] == '(')
		{
			tokenType = LeftParenthesis;
		}
		else {
			tokenType = RightParenthesis;
		}
	}
	int GetType() {
		return tokenType;
	}
	int GetOperator() {
		return operatorType;
	}

	int GetPriority() {
		switch (operatorType)
		{
		case Plus:
			return 0;
		case Minus:
			return 0;
		case Multiply:
			return 1;
		case Divide:
			return 1;
		default:
			break;
		}
	}
};

class NumberToken : public Token
{
public:
	bool isX;
	NumberToken(std::string _value) {
		tokenType = Number;
		if (_value == "x") {
			value = -1;
			isX = true;
		}
		else {
			value = std::stod(_value);
			isX = false;
		}
	}
	int GetPriority() {
		switch (operatorType)
		{
		case Plus:
			return 0;
		case Minus:
			return 0;
		case Multiply:
			return 1;
		case Divide:
			return 1;
		default:
			break;
		}
	}
};

class OperatorToken : public Token
{
public:
	OperatorToken(std::string _operatorType) {
		tokenType = Operator;
		switch (_operatorType[0])
		{
		case '+':
			operatorType = Plus;
			break;
		case '-':
			operatorType = Minus;
			break;
		case '*':
			operatorType = Multiply;
			break;
		case '/':
			operatorType = Divide;
			break;
		default:
			break;
		}
		
	}
	int GetPriority() {
		switch (operatorType)
		{
		case Plus:
			return 0;
		case Minus:
			return 0;
		case Multiply:
			return 1;
		case Divide:
			return 1;
		default:
			break;
		}
	}
};

