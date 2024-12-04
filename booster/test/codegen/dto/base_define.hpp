

// Defaults


#define DTO_INIT(TYPE_NAME, TYPE_EXTEND) \
  template<class __Z__T__PARAM> \
  friend class booster::data::type::__class::Object; \
public: \
  typedef TYPE_NAME Z__CLASS; \
  typedef TYPE_EXTEND Z__CLASS_EXTENDED; \
  typedef booster::data::type::DTOWrapper<Z__CLASS> Wrapper; \
private: \
\
  static const booster::Type* getParentType() { \
    return booster::Object<Z__CLASS_EXTENDED>::Class::getType(); \
  } \
\
  static const char* Z__CLASS_TYPE_NAME() { \
    return #TYPE_NAME; \
  } \
\
  static booster::data::type::BaseObject::Properties* Z__CLASS_GET_FIELDS_MAP(){ \
    static booster::data::type::BaseObject::Properties map = booster::data::type::BaseObject::Properties(); \
    return &map; \
  } \
\
public: \
\
  template<typename ... Args> \
  static Wrapper createShared(Args... args){ \
    return Wrapper(std::make_shared<Z__CLASS>(args...), Wrapper::Class::getType()); \
  }

// Fields

#define BOOSTER_MACRO_DTO_FIELD_1(TYPE, NAME) \
\
static v_int64 Z__PROPERTY_OFFSET_##NAME() { \
  char buffer[sizeof(Z__CLASS)]; \
  auto obj = static_cast<Z__CLASS*>(reinterpret_cast<void*>(buffer)); \
  auto ptr = &obj->NAME; \
  return reinterpret_cast<v_int64>(ptr) - reinterpret_cast<v_int64>(buffer); \
} \
\
static booster::data::type::BaseObject::Property* Z__PROPERTY_SINGLETON_##NAME() { \
  static booster::data::type::BaseObject::Property* property = \
      new booster::data::type::BaseObject::Property(Z__PROPERTY_OFFSET_##NAME(), \
                                                  #NAME, \
                                                  #NAME, \
                                                  TYPE::Class::getType()); \
  return property; \
} \
\
static bool Z__PROPERTY_INIT_##NAME(... ) { \
  Z__CLASS_GET_FIELDS_MAP()->pushBack(Z__PROPERTY_SINGLETON_##NAME()); \
  return true; \
} \
\
static TYPE Z__PROPERTY_INITIALIZER_PROXY_##NAME() { \
  static bool initialized = Z__PROPERTY_INIT_##NAME(1 ,           \
                                                    1 ); \
  (void)initialized; \
  return TYPE(); \
} \
\
TYPE NAME = Z__PROPERTY_INITIALIZER_PROXY_##NAME()

#define BOOSTER_MACRO_DTO_FIELD_2(TYPE, NAME, QUALIFIER) \
\
static v_int64 Z__PROPERTY_OFFSET_##NAME() { \
  char buffer[sizeof(Z__CLASS)]; \
  auto obj = static_cast<Z__CLASS*>(reinterpret_cast<void*>(buffer)); \
  auto ptr = &obj->NAME; \
  return reinterpret_cast<v_int64>(ptr) - reinterpret_cast<v_int64>(buffer); \
} \
\
static booster::data::type::BaseObject::Property* Z__PROPERTY_SINGLETON_##NAME() { \
  static booster::data::type::BaseObject::Property* property = \
      new booster::data::type::BaseObject::Property(Z__PROPERTY_OFFSET_##NAME(), \
                                                  QUALIFIER, \
                                                  #NAME, \
                                                  TYPE::Class::getType()); \
  return property; \
} \
\
static bool Z__PROPERTY_INIT_##NAME(... ) { \
  Z__CLASS_GET_FIELDS_MAP()->pushBack(Z__PROPERTY_SINGLETON_##NAME()); \
  return true; \
} \
\
static TYPE Z__PROPERTY_INITIALIZER_PROXY_##NAME() { \
  static bool initialized = Z__PROPERTY_INIT_##NAME(1 ,           \
                                                    1 ); \
  (void)initialized; \
  return TYPE(); \
} \
\
TYPE NAME = Z__PROPERTY_INITIALIZER_PROXY_##NAME()


#define DTO_FIELD(TYPE, ...) \
BOOSTER_MACRO_EXPAND(BOOSTER_MACRO_MACRO_SELECTOR(BOOSTER_MACRO_DTO_FIELD_, (__VA_ARGS__)) (TYPE, __VA_ARGS__))

// DTO_FIELD_INFO

#define DTO_FIELD_INFO(NAME) \
\
static bool Z__PROPERTY_INIT_##NAME(int, ...) { \
  Z__PROPERTY_INIT_##NAME();  \
  Z__PROPERTY_ADD_INFO_##NAME(&Z__PROPERTY_SINGLETON_##NAME()->info); \
  return true; \
} \
\
static void Z__PROPERTY_ADD_INFO_##NAME(booster::data::type::BaseObject::Property::Info* info)


#define DTO_FIELD_TYPE_SELECTOR(NAME) \
\
class Z__PROPERTY_TYPE_SELECTOR_##NAME : public booster::BaseObject::Property::FieldTypeSelector<Z__CLASS> { \
public: \
  const booster::Type* selectFieldType(Z__CLASS* self) override { \
    return self->Z__PROPERTY_TYPE_SELECTOR_METHOD_##NAME(); \
  } \
}; \
\
static bool Z__PROPERTY_INIT_##NAME(int, int) { \
  Z__PROPERTY_INIT_##NAME(1);  \
  Z__PROPERTY_SINGLETON_##NAME()->info.typeSelector = new Z__PROPERTY_TYPE_SELECTOR_##NAME(); \
  return true; \
} \
\
const booster::Type* Z__PROPERTY_TYPE_SELECTOR_METHOD_##NAME()

// FOR EACH

#define BOOSTER_MACRO_DTO_HC_EQ_PARAM_HC(INDEX, COUNT, X) \
result = ((result << 5) - result) + std::hash<decltype(X)>{}(X);

#define BOOSTER_MACRO_DTO_HC_EQ_PARAM_EQ(INDEX, COUNT, X) \
&& X == other.X

#define DTO_HASHCODE_AND_EQUALS(...) \
v_uint64 defaultHashCode() const override { \
  return 1; \
} \
\
bool defaultEquals(const DTO&) const override { \
  return true; \
} \
\
v_uint64 hashCode() const { \
  v_uint64 result = 1; \
  result = ((result << 5) - result) + static_cast<const Z__CLASS_EXTENDED&>(*this).hashCode(); \
  BOOSTER_MACRO_FOREACH(BOOSTER_MACRO_DTO_HC_EQ_PARAM_HC, __VA_ARGS__) \
  return result; \
} \
\
bool operator==(const Z__CLASS& other) const { \
  return static_cast<const Z__CLASS_EXTENDED&>(*this) == static_cast<const Z__CLASS_EXTENDED&>(other) \
  BOOSTER_MACRO_FOREACH(BOOSTER_MACRO_DTO_HC_EQ_PARAM_EQ, __VA_ARGS__) \
  ; \
} \
\
bool operator!=(const Z__CLASS& other) const { \
  return !this->operator==(other); \
}


#define DTO_HC_EQ(...) DTO_HASHCODE_AND_EQUALS(__VA_ARGS__)
