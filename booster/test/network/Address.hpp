

#ifndef booster_network_Address_hpp
#define booster_network_Address_hpp

#include "booster/Types.hpp"

namespace booster { namespace network {


class Address {
public:


  enum Family : v_int32 {


    IP_4 = 0,


    IP_6 = 1,


    UNSPEC = 2
  };

public:


  Address(const booster::String& pHost, v_uint16 pPort, Family pFamily = UNSPEC);


  booster::String host;


  v_uint16 port;


  Family family;

};

}}

#endif // booster_network_Address_hpp
