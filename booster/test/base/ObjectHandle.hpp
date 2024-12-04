

#ifndef booster_base_ObjectHandle_hpp
#define booster_base_ObjectHandle_hpp

#include "booster/Environment.hpp"

namespace booster { namespace base {

template<class T>
class ObjectHandle {
private:
  T* m_object;
  std::shared_ptr<T> m_ptr;
public:

  ObjectHandle(T* object)
    : m_object(object)
  {}

  template<class Q>
  ObjectHandle(const std::shared_ptr<Q>& sharedObject)
    : m_object(sharedObject.get())
    , m_ptr(sharedObject)
  {}

  std::shared_ptr<T> getPtr() const {
    return m_ptr;
  }

  T* get() const {
    return m_object;
  }

  T* operator->() const {
    return m_object;
  }

  explicit operator bool() const {
    return m_object != nullptr;
  }

};

}}

#endif //booster_base_ObjectHandle_hpp
