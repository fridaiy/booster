

#ifndef booster_data_type_Type_hpp
#define booster_data_type_Type_hpp

#include "booster/base/Countable.hpp"
#include "booster/Environment.hpp"

#include <list>
#include <unordered_map>
#include <vector>
#include <string>

namespace booster { namespace data { namespace type {

class Type; // FWD


class ClassId {
private:
  static std::mutex& getClassMutex();
  static std::vector<const char*>& getClassNames();
  static v_int32 registerClassName(const char* name);
public:

  static int getClassCount();


  static std::vector<const char*> getRegisteredClassNames();
public:


  ClassId(const char* pName);


  const char* const name;


  const v_int32 id;

public:

  inline bool operator == (const ClassId& other) const {
    return id == other.id;
  }

  inline bool operator != (const ClassId& other) const {
    return id != other.id;
  }

};


namespace __class {

  class Void {
  public:

    static const ClassId CLASS_ID;


    static Type* getType();
  };

}

class Void; // FWD


template <class T, class Clazz = __class::Void>
class ObjectWrapper {
  friend Void;
  template <class Q, class W>
  friend class ObjectWrapper;
protected:
  static void checkType(const Type* _this, const Type* other);
protected:
  std::shared_ptr<T> m_ptr;
  const Type* m_valueType;
public:


  typedef T ObjectType;


  typedef Clazz Class;
public:

  ObjectWrapper(const std::shared_ptr<T>& ptr)
    : m_ptr(ptr)
    , m_valueType(Class::getType())
  {}

  ObjectWrapper(const std::shared_ptr<T>& ptr, const Type* const type)
    : m_ptr(ptr)
    , m_valueType(type)
  {}

  ObjectWrapper(std::shared_ptr<T>&& ptr, const Type* const type)
    : m_ptr(std::move(ptr))
    , m_valueType(type)
  {}
  
public:

  ObjectWrapper()
    : m_valueType(Class::getType())
  {}

  ObjectWrapper(std::nullptr_t)
    : m_valueType(Class::getType())
  {}

  ObjectWrapper(const Type* const type)
    : m_valueType(type)
  {}

  ObjectWrapper(const ObjectWrapper& other)
    : m_ptr(other.m_ptr)
    , m_valueType(other.m_valueType)
  {}

  ObjectWrapper(ObjectWrapper&& other)
    : m_ptr(std::move(other.m_ptr))
    , m_valueType(other.m_valueType)
  {}

  template <class Q, class W>
  ObjectWrapper(const ObjectWrapper<Q, W>& other)
    : m_ptr(other.m_ptr)
    , m_valueType(other.m_valueType)
  {}

  template <class Q, class W>
  ObjectWrapper(ObjectWrapper<Q, W>&& other)
    : m_ptr(std::move(other.m_ptr))
    , m_valueType(other.m_valueType)
  {}

  inline ObjectWrapper& operator=(const ObjectWrapper& other){
    checkType(m_valueType, other.m_valueType);
    m_ptr = other.m_ptr;
    return *this;
  }

  inline ObjectWrapper& operator=(ObjectWrapper&& other){
    checkType(m_valueType, other.m_valueType);
    m_ptr = std::move(other.m_ptr);
    return *this;
  }

  template <class Q, class W>
  inline ObjectWrapper& operator=(const ObjectWrapper<Q, W>& other){
    checkType(m_valueType, other.m_valueType);
    m_ptr = other.m_ptr;
    return *this;
  }

  template <class Q, class W>
  inline ObjectWrapper& operator=(ObjectWrapper<Q, W>&& other){
    checkType(m_valueType, other.m_valueType);
    m_ptr = std::move(other.m_ptr);
    return *this;
  }

  template<class Wrapper>
  Wrapper cast() const;

  inline T* operator->() const {
    return m_ptr.operator->();
  }
  
  T* get() const {
    return m_ptr.get();
  }
  
  void resetPtr(const std::shared_ptr<T>& ptr = nullptr) {
    m_ptr = ptr;
  }
  
  std::shared_ptr<T> getPtr() const {
    return m_ptr;
  }

  inline bool operator == (std::nullptr_t) const {
    return m_ptr.get() == nullptr;
  }

  inline bool operator != (std::nullptr_t) const {
    return m_ptr.get() != nullptr;
  }
  
  inline bool operator == (const ObjectWrapper& other) const {
    return m_ptr.get() == other.m_ptr.get();
  }
  
  inline bool operator != (const ObjectWrapper& other) const {
    return m_ptr.get() != other.m_ptr.get();
  }
  
