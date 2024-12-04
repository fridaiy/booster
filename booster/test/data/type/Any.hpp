

#ifndef booster_data_type_Any_hpp
#define booster_data_type_Any_hpp

#include "./Type.hpp"

#include "booster/base/Countable.hpp"

namespace booster { namespace data { namespace type {

namespace __class {


  class Any {
  public:


    static const ClassId CLASS_ID;

    static Type *getType();

  };

}

class AnyHandle : public base::Countable {
public:

  AnyHandle(const std::shared_ptr<void>& objPtr, const Type* const objType)
    : ptr(objPtr)
    , type(objType)
  {}

  std::shared_ptr<void> ptr;
  const Type* const type;

};


class Any : public ObjectWrapper<AnyHandle, __class::Any>{
public:


  Any();


  Any(std::nullptr_t);


  Any(const Any& other);


  Any(Any&& other);

  Any(const std::shared_ptr<AnyHandle>& handle, const Type* const type);


  template<class T, class C>
  Any(const ObjectWrapper<T, C>& polymorph)
    : ObjectWrapper(std::make_shared<AnyHandle>(polymorph.getPtr(), polymorph.getValueType()), __class::Any::getType())
  {}


  template<class T, class C>
  void store(const ObjectWrapper<T, C>& polymorph) {
    m_ptr = std::make_shared<AnyHandle>(polymorph.getPtr(), polymorph.getValueType());
  }


  const Type* getStoredType() const;


  Void retrieve(const Type* type) const;


  template<class WrapperType>
  WrapperType retrieve() const {
    const auto& v = retrieve(WrapperType::Class::getType());
    return WrapperType(std::static_pointer_cast<typename WrapperType::ObjectType>(v.getPtr()), WrapperType::Class::getType());
  }

  Any& operator=(std::nullptr_t);

  Any& operator=(const Any& other);
  Any& operator=(Any&& other);

  template<class T, class C>
  Any& operator=(const ObjectWrapper<T, C>& polymorph) {
    m_ptr = std::make_shared<AnyHandle>(polymorph.getPtr(), polymorph.getValueType());
    return *this;
  }

  bool operator == (std::nullptr_t) const;
  bool operator != (std::nullptr_t) const;

  bool operator == (const Any& other) const;
  bool operator != (const Any& other) const;

};

}}}

#endif //booster_data_type_Any_hpp
