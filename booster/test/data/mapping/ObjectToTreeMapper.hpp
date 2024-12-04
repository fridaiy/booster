

#ifndef booster_data_mapping_ObjectToTreeMapper_hpp
#define booster_data_mapping_ObjectToTreeMapper_hpp

#include "./Tree.hpp"
#include "./ObjectMapper.hpp"

namespace booster { namespace data { namespace mapping {

class ObjectToTreeMapper : public base::Countable {
public:

  struct Config {
    bool includeNullFields = true;
    bool alwaysIncludeRequired = false;
    bool alwaysIncludeNullCollectionElements = false;
    bool useUnqualifiedFieldNames = false;
    bool useUnqualifiedEnumNames = false;
    std::vector<std::string> enabledInterpretations = {};


    void* extra = nullptr;
  };

public:

  struct State {

    const Config* config;
    Tree* tree;
    ErrorStack errorStack;

  };

public:
  typedef void (*MapperMethod)(const ObjectToTreeMapper*, State&, const booster::Void&);
public:

  template<class T>
  static void mapPrimitive(const ObjectToTreeMapper* mapper, State& state, const booster::Void& polymorph){
    (void) mapper;
    if(polymorph){
      state.tree->setPrimitive<typename T::ObjectType>(* static_cast<typename T::ObjectType*>(polymorph.get()));
    } else {
      state.tree->setNull();
    }
  }

  static void mapString(const ObjectToTreeMapper* mapper, State& state, const booster::Void& polymorph);
  static void mapTree(const ObjectToTreeMapper* mapper, State& state, const booster::Void& polymorph);
  static void mapAny(const ObjectToTreeMapper* mapper, State& state, const booster::Void& polymorph);
  static void mapEnum(const ObjectToTreeMapper* mapper, State& state, const booster::Void& polymorph);

  static void mapCollection(const ObjectToTreeMapper* mapper, State& state, const booster::Void& polymorph);
  static void mapMap(const ObjectToTreeMapper* mapper, State& state, const booster::Void& polymorph);

  static void mapObject(const ObjectToTreeMapper* mapper, State& state, const booster::Void& polymorph);

private:
  std::vector<MapperMethod> m_methods;
public:

  ObjectToTreeMapper();

  void setMapperMethod(const data::type::ClassId& classId, MapperMethod method);

  void map(State& state, const booster::Void& polymorph) const;

};

}}}

#endif //booster_data_mapping_ObjectToTreeMapper_hpp
