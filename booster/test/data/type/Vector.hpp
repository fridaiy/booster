

#ifndef booster_data_type_Vector_hpp
#define booster_data_type_Vector_hpp

#include "./Collection.hpp"
#include "./Type.hpp"

#include <vector>
#include <initializer_list>

namespace booster { namespace data { namespace type {

namespace __class {


  class AbstractVector {
  public:

    static const ClassId CLASS_ID;
  };

  template<class T>
  class Vector;

}


template<class T, class C>
class VectorObjectWrapper : public type::ObjectWrapper<std::vector<T>, C> {
public:
  typedef std::vector<T> TemplateObjectType;
  typedef C TemplateObjectClass;
public:

  BOOSTER_DEFINE_OBJECT_WRAPPER_DEFAULTS(VectorObjectWrapper, TemplateObjectType, TemplateObjectClass)

  VectorObjectWrapper(std::initializer_list<T> ilist)
    : type::ObjectWrapper<TemplateObjectType, TemplateObjectClass>(std::make_shared<TemplateObjectType>(ilist))
  {}

  static VectorObjectWrapper createShared() {
    return std::make_shared<TemplateObjectType>();
  }

  VectorObjectWrapper& operator = (std::initializer_list<T> ilist) {
    this->m_ptr = std::make_shared<TemplateObjectType>(ilist);
    return *this;
  }

  T& operator[] (v_buff_usize index) const {
    return this->m_ptr->operator [] (index);
  }

  TemplateObjectType& operator*() const {
    return this->m_ptr.operator*();
  }

};


template<class T>
using Vector = VectorObjectWrapper<T, __class::Vector<T>>;

typedef Vector<Void> AbstractVector;

namespace __class {

  template<class T>
  class Vector : public AbstractVector {
  private:

    static Type createType() {
      Type::Info info;
      info.params.push_back(T::Class::getType());
      info.polymorphicDispatcher = new typename StandardCollection<std::vector<T>, T, Vector>::PolymorphicDispatcher();
      info.isCollection = true;
      return Type(__class::AbstractVector::CLASS_ID, info);
    }

  public:

    static Type* getType() {
      static Type type = createType();
      return &type;
    }

  };

}

}}}

#endif // booster_data_type_Vector_hpp
