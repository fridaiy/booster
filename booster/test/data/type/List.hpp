

#ifndef booster_data_type_List_hpp
#define booster_data_type_List_hpp

#include "./Collection.hpp"
#include "./Type.hpp"

#include <list>
#include <initializer_list>

namespace booster { namespace data { namespace type {

namespace __class {


  class AbstractList {
  public:


    static const ClassId CLASS_ID;

  };

  template<class T>
  class List;

}


template<class T, class C>
class ListObjectWrapper : public type::ObjectWrapper<std::list<T>, C> {
public:
  typedef std::list<T> TemplateObjectType;
  typedef C TemplateObjectClass;
public:

  BOOSTER_DEFINE_OBJECT_WRAPPER_DEFAULTS(ListObjectWrapper, TemplateObjectType, TemplateObjectClass)

  ListObjectWrapper(std::initializer_list<T> ilist)
    : type::ObjectWrapper<TemplateObjectType, TemplateObjectClass>(std::make_shared<TemplateObjectType>(ilist))
  {}

  static ListObjectWrapper createShared() {
    return std::make_shared<TemplateObjectType>();
  }

  ListObjectWrapper& operator = (std::initializer_list<T> ilist) {
    this->m_ptr = std::make_shared<TemplateObjectType>(ilist);
    return *this;
  }

  T& operator[] (v_buff_usize index) const {
    auto it = this->m_ptr->begin();
    std::advance(it, index);
    return *it;
  }

  TemplateObjectType& operator*() const {
    return this->m_ptr.operator*();
  }

};


template<class T>
using List = ListObjectWrapper<T, __class::List<T>>;

typedef List<Void> AbstractList;

namespace __class {

  template<class T>
  class List : public AbstractList {
  private:

    static Type createType() {
      Type::Info info;
      info.params.push_back(T::Class::getType());
      info.polymorphicDispatcher = new typename StandardCollection<std::list<T>, T, List>::PolymorphicDispatcher();
      info.isCollection = true;
      return Type(__class::AbstractList::CLASS_ID, info);
    }

  public:

    static Type* getType() {
      static Type type = createType();
      return &type;
    }

  };

}

}}}

#endif // booster_data_type_List_hpp
