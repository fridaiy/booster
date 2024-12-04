
#include "Error.hpp"

namespace booster { namespace async {

Error::Error(const std::string& message)
  : m_message(message)
{
}

Error::Error(const std::exception_ptr& exceptionPtr)
  : m_message("exception_ptr")
  , m_exceptionPtr(exceptionPtr)
{
}

const std::string& Error::what() const {
  return m_message;
}

const std::exception_ptr& Error::getExceptionPtr() const {
  return m_exceptionPtr;
}

}}
