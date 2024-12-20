

#include "Any.hpp"

namespace booster { namespace data { namespace type {

namespace __class {
  const ClassId Any::CLASS_ID("Any");

  Type* Any::getType() {
    static Type type(CLASS_ID);
    return &type;
  }
}

Any::Any()
  : ObjectWrapper(__class::Any::getType())
{}

Any::Any(std::nullptr_t) : Any() {}

Any::Any(const std::shared_ptr<AnyHandle>& handle, const Type* const type)
  : ObjectWrapper(handle, __class::Any::getType())
{
  // As an ObjectWrapper, Any must have this constructor. It is used in ObjectWrapper.cast<T>(...) method.
  // However, unlike other ObjectWrappers Any won't change its valueType.
  // Any is always object wrapper above AnyHandler.
  (void)type;
}

Any::Any(const Any& other)
  : ObjectWrapper(
      (other.m_ptr) ? std::make_shared<AnyHandle>(other.m_ptr->ptr, other.m_ptr->type) : nullptr,
      __class::Any::getType()
    )
{}

Any::Any(Any&& other)
  : ObjectWrapper(std::move(other.m_ptr),  __class::Any::getType())
{}

const Type* Any::getStoredType() const {
  if(m_ptr) {
    return m_ptr->type;
  }
  return nullptr;
}

Void Any::retrieve(const Type* type) const {
  if(m_ptr) {
    if(!m_ptr->type->extends(type)) {
      throw std::runtime_error("[booster::data::type::Any::retrieve()]: Error. The value type doesn't match.");
    }
    return Void(m_ptr->ptr, type);
  }
  return Void(nullptr, type);
}

Any& Any::operator=(std::nullptr_t) {
  m_ptr.reset();
  return *this;
}

Any& Any::operator=(const Any& other) {
  if(other) {
    m_ptr = std::make_shared<AnyHandle>(other.m_ptr->ptr, other.m_ptr->type);
  } else {
    m_ptr.reset();
  }
  return *this;
}

Any& Any::operator=(Any&& other) {
  m_ptr = std::move(other.m_ptr);
  return *this;
}

bool Any::operator == (std::nullptr_t) const {
  return m_ptr == nullptr || m_ptr->ptr == nullptr;
}

bool Any::operator != (std::nullptr_t) const {
  return !operator == (nullptr);
}

bool Any::operator == (const Any& other) const {
  if(!m_ptr && !other.m_ptr) return true;
  if(!m_ptr || !other.m_ptr) return false;
  return m_ptr->ptr.get() == other.m_ptr->ptr.get();
}

bool Any::operator != (const Any& other) const {
  return !operator == (other);
}

}}}
