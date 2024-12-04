

#ifndef booster_data_type_Object_hpp
#define booster_data_type_Object_hpp

#include "./Type.hpp"

#include "./Tree.hpp"
#include "./Any.hpp"
#include "./Primitive.hpp"
#include "./Enum.hpp"
#include "./UnorderedMap.hpp"
#include "./PairList.hpp"
#include "./List.hpp"
#include "./Vector.hpp"
#include "./UnorderedSet.hpp"

#include "booster/base/Countable.hpp"

#include <type_traits>

namespace booster { namespace data { namespace type {


class BaseObject : public booster::base::Countable {
public:


  class Property {
  public:


    class TypeSelector {
    public:


      virtual ~TypeSelector() = default;


      virtual const type::Type* selectType(BaseObject* self) = 0;
    };

    template<class DTOType>
    class FieldTypeSelector : public TypeSelector {
    public:

      const type::Type* selectType(BaseObject* self) override {
        return selectFieldType(static_cast<DTOType*>(self));
      }

      virtual const type::Type* selectFieldType(DTOType* self) = 0;

    };

  public:


    struct Info {


      std::string description = "";


      std::string pattern = "";


      bool required = false;


      TypeSelector* typeSelector = nullptr;

    };

  private:
    const v_int64 offset;
  public:


    Property(v_int64 pOffset, std::string pName, std::string pUName, const Type* pType);


    const std::string name;


    const std::string unqualifiedName;


    const Type* const type;


    Info info;


    void set(BaseObject* object, const Void& value) const;


    Void get(BaseObject* object) const;


    Void& getAsRef(BaseObject* object) const;

  };


  class Properties {
  private:
    std::unordered_map<std::string, Property*> m_map;
    std::unordered_map<std::string, Property*> m_unqualifiedMap;
    std::list<Property*> m_list;
  public:


    Property* pushBack(Property* property);


    void pushFrontAll(Properties* properties);


    const std::unordered_map<std::string, Property*>& getMap() const;


    const std::unordered_map<std::string, Property*>& getUnqualifiedMap() const;


    const std::list<Property*>& getList() const;

  };

private:
  void* m_basePointer = this;
private:
  void set(v_int64 offset, const Void& value);
  Void get(v_int64 offset) const;
  Void& getAsRef(v_int64 offset) const;
protected:
  void setBasePointer(void* basePointer);
  void* getBasePointer() const;
};

namespace __class {


  class AbstractObject {
  public:

    class PolymorphicDispatcher {
    public:
      
      virtual ~PolymorphicDispatcher() = default;

      virtual type::Void createObject() const = 0;

      virtual const type::BaseObject::Properties* getProperties() const = 0;

    };

  public:


    static const ClassId CLASS_ID;

  };


  template<class T>
  class Object : public AbstractObject {
  public:

    class PolymorphicDispatcher : public AbstractObject::PolymorphicDispatcher {
    public:

      type::Void createObject() const override {
        return type::Void(std::make_shared<T>(), getType());
      }

      const type::BaseObject::Properties* getProperties() const override {
        return propertiesGetter();
      }

    };

  private:

    static type::BaseObject::Properties* initProperties() {


      T obj;


      auto parentType = Object<typename T::Z__CLASS_EXTENDED>::getType();
      if(parentType->parent != nullptr) {
        auto dispatcher = static_cast<const AbstractObject::PolymorphicDispatcher*>(parentType->polymorphicDispatcher);
        dispatcher->getProperties();
      }


      T::Z__CLASS_EXTEND(T::Z__CLASS::Z__CLASS_GET_FIELDS_MAP(), T::Z__CLASS_EXTENDED::Z__CLASS_GET_FIELDS_MAP());

      return T::Z__CLASS::Z__CLASS_GET_FIELDS_MAP();

    }

    static const BaseObject::Properties* propertiesGetter() {
      static type::BaseObject::Properties* properties = initProperties();
      return properties;
    }

    static Type* createType() {
      Type::Info info;
      info.nameQualifier = T::Z__CLASS_TYPE_NAME();
      info.polymorphicDispatcher = new PolymorphicDispatcher();
      info.parent = T::getParentType();
      return new Type(CLASS_ID, info);
    }

  public:


    static Type* getType() {
      static Type* type = createType();
      return type;
    }
    
  };
  
}


template<class ObjT>
class DTOWrapper : public ObjectWrapper<ObjT, __class::Object<ObjT>> {
  template<class Type>
  friend class DTOWrapper;
public:
  typedef ObjT TemplateObjectType;
  typedef __class::Object<ObjT> TemplateObjectClass;
public:

  BOOSTER_DEFINE_OBJECT_WRAPPER_DEFAULTS(DTOWrapper, TemplateObjectType, TemplateObjectClass)

  template<class OtherT>
  DTOWrapper(const OtherT& other)
    : type::ObjectWrapper<ObjT, __class::Object<ObjT>>(other.m_ptr)
  {}

  template<class OtherT>
  DTOWrapper(OtherT&& other)
    : type::ObjectWrapper<ObjT, __class::Object<ObjT>>(std::move(other.m_ptr))
  {}

