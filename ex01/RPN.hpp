#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN {
  private:
    std::stack<int> rpnStack;

  public:
    // Orthodox Canonical Form
    RPN();
    RPN(const RPN &other);
    RPN &operator=(const RPN &other);
    ~RPN();

    void calculate(std::string input);
};

#endif
