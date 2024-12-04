

#ifndef booster_data_type_Collection_hpp
#define booster_data_type_Collection_hpp

#include "./Type.hpp"
#include <unordered_set>

namespace booster { namespace data { namespace type {

namespace __class {


class Collection {
public:


  struct Iterator {


    virtual ~Iterator() = default;


    virtual type::Void get() = 0;


    virtual void next() = 0;


    virtual bool finished() = 0;

  };

public:


  class PolymorphicDispatcher {
  public:


    virtual ~PolymorphicDispatcher() = default;


    virtual type::Void createObject() const = 0;


    virtual const type::Type* getItemType() const = 0;


    virtual v_int64 getCollectionSize(const type::Void& object) const = 0;


    virtual void addItem(const type::Void& object, const type::Void& item) const = 0;


    virtual std::unique_ptr<Iterator> beginIteration(const type::Void& object) const = 0;

  };

  template<class ContainerType, class ItemType>
  struct Inserter {

    static void insert(ContainerType* c, const ItemType& i) {
      c->emplace_back(i);
    }

  };

};

template<class ContainerType, class ItemType, class Clazz>
class StandardCollection {
public:

  struct Iterator : public Collection::Iterator {

    typename ContainerType::iterator iterator;
    typename ContainerType::iterator end;

    type::Void get() override {
      return *iterator;
    }

    void next() override {
      std::advance(iterator, 1);
    }

    bool finished() override {
      return iterator == end;
    }

  };

public:

  class PolymorphicDispatcher : public Collection::PolymorphicDispatcher {
  public:

    type::Void createObject() const override {
      return type::Void(std::make_shared<ContainerType>(), Clazz::getType());
    }

    const type::Type* getItemType() const override {
      const type::Type* collectionType = Clazz::getType();
      return collectionType->params[0];
    }

    v_int64 getCollectionSize(const type::Void& object) const override {
      ContainerType* collection = static_cast<ContainerType*>(object.get());
      return static_cast<v_int64>(collection->size());
    }

    void addItem(const type::Void& object, const type::Void& item) const override {
      ContainerType* collection = static_cast<ContainerType*>(object.get());
      const auto& collectionItem = item.template cast<ItemType>();
      Collection::Inserter<ContainerType, ItemType>::insert(collection, collectionItem);
    }

    std::unique_ptr<Collection::Iterator> beginIteration(const type::Void& object) const override {
      ContainerType* collection = static_cast<ContainerType*>(object.get());
      auto iterator = new Iterator();
      iterator->iterator = collection->begin();
      iterator->end = collection->end();
      return std::unique_ptr<Collection::Iterator>(iterator);
    }

  };

};

template<class ItemType>
struct Collection::Inserter<std::unordered_set<ItemType>, ItemType> {
  static void insert(std::unordered_set<ItemType>* c, const ItemType& i) {
    c->emplace(i);
  }
};

}

}}}

#endif //booster_data_type_Collection_hpp
