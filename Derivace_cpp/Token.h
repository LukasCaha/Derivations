#pragma once

#include <iostream>
#include <string>

enum TokenType { Operator, Number, LeftParenthesis, RightParenthesis, Other };
enum OperatorType { Plus, Minus, Multiply, Divide };

class Token
{
public:
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
};

class NumberToken : public Token
{
public:
	double value;
	NumberToken(std::string _value) {
		tokenType = Number;
		value = std::stod(_value);
	}
};

class OperatorToken : public Token
{
public:
	OperatorType operatorType;
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
};

