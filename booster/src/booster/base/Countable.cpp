

#include "Countable.hpp"

namespace booster { namespace base{
  
Countable::Countable() {
#ifndef BOOSTER_DISABLE_ENV_OBJECT_COUNTERS
  Environment::incObjects();
#endif
}

Countable::Countable(const Countable& other) {
  (void)other;
#ifndef BOOSTER_DISABLE_ENV_OBJECT_COUNTERS
  Environment::incObjects();
#endif
}

Countable::~Countable(){
#ifndef BOOSTER_DISABLE_ENV_OBJECT_COUNTERS
  Environment::decObjects();
#endif
}
  
}}
