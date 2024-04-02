#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

bool isOperator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == 's' || c == 'c' || c == 't' || c == 'l' || c == 'q';
}

bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

bool isLeftParenthesis(char c) {
	return c == '(';
}

bool isRightParenthesis(char c) {
	return c == ')';
}

bool isWhiteSpace(char c) {
	return c == ' ' || c == '\t' || c == '\n';
}

bool isInvalidChar(char c) {
	return !isOperator(c) && !isDigit(c) && !isLeftParenthesis(c) && !isRightParenthesis(c) && !isWhiteSpace(c);
}

bool isOperatorHigherOrEqual(char op1, char op2) {
	if ((op1 == '*' || op1 == '/' || op1 == '^' || op1 == 's' || op1 == 'c' || op1 == 't' || op1 == 'l' || op1 == 'q') && (op2 == '+' || op2 == '-')) {
		return true;
	}
	if ((op1 == '^' || op1 == 's' || op1 == 'c' || op1 == 't' || op1 == 'l' || op1 == 'q') && (op2 == '*' || op2 == '/')) {
		return true;
	}
	return false;
}

double calculate(double operand1, double operand2, char op) {
	switch (op) {
		case '+':
			return operand1 + operand2;
		case '-':
			return operand1 - operand2;
		case '*':
			return operand1 * operand2;
		case '/':
			return operand1 / operand2;
		case '^':
			return pow(operand1, operand2);
		case 's':
			return sin(operand2 * M_PI / 180.0);
		case 'c':
			return cos(operand2 * M_PI / 180.0);
		case 't':
			return tan(operand2 * M_PI / 180.0);
		case 'l':
			return log(operand2);
		case 'q':
			return sqrt(operand2);
	}
	return 0;
}

bool evaluate(string expr, double& result) {
	stack<double> operands;
	stack<char> operators;
	int i = 0;
	int len = expr.length();
	while (i < len) {
		char c = expr[i];
		if (isWhiteSpace(c)) {
			i++;
			continue;
		}
		if (isInvalidChar(c)) {
			return false;

		}
		if (isDigit(c)) {
			double operand;
			stringstream ss;
			ss << c;
			i++;
			while (i < len && (isDigit(expr[i]) || expr[i] == '.')) {
				ss << expr[i];
				i++;
			}
			ss >> operand;
			operands.push(operand);
			continue;
		}
		if (isOperator(c)) {
			if (c == '-' && (i == 0 || isOperator(expr[i - 1]) || isLeftParenthesis(expr[i - 1]))) {
				double operand;
				stringstream ss;
				ss << '-';
				i++;
				while (i < len && (isDigit(expr[i]) || expr[i] == '.')) {
					ss << expr[i];
					i++;
				}
				ss >> operand;
				operands.push(operand);
				continue;
			}
			while (!operators.empty() && isOperatorHigherOrEqual(operators.top(), c)) {
				double operand2 = operands.top();
				operands.pop();
				double operand1 = operands.top();
				operands.pop();
				char op = operators.top();
				operators.pop();
				double result = calculate(operand1, operand2, op);
				operands.push(result);
			}
			operators.push(c);
			i++;
			continue;
		}
		if (isLeftParenthesis(c)) {
			operators.push(c);
			i++;
			continue;
		}
		if (isRightParenthesis(c)) {
			if (operators.empty()) {
				return false;
			}
			char op = operators.top();
			while (!isLeftParenthesis(op)) {
				if (operators.empty()) {
					return false;
				}
				double operand2 = operands.top();
				operands.pop();
				double operand1 = operands.top();
				operands.pop();
				double result = calculate(operand1, operand2, op);
				operands.push(result);
				operators.pop();
				if (operators.empty()) {
					return false;
				}
				op = operators.top();
			}
			operators.pop();
			if (!operators.empty() && (operators.top() == 's' || operators.top() == 'c' || operators.top() == 't' || operators.top() == 'l' || operators.top() == 'q')) {
				char op = operators.top();
				operators.pop();
				double operand = operands.top();
				operands.pop();
				double result = calculate(0, operand, op);
				operands.push(result);
			}
			i++;
			continue;
		}
		if (c == 's' || c == 'c' || c == 't' || c == 'l' || c == 'q') {
			operators.push(c);
			i++;
			continue;
		}
	}
	while (!operators.empty()) {
		char op = operators.top();
		operators.pop();
		if (isLeftParenthesis(op) || operands.empty()) {
			return false;
		}
		double operand2 = operands.top();
		operands.pop();
		if (operands.empty()) {
			return false;
		}
		double operand1 = operands.top();
		operands.pop();
		double result = calculate(operand1, operand2, op);
		operands.push(result);
	}
	if (operands.empty()) {
		return false;
	}
	result = operands.top();
	return true;
}

int main() {
	string expr;
	cout << "input an expression: ";
	getline(cin, expr);
	double result;
	if (evaluate(expr, result)) {
		printf("%lf\n", result);
	} else {
		cout << "illegal expression!" << endl;
	}
    cout<<"press any key to continue...";
    getchar();
	return 0;
}
