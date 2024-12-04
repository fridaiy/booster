

#ifndef booster_json_Deserializer_hpp
#define booster_json_Deserializer_hpp

#include "./Utils.hpp"

#include "booster/data/mapping/ObjectMapper.hpp"
#include "booster/data/mapping/Tree.hpp"

#include "booster/utils/parser/Caret.hpp"
#include "booster/Types.hpp"

#include <vector>

namespace booster { namespace json {


class Deserializer {
public:


  class Config : public booster::base::Countable {
  public:

  };

public:

  struct State {
    const Config* config;
    data::mapping::Tree* tree;
    utils::parser::Caret* caret;
    data::mapping::ErrorStack errorStack;
  };

private:

  static void deserializeNull(State& state);
  static void deserializeNumber(State& state);
  static void deserializeBoolean(State& state);
  static void deserializeString(State& state);

  static void deserializeArray(State& state);
  static void deserializeMap(State& state);

public:

  static void deserialize(State& state);

};

}}

#endif
