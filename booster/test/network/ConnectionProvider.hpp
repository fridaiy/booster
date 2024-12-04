

#ifndef booster_network_ConnectionProvider_hpp
#define booster_network_ConnectionProvider_hpp

#include "booster/data/share/MemoryLabel.hpp"
#include "booster/data/stream/Stream.hpp"
#include "booster/provider/Provider.hpp"

#include <unordered_map>

namespace booster { namespace network {


class ConnectionProvider : public provider::Provider<data::stream::IOStream> {
public:


  static const char* const PROPERTY_HOST;


  static const char* const PROPERTY_PORT;

};
  

class ServerConnectionProvider : virtual public ConnectionProvider {
};


class ClientConnectionProvider : virtual public ConnectionProvider {
};
  
}}

#endif
