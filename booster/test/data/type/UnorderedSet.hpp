

#ifndef booster_data_type_UnorderedSet_hpp
#define booster_data_type_UnorderedSet_hpp

#include "./Collection.hpp"
#include "./Type.hpp"

#include <unordered_set>
#include <initializer_list>

namespace booster { namespace data { namespace type {

namespace __class {


  class AbstractUnorderedSet {
  public:

    static const ClassId CLASS_ID;
  };

  template<class T>
  class UnorderedSet;

}


template<class T, class C>
class UnorderedSetObjectWrapper : public type::ObjectWrapper<std::unordered_set<T>, C> {
public:
  typedef std::unordered_set<T> TemplateObjectType;
  typedef C TemplateObjectClass;
public:

  BOOSTER_DEFINE_OBJECT_WRAPPER_DEFAULTS(UnorderedSetObjectWrapper, TemplateObjectType, TemplateObjectClass)

  UnorderedSetObjectWrapper(std::initializer_list<T> ilist)
    : type::ObjectWrapper<TemplateObjectType, TemplateObjectClass>(std::make_shared<TemplateObjectType>(ilist))
  {}

  static UnorderedSetObjectWrapper createShared() {
    return std::make_shared<TemplateObjectType>();
  }

  UnorderedSetObjectWrapper& operator = (std::initializer_list<T> ilist) {
    this->m_ptr = std::make_shared<TemplateObjectType>(ilist);
    return *this;
  }

  bool operator[] (const T& key) const {
    if(this->m_ptr) {
      auto it = this->m_ptr->find(key);
      return it != this->m_ptr->end();
    }
    return false;
  }

  TemplateObjectType& operator*() const {
    return this->m_ptr.operator*();
  }

};


template<class T>
using UnorderedSet = UnorderedSetObjectWrapper<T, __class::UnorderedSet<T>>;

typedef UnorderedSet<Void> AbstractUnorderedSet;

namespace __class {

template<class T>
class UnorderedSet : public AbstractUnorderedSet {
private:

  static Type createType() {
    Type::Info info;
    info.params.push_back(T::Class::getType());
    info.polymorphicDispatcher = new typename StandardCollection<std::unordered_set<T>, T, UnorderedSet>::PolymorphicDispatcher();
    info.isCollection = true;
    return Type(__class::AbstractUnorderedSet::CLASS_ID, info);
  }

public:

  static Type* getType() {
    static Type type = createType();
    return &type;
  }

};

}

}}}

#endif // booster_data_type_UnorderedSet_hpp
