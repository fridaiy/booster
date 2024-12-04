

#ifndef booster_data_mapping_ObjectRemapper_hpp
#define booster_data_mapping_ObjectRemapper_hpp

#include "./ObjectToTreeMapper.hpp"
#include "./TreeToObjectMapper.hpp"

#include "booster/Types.hpp"

namespace booster { namespace data { namespace mapping {

class ObjectRemapper {
protected:
  ObjectToTreeMapper::Config m_objectToTreeConfig;
  TreeToObjectMapper::Config m_treeToObjectConfig;
  ObjectToTreeMapper m_objectToTreeMapper;
  TreeToObjectMapper m_treeToObjectMapper;
public:

  ObjectRemapper() = default;
  virtual ~ObjectRemapper() = default;

  booster::Void remap(const Tree& tree, const booster::Type* toType, ErrorStack& errorStack) const;

  template<class Wrapper>
  Wrapper remap(const Tree& tree, ErrorStack& errorStack) const {
    auto toType = Wrapper::Class::getType();
    return remap(tree, toType, errorStack).template cast<Wrapper>();
  }

  template<class Wrapper>
  Wrapper remap(const Tree& tree) const {
    auto toType = Wrapper::Class::getType();
    ErrorStack errorStack;
    const auto& result = remap(tree, toType, errorStack).template cast<Wrapper>();
    if(!errorStack.empty()) {
      throw MappingError(std::move(errorStack));
    }
    return result;
  }

  booster::Void remap(const booster::Void& polymorph, const booster::Type* toType, ErrorStack& errorStack) const;

  template<class Wrapper>
  Wrapper remap(const booster::Void& polymorph, ErrorStack& errorStack) const {
    auto toType = Wrapper::Class::getType();
    return remap(polymorph, toType, errorStack).template cast<Wrapper>();
  }

  template<class Wrapper>
  Wrapper remap(const booster::Void& polymorph) const {
    auto toType = Wrapper::Class::getType();
    ErrorStack errorStack;
    const auto& result = remap(polymorph, toType, errorStack).template cast<Wrapper>();
    if(!errorStack.empty()) {
      throw MappingError(std::move(errorStack));
    }
    return result;
  }

  ObjectToTreeMapper::Config& objectToTreeConfig();
  TreeToObjectMapper::Config& treeToObjectConfig();

  ObjectToTreeMapper& objectToTreeMapper();
  TreeToObjectMapper& treeToObjectMapper();

  const ObjectToTreeMapper::Config& objectToTreeConfig() const;
  const TreeToObjectMapper::Config& treeToObjectConfig() const;

  const ObjectToTreeMapper& objectToTreeMapper() const;
  const TreeToObjectMapper& treeToObjectMapper() const;

};

}}}

#endif // booster_data_mapping_ObjectRemapper_hpp
