


#define BOOSTER_MACRO_DTO_ENUM_PARAM_MACRO(MACRO, NAME, PARAM_LIST) MACRO(NAME, PARAM_LIST)
#define BOOSTER_MACRO_DTO_ENUM_PARAM_NAME(MACRO, NAME, PARAM_LIST) NAME
#define BOOSTER_MACRO_DTO_ENUM_PARAM_NAME_STR(MACRO, NAME, PARAM_LIST) #NAME
#define BOOSTER_MACRO_DTO_ENUM_PARAM_VALUE(MACRO, NAME, PARAM_LIST) BOOSTER_MACRO_FIRSTARG PARAM_LIST
#define BOOSTER_MACRO_DTO_ENUM_PARAM_VALUE_STR(MACRO, NAME, PARAM_LIST) BOOSTER_MACRO_FIRSTARG_STR PARAM_LIST
#define BOOSTER_MACRO_DTO_ENUM_PARAM(MACRO, NAME, PARAM_LIST) (MACRO, NAME, PARAM_LIST)


#define VALUE(NAME, ...) \
BOOSTER_MACRO_DTO_ENUM_PARAM(BOOSTER_MACRO_DTO_ENUM_VALUE, NAME, (__VA_ARGS__))

//////////////////////////////////////////////////////////////////////////

#define BOOSTER_MACRO_DTO_ENUM_MACRO_SELECTOR(MACRO, NAME, ...) \
BOOSTER_MACRO_EXPAND(BOOSTER_MACRO_MACRO_SELECTOR(MACRO, (__VA_ARGS__)) (NAME, __VA_ARGS__))

//////////////////////////////////////////////////////////////////////////

// VALUE MACRO

#define BOOSTER_MACRO_DTO_ENUM_VALUE_1(NAME, VAL) \
{ \
  booster::data::type::EnumValueInfo<EnumType> entry = {EnumType::NAME, index ++, #NAME, #NAME, nullptr}; \
  info.byName.insert({#NAME, entry}); \
  info.byUnqualifiedName.insert({#NAME, entry}); \
  info.byValue.insert({static_cast<v_uint64>(EnumType::NAME), entry}); \
  info.byIndex.push_back(entry); \
}

#define BOOSTER_MACRO_DTO_ENUM_VALUE_2(NAME, VAL, QUALIFIER) \
{ \
  booster::data::type::EnumValueInfo<EnumType> entry = {EnumType::NAME, index ++, QUALIFIER, #NAME, nullptr}; \
  info.byName.insert({QUALIFIER, entry}); \
  info.byUnqualifiedName.insert({#NAME, entry}); \
  info.byValue.insert({static_cast<v_uint64>(EnumType::NAME), entry}); \
  info.byIndex.push_back(entry); \
}

#define BOOSTER_MACRO_DTO_ENUM_VALUE_3(NAME, VAL, QUALIFIER, DESCRIPTION) \
{ \
  booster::data::type::EnumValueInfo<EnumType> entry = {EnumType::NAME, index ++, QUALIFIER, #NAME, DESCRIPTION}; \
  info.byName.insert({QUALIFIER, entry}); \
  info.byUnqualifiedName.insert({#NAME, entry}); \
  info.byValue.insert({static_cast<v_uint64>(EnumType::NAME), entry}); \
  info.byIndex.push_back(entry); \
}

#define BOOSTER_MACRO_DTO_ENUM_VALUE(NAME, PARAM_LIST) \
BOOSTER_MACRO_DTO_ENUM_MACRO_SELECTOR(BOOSTER_MACRO_DTO_ENUM_VALUE_, NAME, BOOSTER_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

// FOR EACH

#define BOOSTER_MACRO_DTO_ENUM_PARAM_DECL_FIRST(INDEX, COUNT, X) \
BOOSTER_MACRO_DTO_ENUM_PARAM_NAME X = BOOSTER_MACRO_DTO_ENUM_PARAM_VALUE X

#define BOOSTER_MACRO_DTO_ENUM_PARAM_DECL_REST(INDEX, COUNT, X) \
, BOOSTER_MACRO_DTO_ENUM_PARAM_NAME X = BOOSTER_MACRO_DTO_ENUM_PARAM_VALUE X

#define BOOSTER_MACRO_DTO_ENUM_PARAM_PUT(INDEX, COUNT, X) \
BOOSTER_MACRO_DTO_ENUM_PARAM_MACRO X

// ENUM MACRO

#define BOOSTER_ENUM_0(NAME, ORDINAL_TYPE) \
enum class NAME : ORDINAL_TYPE {}; \
\
namespace { \
\
class Z__BOOSTER_ENUM_META_##NAME : public booster::data::type::EnumMeta<NAME> { \
private: \
\
  static bool init() { \
    auto& info = *EnumMeta<NAME>::getInfo(); \
    v_int32 index = 0; \
    (void)index; \
    info.nameQualifier = #NAME; \
    return true; \
  } \
\
public: \
\
  static bool initializer() { \
    static bool initialized = init(); \
    return initialized; \
  } \
\
}; \
\
bool Z__BOOSTER_ENUM_META_INITIALIZER_##NAME = Z__BOOSTER_ENUM_META_##NAME::initializer(); \
\
}

#define BOOSTER_ENUM_1(NAME, ORDINAL_TYPE, ...) \
enum class NAME : ORDINAL_TYPE { \
  BOOSTER_MACRO_FOREACH_FIRST_AND_REST( \
    BOOSTER_MACRO_DTO_ENUM_PARAM_DECL_FIRST, \
    BOOSTER_MACRO_DTO_ENUM_PARAM_DECL_REST, \
    __VA_ARGS__ \
  ) \
}; \
\
class Z__BOOSTER_ENUM_META_##NAME : public booster::data::type::EnumMeta<NAME> { \
private: \
\
  static bool init() { \
    auto& info = *EnumMeta<NAME>::getInfo(); \
    v_int32 index = 0; \
    info.nameQualifier = #NAME; \
    BOOSTER_MACRO_FOREACH(BOOSTER_MACRO_DTO_ENUM_PARAM_PUT, __VA_ARGS__) \
    return true; \
  } \
\
public: \
\
  static bool initializer() { \
    static bool initialized = init(); \
    return initialized; \
  } \
\
}; \
\
static bool Z__BOOSTER_ENUM_META_INITIALIZER_##NAME = Z__BOOSTER_ENUM_META_##NAME::initializer();

// Chooser

#define BOOSTER_ENUM_MACRO_0(NAME, ORDINAL_TYPE) \
BOOSTER_ENUM_0(NAME, ORDINAL_TYPE)

#define BOOSTER_ENUM_MACRO_1(NAME, ORDINAL_TYPE, ...) \
BOOSTER_ENUM_1(NAME, ORDINAL_TYPE, __VA_ARGS__)


#define ENUM(NAME, ...) \
BOOSTER_MACRO_EXPAND(BOOSTER_MACRO_MACRO_BINARY_SELECTOR(BOOSTER_ENUM_MACRO_, (__VA_ARGS__)) (NAME, __VA_ARGS__))
