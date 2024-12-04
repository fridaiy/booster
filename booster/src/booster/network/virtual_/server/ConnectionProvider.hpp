

#ifndef booster_network_virtual__server_ConnectionProvider_hpp
#define booster_network_virtual__server_ConnectionProvider_hpp

#include "booster/network/virtual_/Interface.hpp"
#include "booster/network/ConnectionProvider.hpp"

namespace booster { namespace network { namespace virtual_ { namespace server {


class ConnectionProvider : public booster::network::ServerConnectionProvider {
private:

  class ConnectionInvalidator : public provider::Invalidator<data::stream::IOStream> {
  public:

    void invalidate(const std::shared_ptr<data::stream::IOStream>& connection) override;

  };

private:
  std::shared_ptr<ConnectionInvalidator> m_invalidator;
  std::shared_ptr<virtual_::Interface> m_interface;
  std::shared_ptr<virtual_::Interface::ListenerLock> m_listenerLock;
  bool m_open;
  v_io_size m_maxAvailableToRead;
  v_io_size m_maxAvailableToWrite;
public:


  ConnectionProvider(const std::shared_ptr<virtual_::Interface>& _interface);


  static std::shared_ptr<ConnectionProvider> createShared(const std::shared_ptr<virtual_::Interface>& _interface);


  void setSocketMaxAvailableToReadWrtie(v_io_size maxToRead, v_io_size maxToWrite);


  void stop() override;


  provider::ResourceHandle<data::stream::IOStream> get() override;


  booster::async::CoroutineStarterForResult<const provider::ResourceHandle<data::stream::IOStream>&> getAsync() override {

    throw std::runtime_error("[booster::network::virtual_::server::ConnectionProvider::getConnectionAsync()]: Error. Not implemented.");
  }
  
};
  
}}}}

#endif
