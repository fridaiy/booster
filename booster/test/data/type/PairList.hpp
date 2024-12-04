

#ifndef booster_data_type_PairList_hpp
#define booster_data_type_PairList_hpp

#include "./Map.hpp"
#include "./Type.hpp"

#include <list>
#include <initializer_list>
#include <utility>

namespace booster { namespace data { namespace type {

namespace __class {


  class AbstractPairList {
  public:

    static const ClassId CLASS_ID;
  };

  template<class Key, class Value>
  class PairList;

}


template<class Key, class Value, class C>
class PairListObjectWrapper : public type::ObjectWrapper<std::list<std::pair<Key, Value>>, C> {
public:
  typedef std::list<std::pair<Key, Value>> TemplateObjectType;
  typedef C TemplateObjectClass;
public:

  BOOSTER_DEFINE_OBJECT_WRAPPER_DEFAULTS(PairListObjectWrapper, TemplateObjectType, TemplateObjectClass)

  PairListObjectWrapper(std::initializer_list<std::pair<Key, Value>> ilist)
    : type::ObjectWrapper<TemplateObjectType, TemplateObjectClass>(std::make_shared<TemplateObjectType>(ilist))
  {}

  static PairListObjectWrapper createShared() {
    return std::make_shared<TemplateObjectType>();
  }

  PairListObjectWrapper& operator = (std::initializer_list<std::pair<Key, Value>> ilist) {
    this->m_ptr = std::make_shared<TemplateObjectType>(ilist);
    return *this;
  }

  std::pair<Key, Value>& operator[] (v_buff_usize index) const {
    auto it = this->m_ptr->begin();
    std::advance(it, index);
    return *it;
  }
  
  Value& operator[] (const Key& key) const {
    auto& list = *(this->m_ptr.get());
    auto it = list.begin();
    while(it != list.end()) {
      if(it->first == key) {
        return it->second;
      }
      it ++;
    }
    list.push_back({key, nullptr});
    return list.back().second;
  }

  Value getValueByKey(const Key& key, const Value& defValue = nullptr) const {
    auto& list = *(this->m_ptr.get());
    auto it = list.begin();
    while(it != list.end()) {
      if(it->first == key) {
        return it->second;
      }
      it ++;
    }
    return defValue;
  }

  TemplateObjectType& operator*() const {
    return this->m_ptr.operator*();
  }

};


template<class Key, class Value>
using PairList = PairListObjectWrapper<Key, Value, __class::PairList<Key, Value>>;

namespace __class {

template<class Key, class Value>
class PairList : public AbstractPairList {
private:

  static Type createType() {
    Type::Info info;
    info.params.push_back(Key::Class::getType());
    info.params.push_back(Value::Class::getType());
    info.polymorphicDispatcher =
      new typename __class::StandardMap<std::list<std::pair<Key, Value>>, Key, Value, PairList>::PolymorphicDispatcher();
    info.isMap = true;
    return Type(__class::AbstractPairList::CLASS_ID, info);
  }

public:

  static Type* getType() {
    static Type type = createType();
    return &type;
  }

};

}

}}}

#endif // booster_data_type_PairList_hpp
