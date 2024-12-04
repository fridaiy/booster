

#ifndef booster_json_ObjectMapper_hpp
#define booster_json_ObjectMapper_hpp

#include "./Serializer.hpp"
#include "./Deserializer.hpp"

#include "booster/data/mapping/ObjectToTreeMapper.hpp"
#include "booster/data/mapping/TreeToObjectMapper.hpp"
#include "booster/data/mapping/ObjectMapper.hpp"

namespace booster { namespace json {


class ObjectMapper : public booster::base::Countable, public booster::data::mapping::ObjectMapper {
private:
  static Info getMapperInfo() {
    return Info("application", "json");
  }

public:

  class DeserializerConfig {
  public:
    data::mapping::TreeToObjectMapper::Config mapper;
    Deserializer::Config json;
  };

public:

  class SerializerConfig {
  public:
    data::mapping::ObjectToTreeMapper::Config mapper;
    Serializer::Config json;
  };

private:
  void writeTree(data::stream::ConsistentOutputStream* stream, const data::mapping::Tree& tree, data::mapping::ErrorStack& errorStack) const;
private:
  SerializerConfig m_serializerConfig;
  DeserializerConfig m_deserializerConfig;
private:
  data::mapping::ObjectToTreeMapper m_objectToTreeMapper;
  data::mapping::TreeToObjectMapper m_treeToObjectMapper;
public:

  ObjectMapper(const SerializerConfig& serializerConfig = {}, const DeserializerConfig& deserializerConfig = {});

  void write(data::stream::ConsistentOutputStream* stream, const booster::Void& variant, data::mapping::ErrorStack& errorStack) const override;

  booster::Void read(booster::utils::parser::Caret& caret, const booster::Type* type, data::mapping::ErrorStack& errorStack) const override;

  const data::mapping::ObjectToTreeMapper& objectToTreeMapper() const;
  const data::mapping::TreeToObjectMapper& treeToObjectMapper() const;

  data::mapping::ObjectToTreeMapper& objectToTreeMapper();
  data::mapping::TreeToObjectMapper& treeToObjectMapper();

  const SerializerConfig& serializerConfig() const;
  const DeserializerConfig& deserializerConfig() const;

  SerializerConfig& serializerConfig();
  DeserializerConfig& deserializerConfig();
  
};
  
}}

#endif
