

#ifndef booster_data_share_LazyStringMap_hpp
#define booster_data_share_LazyStringMap_hpp

#include "./MemoryLabel.hpp"
#include "booster/concurrency/SpinLock.hpp"

#include <unordered_map>

namespace booster { namespace data { namespace share {


template<typename Key, typename MapType>
class LazyStringMapTemplate {
public:
  typedef booster::data::type::String String;
private:
  mutable concurrency::SpinLock m_lock;
  mutable bool m_fullyInitialized;
  MapType m_map;
public:


  LazyStringMapTemplate()
    : m_fullyInitialized(true)
  {}


  LazyStringMapTemplate(const LazyStringMapTemplate& other) {

    std::lock_guard<concurrency::SpinLock> otherLock(other.m_lock);

    m_fullyInitialized = other.m_fullyInitialized;
    m_map = MapType(other.m_map);

  }


  LazyStringMapTemplate(LazyStringMapTemplate&& other) {

    std::lock_guard<concurrency::SpinLock> otherLock(other.m_lock);

    m_fullyInitialized = other.m_fullyInitialized;
    m_map = std::move(other.m_map);

  }

  LazyStringMapTemplate& operator = (const LazyStringMapTemplate& other) {

    if(this != &other) {

      std::lock_guard<concurrency::SpinLock> thisLock(m_lock);
      std::lock_guard<concurrency::SpinLock> otherLock(other.m_lock);

      m_fullyInitialized = other.m_fullyInitialized;
      m_map = MapType(other.m_map);

    }

    return *this;

  }

  LazyStringMapTemplate& operator = (LazyStringMapTemplate&& other) {

    if(this != &other) {

      std::lock_guard<concurrency::SpinLock> thisLock(m_lock);
      std::lock_guard<concurrency::SpinLock> otherLock(other.m_lock);

      m_fullyInitialized = other.m_fullyInitialized;
      m_map = std::move(other.m_map);

    }

    return *this;

  }


  void put(const Key& key, const StringKeyLabel& value) {

    std::lock_guard<concurrency::SpinLock> lock(m_lock);

    m_map.insert({key, value});
    m_fullyInitialized = false;

  }


  void put_LockFree(const Key& key, const StringKeyLabel& value) {
    m_map.insert({key, value});
    m_fullyInitialized = false;
  }


  bool putIfNotExists(const Key& key, const StringKeyLabel& value) {

    std::lock_guard<concurrency::SpinLock> lock(m_lock);

    auto it = m_map.find(key);

    if(it == m_map.end()) {
      m_map.insert({key, value});
      m_fullyInitialized = false;
      return true;
    }

    return false;

  }


  bool putIfNotExists_LockFree(const Key& key, const StringKeyLabel& value) {

    auto it = m_map.find(key);

    if(it == m_map.end()) {
      m_map.insert({key, value});
      m_fullyInitialized = false;
      return true;
    }

    return false;

  }


  bool putOrReplace(const Key& key, const StringKeyLabel& value) {

    std::lock_guard<concurrency::SpinLock> lock(m_lock);

    bool needsErase = m_map.find(key) != m_map.end();
    if (needsErase) {
      m_map.erase(key);
    }
    m_map.insert({key, value});
    m_fullyInitialized = false;

    return needsErase;

  }


  bool putOrReplace_LockFree(const Key& key, const StringKeyLabel& value) {

    bool needsErase = m_map.find(key) != m_map.end();
    if (needsErase) {
      m_map.erase(key);
    }
    m_map.insert({key, value});
    m_fullyInitialized = false;

    return needsErase;

  }


  String get(const Key& key) const {

    std::lock_guard<concurrency::SpinLock> lock(m_lock);

    auto it = m_map.find(key);

    if(it != m_map.end()) {
      it->second.captureToOwnMemory();
      return it->second.getMemoryHandle();
    }

    return nullptr;

  }


  template<class T>
  T getAsMemoryLabel(const Key& key) const {

    std::lock_guard<concurrency::SpinLock> lock(m_lock);

    auto it = m_map.find(key);

    if(it != m_map.end()) {
      it->second.captureToOwnMemory();
      const auto& label = it->second;
      return T(label.getMemoryHandle(), reinterpret_cast<const char*>(label.getData()), label.getSize());
    }

    return T(nullptr, nullptr, 0);

  }


  template<class T>
  T getAsMemoryLabel_Unsafe(const Key& key) const {

    std::lock_guard<concurrency::SpinLock> lock(m_lock);

    auto it = m_map.find(key);

    if(it != m_map.end()) {
      const auto& label = it->second;
      return T(label.getMemoryHandle(), reinterpret_cast<const char*>(label.getData()), label.getSize());
    }

    return T(nullptr, nullptr, 0);

  }


  const MapType& getAll() const {

    std::lock_guard<concurrency::SpinLock> lock(m_lock);

    if(!m_fullyInitialized) {

      for(auto& pair : m_map) {
        pair.first.captureToOwnMemory();
        pair.second.captureToOwnMemory();
      }

      m_fullyInitialized = true;
    }

    return m_map;

  }


  const MapType& getAll_Unsafe() const {
    return m_map;
  }


  v_int32 getSize() const {
    std::lock_guard<concurrency::SpinLock> lock(m_lock);
    return static_cast<v_int32>(m_map.size());
  }

};


template<typename Key, typename Value = StringKeyLabel>
using LazyStringMap = LazyStringMapTemplate<Key, std::unordered_map<Key, Value>>;


template<typename Key, typename Value = StringKeyLabel>
using LazyStringMultimap = LazyStringMapTemplate<Key, std::unordered_multimap<Key, Value>>;

}}}

#endif //booster_data_share_LazyStringMap_hpp
