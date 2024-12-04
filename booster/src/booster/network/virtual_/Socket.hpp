

#ifndef booster_network_virtual__Socket_hpp
#define booster_network_virtual__Socket_hpp

#include "./Pipe.hpp"

namespace booster { namespace network { namespace virtual_ {


class Socket : public booster::data::stream::IOStream, public booster::base::Countable {
private:
  std::shared_ptr<Pipe> m_pipeIn;
  std::shared_ptr<Pipe> m_pipeOut;
public:

  Socket(const std::shared_ptr<Pipe>& pipeIn, const std::shared_ptr<Pipe>& pipeOut);
public:


  static std::shared_ptr<Socket> createShared(const std::shared_ptr<Pipe>& pipeIn, const std::shared_ptr<Pipe>& pipeOut);


  ~Socket() override;


  void setMaxAvailableToReadWrtie(v_io_size maxToRead, v_io_size maxToWrite);


  v_io_size read(void *data, v_buff_size count, async::Action& action) override;


  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;


  void setOutputStreamIOMode(booster::data::stream::IOMode ioMode) override;


  booster::data::stream::IOMode getOutputStreamIOMode() override;


  void setInputStreamIOMode(booster::data::stream::IOMode ioMode) override;


  booster::data::stream::IOMode getInputStreamIOMode() override;


  booster::data::stream::Context& getOutputStreamContext() override;


  booster::data::stream::Context& getInputStreamContext() override;


  void close();
  
};
  
}}}

#endif
