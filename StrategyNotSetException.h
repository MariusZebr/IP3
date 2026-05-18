#ifndef STRATEGYNOTSETEXCEPTION_H_2074525
#define STRATEGYNOTSETEXCEPTION_H_2074525

#include <stdexcept>
#include <string>

class StrategyNotSetException : public std::runtime_error
{
public:
  StrategyNotSetException(const std::string &strategyType);
};

#endif // STRATEGYNOTSETEXCEPTION_H_2074525