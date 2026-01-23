#include "RPN.hpp"
#include <iostream>
#include <sstream>

RPN::RPN() {}

RPN::RPN(const RPN &other) : rpnStack(other.rpnStack) {}

RPN &RPN::operator=(const RPN &other) {
    if (this != &other) {
        rpnStack = other.rpnStack;
    }
    return *this;
}

RPN::~RPN() {}

bool isOperator(std::string in) {
    if (in == "+" || in == "-" || in == "*" || in == "/")
        return true;
    return false;
}

void RPN::calculate(std::string input) {
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        if (isOperator(token)) {
            if (rpnStack.size() < 2) {
                throw std::runtime_error("Error");
            }
            int right = rpnStack.top();
            rpnStack.pop();
            int left = rpnStack.top();
            rpnStack.pop();
            switch (token[0]) {
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
        } else if (token.size() == 1 && std::isdigit(token[0])) {
            int number = std::atoi(token.c_str());
            rpnStack.push(number);
        } else {
            throw std::runtime_error("Error");
        }
    }
    if (rpnStack.size() != 1) {
        throw std::runtime_error("Error");
    }
    std::cout << rpnStack.top() << std::endl;
}
