

#ifndef booster_provider_Provider_hpp
#define booster_provider_Provider_hpp

#include "Invalidator.hpp"

#include "booster/async/Coroutine.hpp"
#include "booster/data/share/MemoryLabel.hpp"

namespace booster { namespace provider {


template<class T, class PTR>
struct ResourceHandleTemplate {


  ResourceHandleTemplate() = default;


  ResourceHandleTemplate(std::nullptr_t) {}


  ResourceHandleTemplate(const PTR& resourceObject,
                         const std::shared_ptr<Invalidator<T>> &resourceInvalidator)
    : object(resourceObject), invalidator(resourceInvalidator)
  {}


  PTR object;


  std::shared_ptr<Invalidator<T>> invalidator;

  inline bool operator == (std::nullptr_t) const {
    return object.get() == nullptr;
  }

  inline bool operator != (std::nullptr_t) const {
    return object.get() != nullptr;
  }

  explicit inline operator bool() const {
    return object.operator bool();
  }


  virtual void invalidate() {
    invalidator->invalidate(object);
  }

};


template<class T>
struct ResourceHandle : public ResourceHandleTemplate<T, std::shared_ptr<T>> {


  ResourceHandle() = default;


  ResourceHandle(std::nullptr_t) {}


  ResourceHandle(const std::shared_ptr<T>& resourceObject,
                 const std::shared_ptr<Invalidator<T>>& resourceInvalidator)
    : ResourceHandleTemplate<T, std::shared_ptr<T>>(resourceObject, resourceInvalidator)
  {}

};


template<class T>
struct WeakResourceHandle : public ResourceHandleTemplate<T, std::weak_ptr<T>> {


  WeakResourceHandle() = default;


  WeakResourceHandle(std::nullptr_t) {}


  WeakResourceHandle(const std::weak_ptr<T>& resourceObject,
                     const std::shared_ptr<Invalidator<T>>& resourceInvalidator)
    : ResourceHandleTemplate<T, std::weak_ptr<T>>(resourceObject, resourceInvalidator)
  {}

};


template <class T>
class Provider : public booster::base::Countable {
protected:

  void setProperty(const booster::String& key, const booster::String& value) {
    m_properties[key] = value;
  }

protected:
  std::unordered_map<data::share::StringKeyLabelCI, data::share::StringKeyLabel> m_properties;
public:


  Provider() = default;


  Provider(const std::unordered_map<data::share::StringKeyLabelCI, data::share::StringKeyLabel>& properties)
    : m_properties(properties)
  {}


  virtual ~Provider() override = default;


  const std::unordered_map<data::share::StringKeyLabelCI, data::share::StringKeyLabel>& getProperties() const {
    return m_properties;
  }


  data::share::StringKeyLabel getProperty(const booster::String& key) const {
    auto it = m_properties.find(key);
    if(it == m_properties.end()) {
      return nullptr;
    }
    return it->second;
  }


  virtual ResourceHandle<T> get() = 0;


  virtual async::CoroutineStarterForResult<const ResourceHandle<T>&> getAsync() = 0;


  virtual void stop() = 0;

};

}}

#endif // booster_provider_Provider_hpp