  explicit inline operator bool() const {
    return m_ptr.operator bool();
  }


  const Type* getValueType() const {
    return m_valueType;
  }
  
};

class Void : public ObjectWrapper<void, __class::Void> {
public:
  Void(const std::shared_ptr<void>& ptr, const type::Type* const valueType)
    : ObjectWrapper<void, __class::Void>(ptr, valueType)
  {}
public:

  Void() {}

  template<typename T,
    typename enabled = typename std::enable_if<std::is_same<T, std::nullptr_t>::value, void>::type
  >
  Void(T) {}

  Void(const Type* const type)
    : ObjectWrapper<void, __class::Void>(type)
  {}

  Void(const std::shared_ptr<void>& ptr)
    : type::ObjectWrapper<void, __class::Void>(ptr)
  {}

  Void(std::shared_ptr<void>&& ptr)
    : type::ObjectWrapper<void, __class::Void>(std::forward<std::shared_ptr<void>>(ptr))
  {}

  Void(const Void& other)
    : type::ObjectWrapper<void, __class::Void>(other.getPtr(), other.getValueType())
  {}

  Void(Void&& other)
    : type::ObjectWrapper<void, __class::Void>(std::move(other.getPtr()), other.getValueType())
  {}

  template<typename T, typename C>
  Void(const ObjectWrapper<T, C>& other)
    : type::ObjectWrapper<void, __class::Void>(other.getPtr(), other.getValueType())
  {}

  template<typename T, typename C>
  Void(ObjectWrapper<T, C>&& other)
    : type::ObjectWrapper<void, __class::Void>(std::move(other.getPtr()), other.getValueType())
  {}

  template<typename T,
    typename enabled = typename std::enable_if<std::is_same<T, std::nullptr_t>::value, void>::type
  >
  inline Void& operator = (std::nullptr_t) {
    m_ptr.reset();
    return *this;
  }

  inline Void& operator = (const Void& other){
    m_ptr = other.m_ptr;
    m_valueType = other.getValueType();
    return *this;
  }

  inline Void& operator = (Void&& other){
    m_ptr = std::move(other.m_ptr);
    m_valueType = other.getValueType();
    return *this;
  }

  template<typename T, typename C>
  inline Void& operator = (const ObjectWrapper<T, C>& other){
    m_ptr = other.m_ptr;
    m_valueType = other.getValueType();
    return *this;
  }

  template<typename T, typename C>
  inline Void& operator = (ObjectWrapper<T, C>&& other){
    m_ptr = std::move(other.m_ptr);
    m_valueType = other.getValueType();
    return *this;
  }

  template<typename T,
    typename enabled = typename std::enable_if<std::is_same<T, std::nullptr_t>::value, void>::type
  >
  inline bool operator == (T) const {
    return m_ptr.get() == nullptr;
  }

  template<typename T,
    typename enabled = typename std::enable_if<std::is_same<T, std::nullptr_t>::value, void>::type
  >
  inline bool operator != (T) const {
    return m_ptr.get() != nullptr;
  }

  template<typename T, typename C>
  inline bool operator == (const ObjectWrapper<T, C> &other) const {
    return m_ptr.get() == other.get();
  }

  template<typename T, typename C>
  inline bool operator != (const ObjectWrapper<T, C> &other) const {
    return m_ptr.get() != other.get();
  }
};

template <typename T>
struct ObjectWrapperByUnderlyingType {};


class Type {
public:


  class AbstractInterpretation {
  public:

    virtual ~AbstractInterpretation() = default;


    virtual Void toInterpretation(const Void& originalValue) const = 0;


    virtual Void fromInterpretation(const Void& interValue) const = 0;


    virtual const Type* getInterpretationType() const = 0;
  };

  template<class OriginalWrapper, class InterWrapper>
  class Interpretation : public AbstractInterpretation {
  public:

    Void toInterpretation(const Void& originalValue) const override {
      return interpret(originalValue.template cast<OriginalWrapper>());
    }

    Void fromInterpretation(const Void& interValue) const override {
      return reproduce(interValue.template cast<InterWrapper>());
    }

    const Type* getInterpretationType() const override {
      return InterWrapper::Class::getType();
    }

  public:

    virtual InterWrapper interpret(const OriginalWrapper& value) const = 0;
    virtual OriginalWrapper reproduce(const InterWrapper& value) const = 0;

  };

  typedef std::unordered_map<std::string, const AbstractInterpretation*> InterpretationMap;

public:


