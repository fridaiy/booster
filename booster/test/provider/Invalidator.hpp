

#ifndef booster_provider_Invalidator_hpp
#define booster_provider_Invalidator_hpp

#include "booster/base/Countable.hpp"
#include <memory>

namespace booster { namespace provider {


template<class T>
class Invalidator : public booster::base::Countable {
public:


  virtual ~Invalidator() override = default;


  virtual void invalidate(const std::shared_ptr<T> &resource) = 0;

};

}}

#endif //booster_provider_Invalidator_hpp
