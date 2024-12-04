

#ifndef booster_json_Serializer_hpp
#define booster_json_Serializer_hpp

#include "./Utils.hpp"
#include "./Beautifier.hpp"

#include "booster/data/mapping/ObjectMapper.hpp"
#include "booster/data/mapping/Tree.hpp"
#include "booster/Types.hpp"

namespace booster { namespace json {


class Serializer {
public:


  class Config : public booster::base::Countable {
  public:


    bool includeNullElements = true;


    bool useBeautifier = false;


    booster::String beautifierIndent = "  ";


    booster::String beautifierNewLine = "\n";


    v_uint32 escapeFlags = json::Utils::FLAG_ESCAPE_ALL;

  };

public:

  struct State {

    const Config* config;
    const data::mapping::Tree* tree;
    data::stream::ConsistentOutputStream* stream;

    data::mapping::ErrorStack errorStack;

  };

private:

  
  static void serializeString(booster::data::stream::ConsistentOutputStream* stream,
                              const char* data,
                              v_buff_size size,
                              v_uint32 escapeFlags);

  static void serializeNull(State& state);
  static void serializeString(State& state);
  static void serializeArray(State& state);
  static void serializeMap(State& state);
  static void serializePairs(State& state);

  static void serialize(State& state);

public:

  static void serializeToStream(data::stream::ConsistentOutputStream* stream, State& state);

};

}}

#endif
