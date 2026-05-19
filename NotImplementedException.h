#include <stdexcept>
#include <string> 

class NotImplementedException : public std::logic_error
{
public:
  NotImplementedException(const std::string &methodName);
};