  static DTOWrapper createShared() {
    return std::make_shared<TemplateObjectType>();
  }

  template<class T>
  DTOWrapper& operator = (const DTOWrapper<T>& other) {
    this->m_ptr = other.m_ptr;
    return *this;
  }

  template<class T>
  DTOWrapper& operator = (DTOWrapper<T>&& other) {
    this->m_ptr = std::move(other.m_ptr);
    return *this;
  }

  template<typename T,
    typename enabled = typename std::enable_if<std::is_same<T, std::nullptr_t>::value, void>::type
  >
  inline bool operator == (T){
    return this->m_ptr.get() == nullptr;
  }

  template<typename T,
    typename enabled = typename std::enable_if<std::is_same<T, std::nullptr_t>::value, void>::type
  >
  inline bool operator != (T){
    return this->m_ptr.get() != nullptr;
  }

  template<typename T,
    typename enabled = typename std::enable_if<std::is_same<T, DTOWrapper>::value, void>::type
  >
  inline bool operator == (const T &other) const {
    if(this->m_ptr.get() == other.m_ptr.get()) return true;
    if(!this->m_ptr || !other.m_ptr) return false;
    return *this->m_ptr == *other.m_ptr;
  }

  template<typename T,
    typename enabled = typename std::enable_if<std::is_same<T, DTOWrapper>::value, void>::type
  >
  inline bool operator != (const T &other) const {
    return !operator == (other);
  }

  static const std::unordered_map<std::string, BaseObject::Property*>& getPropertiesMap() {
    auto dispatcher = static_cast<const __class::AbstractObject::PolymorphicDispatcher*>(
      __class::Object<ObjT>::getType()->polymorphicDispatcher
    );
    return dispatcher->getProperties()->getMap();
  }

  static const std::list<BaseObject::Property*>& getPropertiesList() {
    auto dispatcher = static_cast<const __class::AbstractObject::PolymorphicDispatcher*>(
      __class::Object<ObjT>::getType()->polymorphicDispatcher
    );
    return dispatcher->getProperties()->getList();
  }

  static v_int64 getPropertiesCount() {
    auto dispatcher = static_cast<const __class::AbstractObject::PolymorphicDispatcher*>(
      __class::Object<ObjT>::getType()->polymorphicDispatcher
    );
    return static_cast<v_int64>(dispatcher->getProperties()->getList().size());
  }

  ObjectWrapper<void>& operator[](const std::string& propertyName) {
    return getPropertiesMap().at(propertyName)->getAsRef(this->m_ptr.get());
  }

};


class DTO : public BaseObject {
  template<class T>
  friend class __class::Object;
private:
  typedef DTO Z__CLASS;
  typedef DTO Z__CLASS_EXTENDED;
public:
  typedef booster::data::type::Void Void;
  typedef booster::data::type::Any Any;
  typedef booster::data::type::Tree Tree;
  typedef booster::data::type::String String;
  typedef booster::data::type::Int8 Int8;
  typedef booster::data::type::UInt8 UInt8;
  typedef booster::data::type::Int16 Int16;
  typedef booster::data::type::UInt16 UInt16;
  typedef booster::data::type::Int32 Int32;
  typedef booster::data::type::UInt32 UInt32;
  typedef booster::data::type::Int64 Int64;
  typedef booster::data::type::UInt64 UInt64;
  typedef booster::data::type::Float32 Float32;
  typedef booster::data::type::Float64 Float64;
  typedef booster::data::type::Boolean Boolean;

  template <class T>
  using Object = DTOWrapper<T>;

  template <class T>
  using Enum = booster::data::type::Enum<T>;

  template <class T>
  using Vector = booster::data::type::Vector<T>;

  template <class T>
  using UnorderedSet = booster::data::type::UnorderedSet<T>;

  template <class T>
  using List = booster::data::type::List<T>;

  template <class Value>
  using Fields = booster::data::type::PairList<String, Value>;

  template <class Value>
  using UnorderedFields = booster::data::type::UnorderedMap<String, Value>;

private:
  static const type::Type* getParentType();
  static const char* Z__CLASS_TYPE_NAME();
  static data::type::BaseObject::Properties* Z__CLASS_GET_FIELDS_MAP();
  static BaseObject::Properties* Z__CLASS_EXTEND(BaseObject::Properties* properties, BaseObject::Properties* extensionProperties);
public:

  virtual v_uint64 defaultHashCode() const {
    return reinterpret_cast<v_uint64>(this);
  }

  virtual bool defaultEquals(const DTO& other) const {
    return this == &other;
  }

  v_uint64 hashCode() const {
    return defaultHashCode();
  }

  bool operator==(const DTO& other) const {
    return defaultEquals(other);
  }

};
  
}}}

namespace std {

  template<class T>
  struct hash<booster::data::type::DTOWrapper<T>> {

    typedef booster::data::type::DTOWrapper<T> argument_type;
    typedef v_uint64 result_type;

    result_type operator()(argument_type const &ow) const noexcept {
      if(ow) {
        return ow->hashCode();
      }
      return 0;
    }

  };

}

#endif
