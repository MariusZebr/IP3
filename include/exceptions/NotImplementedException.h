#ifndef NOT_IMPLEMENTED_EXCEPTION_H_123456
#define NOT_IMPLEMENTED_EXCEPTION_H_123456

#include <stdexcept>
#include <string> 

class NotImplementedException : public std::logic_error
{
public:
  NotImplementedException(const std::string &methodName);
};

#endif // NOT_IMPLEMENTED_EXCEPTION_H_123456