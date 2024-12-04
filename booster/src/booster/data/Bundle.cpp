

#include "Bundle.hpp"

namespace booster { namespace data {

void Bundle::put(const booster::String& key, const booster::Void& polymorph) {
  m_data.insert({key, polymorph});
}

const std::unordered_map<booster::String, booster::Void>& Bundle::getAll() const {
  return m_data;
}

}}
