

#ifndef booster_network_tcp_Connection_hpp
#define booster_network_tcp_Connection_hpp

#include "booster/data/stream/Stream.hpp"

namespace booster { namespace network { namespace tcp {


class Connection : public booster::base::Countable, public booster::data::stream::IOStream {
private:
  static booster::data::stream::DefaultInitializedContext DEFAULT_CONTEXT;
private:
  v_io_handle m_handle;
  data::stream::IOMode m_mode;
private:
  void setStreamIOMode(booster::data::stream::IOMode ioMode);
public:

  Connection(v_io_handle handle);
public:


  ~Connection() override;


  v_io_size write(const void *buff, v_buff_size count, async::Action& action) override;


  v_io_size read(void *buff, v_buff_size count, async::Action& action) override;


  void setOutputStreamIOMode(booster::data::stream::IOMode ioMode) override;


  booster::data::stream::IOMode getOutputStreamIOMode() override;


  booster::data::stream::Context& getOutputStreamContext() override;


  void setInputStreamIOMode(booster::data::stream::IOMode ioMode) override;


  booster::data::stream::IOMode getInputStreamIOMode() override;


  booster::data::stream::Context& getInputStreamContext() override;


  void close();


  v_io_handle getHandle(){
    return m_handle;
  }
  
};
  
}}}

#endif
