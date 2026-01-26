#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <stack>

RPN::RPN() {}

RPN::RPN(const RPN &other) { (void)other; }

RPN &RPN::operator=(const RPN &other) {
    (void)other;
    return *this;
}

RPN::~RPN() {}

void RPN::performOp(char op, std::stack<int> &rpnStack) {
    if (rpnStack.size() < 2) {
        throw std::runtime_error("Error");
    }
    int right = rpnStack.top();
    rpnStack.pop();
    int left = rpnStack.top();
    rpnStack.pop();

    switch (op) {
    case '+':
        rpnStack.push(left + right);
        break;
    case '-':
        rpnStack.push(left - right);
        break;
    case '*':
        rpnStack.push(left * right);
        break;
    case '/':
        if (right == 0)
            throw std::runtime_error("Error");
        rpnStack.push(left / right);
        break;
    default:
        throw std::runtime_error("Error");
    }
}

void RPN::calculate(std::string input) {
    std::stringstream ss(input);
    std::string token;
    std::stack<int> rpnStack;
    while (ss >> token) {
        if (token.size() != 1) {
            throw std::runtime_error("Error");
        }
        if (std::isdigit(token[0])) {
            rpnStack.push(token[0] - '0');
        } else {
            performOp(token[0], rpnStack);
        }
    }
    if (rpnStack.size() != 1) {
        throw std::runtime_error("Error");
    }
    std::cout << rpnStack.top() << std::endl;
}
