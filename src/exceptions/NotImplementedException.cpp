#include "include/exceptions/NotImplementedException.h"

class NotImplementedException : public std::logic_error
{
public:
  NotImplementedException(const std::string &methodName) : std::logic_error(methodName + " is not implemented")
  {
  }
};