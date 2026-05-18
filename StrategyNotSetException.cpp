#include "StrategyNotSetException.h"

StrategyNotSetException::StrategyNotSetException(const std::string &strategyType)
    : std::runtime_error("No " + strategyType + " strategy set")
  {
  }