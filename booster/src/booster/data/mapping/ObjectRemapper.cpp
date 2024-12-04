

#include "ObjectRemapper.hpp"

namespace booster { namespace data { namespace mapping {

booster::Void ObjectRemapper::remap(const Tree& tree, const booster::Type* toType, ErrorStack& errorStack) const {
  TreeToObjectMapper::State state;
  state.tree = std::addressof(tree);
  state.config = &m_treeToObjectConfig;
  const auto & result = m_treeToObjectMapper.map(state, toType);
  if(!state.errorStack.empty()) {
    errorStack = std::move(state.errorStack);
    return nullptr;
  }
  return result;
}

booster::Void ObjectRemapper::remap(const booster::Void& polymorph, const booster::Type* toType, ErrorStack& errorStack) const {

  if(polymorph == nullptr) {
    return nullptr;
  }


  if(polymorph.getValueType() == booster::Tree::Class::getType()) {
    auto tree = static_cast<const Tree*>(polymorph.get());
    return remap(*tree, toType, errorStack);
  }

  Tree tree;

  {
    ObjectToTreeMapper::State state;
    state.tree = &tree;
    state.config = &m_objectToTreeConfig;
    m_objectToTreeMapper.map(state, polymorph);
    if(!state.errorStack.empty()) {
      errorStack = std::move(state.errorStack);
      return nullptr;
    }
  }


  if(toType == data::type::Tree::Class::getType()) {
    return booster::Tree(std::move(tree));
  }

  return remap(tree, toType, errorStack);

}

ObjectToTreeMapper::Config& ObjectRemapper::objectToTreeConfig() {
  return m_objectToTreeConfig;
}

TreeToObjectMapper::Config& ObjectRemapper::treeToObjectConfig() {
  return m_treeToObjectConfig;
}

ObjectToTreeMapper& ObjectRemapper::objectToTreeMapper() {
  return m_objectToTreeMapper;
}

TreeToObjectMapper& ObjectRemapper::treeToObjectMapper() {
  return m_treeToObjectMapper;
}

const ObjectToTreeMapper::Config& ObjectRemapper::objectToTreeConfig() const {
  return m_objectToTreeConfig;
}

const TreeToObjectMapper::Config& ObjectRemapper::treeToObjectConfig() const {
  return m_treeToObjectConfig;
}

const ObjectToTreeMapper& ObjectRemapper::objectToTreeMapper() const {
  return m_objectToTreeMapper;
}

const TreeToObjectMapper& ObjectRemapper::treeToObjectMapper() const {
  return m_treeToObjectMapper;
}

}}}
