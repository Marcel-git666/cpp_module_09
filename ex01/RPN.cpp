#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN &other) : rpnStack(other.rpnStack) {}

RPN &RPN::operator=(const RPN &other) {
    if (this != &other) {
        rpnStack = other.rpnStack;
    }
    return *this;
}

RPN::~RPN() {}
