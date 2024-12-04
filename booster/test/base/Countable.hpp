

#ifndef booster_base_Countable_hpp
#define booster_base_Countable_hpp

#include "booster/Environment.hpp"

#include <memory>

namespace booster { namespace base{


class Countable {
public:
  
  Countable();

  
  Countable(const Countable& other);

  
  virtual ~Countable();

  Countable& operator = (Countable&) = default;


};
  
}}

#endif 
