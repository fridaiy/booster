

#ifndef booster_data_type_Map_hpp
#define booster_data_type_Map_hpp

#include "./Type.hpp"
#include <list>

namespace booster { namespace data { namespace type {

namespace __class {


class Map {
public:


  struct Iterator {


    virtual ~Iterator() = default;


    virtual type::Void getKey() = 0;


    virtual type::Void getValue() = 0;


    virtual void next() = 0;


    virtual bool finished() = 0;

  };

public:


  class PolymorphicDispatcher {
  public:


    virtual ~PolymorphicDispatcher() = default;


    virtual type::Void createObject() const = 0;


    virtual const type::Type* getKeyType() const = 0;


    virtual const type::Type* getValueType() const = 0;


    virtual v_int64 getMapSize(const type::Void& object) const = 0;


    virtual void addItem(const type::Void& object, const type::Void& key, const type::Void& value) const = 0;


    virtual std::unique_ptr<Iterator> beginIteration(const type::Void& object) const = 0;

  };

  template<class ContainerType, class KeyType, class ValueType>
  struct Inserter {

    static void insert(ContainerType* c, const KeyType& k, const ValueType& v) {
      (*c)[k] = v;
    }

  };

};

template<class ContainerType, class KeyType, class ValueType, class Clazz>
class StandardMap {
public:

  struct Iterator : public Map::Iterator {

    typename ContainerType::iterator iterator;
    typename ContainerType::iterator end;

    type::Void getKey() override {
      return iterator->first;
    }

    type::Void getValue() override {
      return iterator->second;
    }

    void next() override {
      std::advance(iterator, 1);
    }

    bool finished() override {
      return iterator == end;
    }

  };

public:

  class PolymorphicDispatcher : public Map::PolymorphicDispatcher {
  public:

    type::Void createObject() const override {
      return type::Void(std::make_shared<ContainerType>(), Clazz::getType());
    }

    const type::Type* getKeyType() const override {
      const type::Type* mapType = Clazz::getType();
      return mapType->params[0];
    }

    const type::Type* getValueType() const override {
      const type::Type* mapType = Clazz::getType();
      return mapType->params[1];
    }

    v_int64 getMapSize(const type::Void& object) const override {
      ContainerType* map = static_cast<ContainerType*>(object.get());
      return static_cast<v_int64>(map->size());
    }

    void addItem(const type::Void& object, const type::Void& key, const type::Void& value) const override {
      ContainerType* map = static_cast<ContainerType*>(object.get());
      const auto& mapKey = key.template cast<KeyType>();
      const auto& mapValue = value.template cast<ValueType>();
      Map::Inserter<ContainerType, KeyType, ValueType>::insert(map, mapKey, mapValue);
    }

    std::unique_ptr<Map::Iterator> beginIteration(const type::Void& object) const override {
      ContainerType* map = static_cast<ContainerType*>(object.get());
      auto iterator = new Iterator();
      iterator->iterator = map->begin();
      iterator->end = map->end();
      return std::unique_ptr<Map::Iterator>(iterator);
    }

  };

};

template<class KeyType, class ValueType>
struct Map::Inserter<std::list<std::pair<KeyType, ValueType>>, KeyType, ValueType> {
  static void insert(std::list<std::pair<KeyType, ValueType>>* c, const KeyType& k, const ValueType& v) {
    c->push_back({k, v});
  }
};

}

}}}

#endif //booster_data_type_Map_hpp
