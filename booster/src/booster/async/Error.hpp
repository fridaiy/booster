

#ifndef booster_async_Error_hpp
#define booster_async_Error_hpp

#include "booster/base/Countable.hpp"

namespace booster { namespace async {


class Error : public booster::base::Countable {
private:
  std::string m_message;
  std::exception_ptr m_exceptionPtr;
public:

  explicit Error(const std::string& message);
  explicit Error(const std::exception_ptr& exceptionPtr);

  const std::exception_ptr& getExceptionPtr() const;

  const std::string& what() const;
  template<class ErrorClass>
  bool is() const {
    return dynamic_cast<const ErrorClass*>(this) != nullptr;
  }

};

}}


#endif