  struct Info {


    Info() {}


    const char* nameQualifier = nullptr;


    std::vector<const Type*> params;


    void* polymorphicDispatcher = nullptr;


    InterpretationMap interpretationMap;


    const Type* parent = nullptr;


     bool isCollection = false;


     bool isMap = false;
  };

public:


  Type(const ClassId& pClassId, const Info& typeInfo = Info());


  const ClassId classId;


  const char* const nameQualifier;


  const std::vector<const Type*> params;


  const void* const polymorphicDispatcher;


  const InterpretationMap interpretationMap;


  const Type* const parent;


  const bool isCollection;


  const bool isMap;

public:


  const AbstractInterpretation* findInterpretation(const std::vector<std::string>& names) const;


  bool extends(const Type* other) const;
  
};

template <class T, class Clazz>
template<class Wrapper>
Wrapper ObjectWrapper<T, Clazz>::cast() const {
  if(!Wrapper::Class::getType()->extends(m_valueType)) {
    if(Wrapper::Class::getType() != __class::Void::getType() && m_valueType != __class::Void::getType()) {
      throw std::runtime_error("[booster::data::type::ObjectWrapper::cast()]: Error. Invalid cast "
                               "from '" + std::string(m_valueType->classId.name) + "' to '" +
                               std::string(Wrapper::Class::getType()->classId.name) + "'.");
    }
  }
  return Wrapper(std::static_pointer_cast<typename Wrapper::ObjectType>(m_ptr), Wrapper::Class::getType());
}

template <class T, class Clazz>
void ObjectWrapper<T, Clazz>::checkType(const Type* _this, const Type* other) {
  if(!_this->extends(other)) {
    throw std::runtime_error("[booster::data::type::ObjectWrapper::checkType()]: Error. "
                             "Type mismatch: stored '" + std::string(_this->classId.name) + "' vs "
                             "assigned '" + std::string(other->classId.name) + "'.");
  }
}

#define BOOSTER_DEFINE_OBJECT_WRAPPER_DEFAULTS(WRAPPER_NAME, OBJECT_TYPE, OBJECT_CLASS) \
public: \
  WRAPPER_NAME(const std::shared_ptr<OBJECT_TYPE>& ptr, const type::Type* const valueType) \
    : type::ObjectWrapper<OBJECT_TYPE, OBJECT_CLASS>(ptr, valueType) \
  {} \
public: \
\
  WRAPPER_NAME() {} \
\
  WRAPPER_NAME(std::nullptr_t) {} \
\
  WRAPPER_NAME(const std::shared_ptr<OBJECT_TYPE>& ptr) \
    : type::ObjectWrapper<OBJECT_TYPE, OBJECT_CLASS>(ptr) \
  {} \
\
  WRAPPER_NAME(std::shared_ptr<OBJECT_TYPE>&& ptr) \
    : type::ObjectWrapper<OBJECT_TYPE, OBJECT_CLASS>(std::forward<std::shared_ptr<OBJECT_TYPE>>(ptr)) \
  {} \
\
  WRAPPER_NAME(const WRAPPER_NAME& other) \
    : type::ObjectWrapper<OBJECT_TYPE, OBJECT_CLASS>(other) \
  {} \
\
  WRAPPER_NAME(WRAPPER_NAME&& other) \
    : type::ObjectWrapper<OBJECT_TYPE, OBJECT_CLASS>(std::forward<WRAPPER_NAME>(other)) \
  {} \
\
  inline WRAPPER_NAME& operator = (std::nullptr_t) { \
    this->m_ptr.reset(); \
    return *this; \
  } \
\
  inline WRAPPER_NAME& operator = (const WRAPPER_NAME& other) { \
    this->m_ptr = other.m_ptr; \
    return *this; \
  } \
\
  inline WRAPPER_NAME& operator = (WRAPPER_NAME&& other) { \
    this->m_ptr = std::move(other.m_ptr); \
    return *this; \
  } \


}}}

namespace std {

template<>
struct hash<booster::data::type::ClassId> {

  typedef booster::data::type::ClassId argument_type;
  typedef v_uint64 result_type;

  result_type operator()(argument_type const& v) const noexcept {
    return static_cast<result_type>(v.id);
  }

};

template<>
struct hash<booster::data::type::Void> {

  typedef booster::data::type::Void argument_type;
  typedef v_uint64 result_type;

  result_type operator()(argument_type const& v) const noexcept {
    return reinterpret_cast<result_type>(v.get());
  }

};

}
  
#endif
