

#ifndef booster_network_ConnectionHandler_hpp
#define booster_network_ConnectionHandler_hpp

#include "booster/provider/Provider.hpp"
#include "booster/data/stream/Stream.hpp"
#include <unordered_map>

namespace booster { namespace network {


class ConnectionHandler {
public:

  typedef booster::data::stream::IOStream IOStream;


  typedef std::unordered_map<booster::String, booster::String> ParameterMap;
public:


  virtual ~ConnectionHandler() = default;


  virtual void handleConnection(const provider::ResourceHandle<IOStream>& connectionData,
                                const std::shared_ptr<const ParameterMap>& params) = 0;


  virtual void stop() = 0;
};
  
}}

#endif
