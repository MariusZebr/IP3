#include "../../include/exceptions/NotImplementedException.h"

NotImplementedException::NotImplementedException(const std::string &methodName) : std::logic_error(methodName + " is not implemented")
{
}