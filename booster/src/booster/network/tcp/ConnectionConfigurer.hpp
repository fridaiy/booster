

#ifndef booster_network_tcp_ConnectionConfigurer_hpp
#define booster_network_tcp_ConnectionConfigurer_hpp

#include "booster/IODefinitions.hpp"

namespace booster { namespace network { namespace tcp {


class ConnectionConfigurer {
public:

  virtual ~ConnectionConfigurer() = default;


  virtual void configure(booster::v_io_handle handle) = 0;
};

}}}

#endif
