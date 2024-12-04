

#ifndef booster_network_ConnectionPool_hpp
#define booster_network_ConnectionPool_hpp

#include "ConnectionProvider.hpp"
#include "booster/provider/Pool.hpp"

namespace booster { namespace network {


struct ConnectionAcquisitionProxy : public provider::AcquisitionProxy<data::stream::IOStream, ConnectionAcquisitionProxy> {

  ConnectionAcquisitionProxy(const provider::ResourceHandle<data::stream::IOStream>& resource,
                             const std::shared_ptr<PoolInstance>& pool)
    : provider::AcquisitionProxy<data::stream::IOStream, ConnectionAcquisitionProxy>(resource, pool)
  {}

  v_io_size write(const void *buff, v_buff_size count, async::Action& action) override;
  v_io_size read(void *buff, v_buff_size count, async::Action& action) override;

  void setOutputStreamIOMode(booster::data::stream::IOMode ioMode) override;
  booster::data::stream::IOMode getOutputStreamIOMode() override;

  void setInputStreamIOMode(booster::data::stream::IOMode ioMode) override;
  booster::data::stream::IOMode getInputStreamIOMode() override;

  booster::data::stream::Context& getOutputStreamContext() override;
  booster::data::stream::Context& getInputStreamContext() override;

};

typedef booster::provider::Pool<
  booster::network::ClientConnectionProvider,
  booster::data::stream::IOStream,
  booster::network::ConnectionAcquisitionProxy
> ClientConnectionPool;

typedef booster::provider::Pool<
  booster::network::ServerConnectionProvider,
  booster::data::stream::IOStream,
  booster::network::ConnectionAcquisitionProxy
> ServerConnectionPool;


}}

#endif // booster_network_ConnectionPool_hpp
