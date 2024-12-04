

#ifndef booster_network_tcp_server_ConnectionProvider_hpp
#define booster_network_tcp_server_ConnectionProvider_hpp

#include "booster/network/Address.hpp"
#include "booster/network/ConnectionProvider.hpp"
#include "booster/network/tcp/Connection.hpp"
#include "booster/network/tcp/ConnectionConfigurer.hpp"

#include "booster/Types.hpp"

namespace booster { namespace network { namespace tcp { namespace server {


class ConnectionProvider : public ServerConnectionProvider {
private:

  class ConnectionInvalidator : public provider::Invalidator<data::stream::IOStream> {
  public:

    void invalidate(const std::shared_ptr<data::stream::IOStream>& connection) override;

  };

public:


  class ExtendedConnection : public booster::network::tcp::Connection {
  public:

    static const char* const PROPERTY_PEER_ADDRESS;
    static const char* const PROPERTY_PEER_ADDRESS_FORMAT;
    static const char* const PROPERTY_PEER_PORT;

  protected:
    data::stream::DefaultInitializedContext m_context;
  public:


    ExtendedConnection(v_io_handle handle, data::stream::Context::Properties&& properties);


    booster::data::stream::Context& getOutputStreamContext() override;


    booster::data::stream::Context& getInputStreamContext() override;

  };

private:
  std::shared_ptr<ConnectionInvalidator> m_invalidator;
  network::Address m_address;
  std::atomic<bool> m_closed;
  booster::v_io_handle m_serverHandle;
  bool m_useExtendedConnections;
  std::shared_ptr<ConnectionConfigurer> m_connectionConfigurer;
private:
  booster::v_io_handle instantiateServer();
private:
  void prepareConnectionHandle(booster::v_io_handle handle);
  provider::ResourceHandle<data::stream::IOStream> getDefaultConnection();
  provider::ResourceHandle<data::stream::IOStream> getExtendedConnection();
public:


  ConnectionProvider(const network::Address& address, bool useExtendedConnections = false);

public:


  static std::shared_ptr<ConnectionProvider> createShared(const network::Address& address, bool useExtendedConnections = false){
    return std::make_shared<ConnectionProvider>(address, useExtendedConnections);
  }


  void setConnectionConfigurer(const std::shared_ptr<ConnectionConfigurer>& connectionConfigurer);


  ~ConnectionProvider() override;


  void stop() override;


  provider::ResourceHandle<data::stream::IOStream> get() override;


  booster::async::CoroutineStarterForResult<const provider::ResourceHandle<data::stream::IOStream>&> getAsync() override {

    throw std::runtime_error("[booster::network::tcp::server::ConnectionProvider::getAsync()]: Error. Not implemented.");
  }


  const network::Address& getAddress() const {
    return m_address;
  }
  
};
  
}}}}

#endif
