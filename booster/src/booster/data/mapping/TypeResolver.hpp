

#ifndef booster_data_mapping_TypeResolver_hpp
#define booster_data_mapping_TypeResolver_hpp

#include "booster/Types.hpp"

namespace booster { namespace data { namespace mapping {


class TypeResolver {
public:


  struct Cache {

    std::unordered_map<const booster::Type*, const booster::Type*> types;


    std::unordered_map<const booster::Type*, std::unordered_map<booster::Void, booster::Void>> values;
  };

private:

  const booster::Type* findPropertyType(const booster::Type* baseType,
                                      const std::vector<std::string>& path,
                                      v_uint32 pathPosition,
                                      Cache& cache) const;

  booster::Void findPropertyValue(const booster::Void& baseObject,
                                const std::vector<std::string>& path,
                                v_uint32 pathPosition,
                                Cache& cache) const;

private:
  std::vector<bool> m_knownClasses;
  std::vector<std::string> m_enabledInterpretations;
public:


  TypeResolver();


  virtual ~TypeResolver() = default;


  void setKnownClass(const booster::ClassId& classId, bool isKnown);


  void addKnownClasses(const std::vector<booster::ClassId>& knownClasses);


  bool isKnownClass(const booster::ClassId& classId) const;


  bool isKnownType(const booster::Type* type) const;


  void setEnabledInterpretations(const std::vector<std::string>& interpretations);


  const std::vector<std::string>& getEnabledInterpretations() const;


  const booster::Type* resolveType(const booster::Type* type, Cache& cache) const;


  booster::Void resolveValue(const booster::Void& value, Cache& cache) const;


  const booster::Type* resolveObjectPropertyType(const booster::Type* objectType,
                                              const std::vector<std::string>& path,
                                              Cache& cache) const;


  booster::Void resolveObjectPropertyValue(const booster::Void& object,
                                         const std::vector<std::string>& path,
                                         Cache& cache) const;

};

}}}

#endif // booster_data_mapping_TypeResolver_hpp
