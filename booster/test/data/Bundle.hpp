

#ifndef booster_data_Bundle_hpp
#define booster_data_Bundle_hpp

#include "booster/Types.hpp"

namespace booster { namespace data {


class Bundle {
private:
  std::unordered_map<booster::String, booster::Void> m_data;
public:


  Bundle() = default;


  virtual ~Bundle() = default;


  void put(const booster::String& key, const booster::Void& polymorph);


  template<typename WrapperType>
  WrapperType get(const booster::String& key) const {
    auto it = m_data.find(key);
    if(it == m_data.end()) {
      throw std::runtime_error("[booster::data::Bundle::get()]: "
                               "Error. Data not found for key '" + *key + "'.");
    }

    if(!WrapperType::Class::getType()->extends(it->second.getValueType())) {
      throw std::runtime_error("[booster::data::Bundle::get()]: Error. Type mismatch for key '" + *key +
                               "'. Stored '" +
                               std::string(it->second.getValueType()->classId.name) +
                               "' vs requested '" + std::string(WrapperType::Class::getType()->classId.name) + "'.");
    }
    return it->second.template cast<WrapperType>();
  }


  const std::unordered_map<booster::String, booster::Void>& getAll() const;

};

}}

#endif //booster_data_Bundle_hpp
