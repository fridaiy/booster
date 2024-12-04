

#ifndef booster_data_mapping_TreeToObjectMapper_hpp
#define booster_data_mapping_TreeToObjectMapper_hpp

#include "./Tree.hpp"
#include "./ObjectMapper.hpp"

#include "booster/utils/Conversion.hpp"

namespace booster { namespace data { namespace mapping {

class TreeToObjectMapper : public base::Countable {
public:

  struct Config {
    bool allowUnknownFields = true;
    bool allowLexicalCasting = false;
    bool useUnqualifiedFieldNames = false;
    bool useUnqualifiedEnumNames = false;
    std::vector<std::string> enabledInterpretations = {};


    void* extra = nullptr;
  };

public:

  enum class GuessedPrimitiveType : v_uint32 {
    NOT_PRIMITIVE = 0,
    BOOL_TRUE = 1,
    BOOL_FALSE = 2,
    INT = 3,
    FLOAT = 4
  };

public:

  struct State {

    const Config* config;
    const Tree* tree;
    ErrorStack errorStack;

  };

public:
  typedef booster::Void (*MapperMethod)(const TreeToObjectMapper*, State&, const Type* const);
public:

  static GuessedPrimitiveType guessedPrimitiveType(const booster::String& text);

  template<class T>
  static booster::Void mapPrimitive(const TreeToObjectMapper* mapper, State& state, const Type* const type){
    (void) mapper;
    (void) type;
    if(state.tree->isPrimitive()) {
      return T(state.tree->operator typename T::UnderlyingType());
    }
    if(state.tree->isNull()) {
      return booster::Void(T::Class::getType());
    }
    if(state.config->allowLexicalCasting && state.tree->isString()) {
      const auto& text = state.tree->getString();
      auto pt = guessedPrimitiveType(text);
      switch (pt) {
        case GuessedPrimitiveType::BOOL_TRUE:
          return T(static_cast<typename T::UnderlyingType>(true));
        case GuessedPrimitiveType::BOOL_FALSE:
          return T(static_cast<typename T::UnderlyingType>(false));
        case GuessedPrimitiveType::INT: {
          bool success;
          auto value = static_cast<typename T::UnderlyingType>(utils::Conversion::strToInt64(text, success));
          if(!success) break;
          return T(value);
        }
        case GuessedPrimitiveType::FLOAT: {
          bool success;
          auto value = static_cast<typename T::UnderlyingType>(utils::Conversion::strToFloat64(text, success));
          if(!success) break;
          return T(value);
        }

        case GuessedPrimitiveType::NOT_PRIMITIVE:
        default:
          break;
      }
    }
    state.errorStack.push("[booster::data::mapping::TreeToObjectMapper::mapPrimitive()]: Value is NOT a Primitive type");
    return nullptr;
  }

  static const Type* guessType(const Tree& node);

  static booster::Void mapString(const TreeToObjectMapper* mapper, State& state, const Type* type);
  static booster::Void mapTree(const TreeToObjectMapper* mapper, State& state, const Type* type);
  static booster::Void mapAny(const TreeToObjectMapper* mapper, State& state, const Type* type);
  static booster::Void mapEnum(const TreeToObjectMapper* mapper, State& state, const Type* type);

  static booster::Void mapCollection(const TreeToObjectMapper* mapper, State& state, const Type* type);
  static booster::Void mapMap(const TreeToObjectMapper* mapper, State& state, const Type* type);

  static booster::Void mapObject(const TreeToObjectMapper* mapper, State& state, const Type* type);

private:
  std::vector<MapperMethod> m_methods;
public:

  TreeToObjectMapper();

  void setMapperMethod(const data::type::ClassId& classId, MapperMethod method);

  booster::Void map(State& state, const Type* type) const;

};

}}}

#endif //booster_data_mapping_TreeToObjectMapper_hpp
