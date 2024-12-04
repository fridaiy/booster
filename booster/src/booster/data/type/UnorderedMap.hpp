

#ifndef booster_data_type_UnorderedMap_hpp
#define booster_data_type_UnorderedMap_hpp

#include "./Map.hpp"
#include "./Type.hpp"

#include <unordered_map>
#include <initializer_list>
#include <utility>

namespace booster { namespace data { namespace type {

namespace __class {


  class AbstractUnorderedMap {
  public:

    static const ClassId CLASS_ID;
  };

  template<class Key, class Value>
  class UnorderedMap;

}


template<class Key, class Value, class C>
class UnorderedMapObjectWrapper : public type::ObjectWrapper<std::unordered_map<Key, Value>, C> {
public:
  typedef std::unordered_map<Key, Value> TemplateObjectType;
  typedef C TemplateObjectClass;
public:

  BOOSTER_DEFINE_OBJECT_WRAPPER_DEFAULTS(UnorderedMapObjectWrapper, TemplateObjectType, TemplateObjectClass)

  UnorderedMapObjectWrapper(std::initializer_list<std::pair<const Key, Value>> ilist)
    : type::ObjectWrapper<TemplateObjectType, TemplateObjectClass>(std::make_shared<TemplateObjectType>(ilist))
  {}

  static UnorderedMapObjectWrapper createShared() {
    return std::make_shared<TemplateObjectType>();
  }

  UnorderedMapObjectWrapper& operator = (std::initializer_list<std::pair<const Key, Value>> ilist) {
    this->m_ptr = std::make_shared<TemplateObjectType>(ilist);
    return *this;
  }

  Value& operator[] (const Key& key) const {
    return this->m_ptr->operator [] (key);
  }

  TemplateObjectType& operator*() const {
    return this->m_ptr.operator*();
  }

};


template<class Key, class Value>
using UnorderedMap = UnorderedMapObjectWrapper<Key, Value, __class::UnorderedMap<Key, Value>>;

namespace __class {

  template<class Key, class Value>
  class UnorderedMap : public AbstractUnorderedMap {
  private:

    static Type createType() {
      Type::Info info;
      info.params.push_back(Key::Class::getType());
      info.params.push_back(Value::Class::getType());
      info.polymorphicDispatcher =
        new typename __class::StandardMap<std::unordered_map<Key, Value>, Key, Value, UnorderedMap>::PolymorphicDispatcher();
      info.isMap = true;
      return Type(__class::AbstractUnorderedMap::CLASS_ID, info);
    }

  public:

    static Type* getType() {
      static Type type = createType();
      return &type;
    }

  };

}

}}}

#endif // booster_data_type_UnorderedMap_hpp
