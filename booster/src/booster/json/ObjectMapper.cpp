

#include "ObjectMapper.hpp"

namespace booster { namespace json {

ObjectMapper::ObjectMapper(const SerializerConfig& serializerConfig, const DeserializerConfig& deserializerConfig)
  : data::mapping::ObjectMapper(getMapperInfo())
  , m_serializerConfig(serializerConfig)
  , m_deserializerConfig(deserializerConfig)
{}

void ObjectMapper::writeTree(data::stream::ConsistentOutputStream* stream, const data::mapping::Tree& tree, data::mapping::ErrorStack& errorStack) const {
  Serializer::State state;
  state.config = &m_serializerConfig.json;
  state.tree = &tree;
  Serializer::serializeToStream(stream, state);
  if(!state.errorStack.empty()) {
    errorStack = std::move(state.errorStack);
    return;
  }
}

void ObjectMapper::write(data::stream::ConsistentOutputStream* stream, const booster::Void& variant, data::mapping::ErrorStack& errorStack) const {


  if(variant.getValueType() == booster::Tree::Class::getType()) {
    auto tree = static_cast<const data::mapping::Tree*>(variant.get());
    writeTree(stream, *tree, errorStack);
    return;
  }

  data::mapping::Tree tree;
  data::mapping::ObjectToTreeMapper::State state;

  state.config = &m_serializerConfig.mapper;
  state.tree = &tree;

  m_objectToTreeMapper.map(state, variant);
  if(!state.errorStack.empty()) {
    errorStack = std::move(state.errorStack);
    return;
  }

  writeTree(stream, tree, errorStack);

}

booster::Void ObjectMapper::read(utils::parser::Caret& caret, const data::type::Type* type, data::mapping::ErrorStack& errorStack) const {

  data::mapping::Tree tree;

  {
    Deserializer::State state;
    state.caret = &caret;
    state.tree = &tree;
    state.config = &m_deserializerConfig.json;
    Deserializer::deserialize(state);
    if(!state.errorStack.empty()) {
      errorStack = std::move(state.errorStack);
      return nullptr;
    }
  }


  if(type == data::type::Tree::Class::getType()) {
    return booster::Tree(std::move(tree));
  }

  {
    data::mapping::TreeToObjectMapper::State state;
    state.tree = &tree;
    state.config = &m_deserializerConfig.mapper;
    const auto & result = m_treeToObjectMapper.map(state, type);
    if(!state.errorStack.empty()) {
      errorStack = std::move(state.errorStack);
      return nullptr;
    }
    return result;
  }

}

const data::mapping::ObjectToTreeMapper& ObjectMapper::objectToTreeMapper() const {
  return m_objectToTreeMapper;
}

const data::mapping::TreeToObjectMapper& ObjectMapper::treeToObjectMapper() const {
  return m_treeToObjectMapper;
}

data::mapping::ObjectToTreeMapper& ObjectMapper::objectToTreeMapper() {
  return m_objectToTreeMapper;
}

data::mapping::TreeToObjectMapper& ObjectMapper::treeToObjectMapper() {
  return m_treeToObjectMapper;
}

const ObjectMapper::SerializerConfig& ObjectMapper::serializerConfig() const {
  return m_serializerConfig;
}

const ObjectMapper::DeserializerConfig& ObjectMapper::deserializerConfig() const {
  return m_deserializerConfig;
}

ObjectMapper::SerializerConfig& ObjectMapper::serializerConfig() {
  return m_serializerConfig;
}

ObjectMapper::DeserializerConfig& ObjectMapper::deserializerConfig() {
  return m_deserializerConfig;
}

}}
