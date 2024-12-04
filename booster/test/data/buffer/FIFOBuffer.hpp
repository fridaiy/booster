

#ifndef booster_data_buffer_FIFOBuffer_hpp
#define booster_data_buffer_FIFOBuffer_hpp

#include "booster/data/stream/Stream.hpp"
#include "booster/IODefinitions.hpp"
#include "booster/async/Coroutine.hpp"
#include "booster/concurrency/SpinLock.hpp"

namespace booster { namespace data { namespace buffer {


class FIFOBuffer {
private:
  p_char8 m_buffer;
  v_buff_size m_bufferSize;
  v_buff_size m_readPosition;
  v_buff_size m_writePosition;
  bool m_canRead;
public:


  FIFOBuffer(void* buffer, v_buff_size bufferSize,
             v_buff_size readPosition = 0, v_buff_size writePosition = 0,
             bool canRead = false);


  void setBufferPosition(v_buff_size readPosition, v_buff_size writePosition, bool canRead);


  v_io_size availableToRead() const;


  v_io_size availableToWrite() const;


  v_buff_size getBufferSize() const;


  v_io_size read(void *data, v_buff_size count);


  v_io_size peek(void *data, v_buff_size count);


  v_io_size commitReadOffset(v_buff_size count);


  v_io_size write(const void *data, v_buff_size count);


  v_io_size readAndWriteToStream(data::stream::WriteCallback* stream, v_buff_size count, async::Action& action);


  v_io_size readFromStreamAndWrite(data::stream::ReadCallback* stream, v_buff_size count, async::Action& action);


  v_io_size flushToStream(data::stream::OutputStream* stream);


  async::CoroutineStarter flushToStreamAsync(const std::shared_ptr<data::stream::OutputStream>& stream);

  
};


class SynchronizedFIFOBuffer {
private:
  FIFOBuffer m_fifo;
  booster::concurrency::SpinLock m_lock;
public:


  SynchronizedFIFOBuffer(void* buffer, v_buff_size bufferSize,
                         v_buff_size readPosition = 0, v_buff_size writePosition = 0,
                         bool canRead = false);


  void setBufferPosition(v_buff_size readPosition, v_buff_size writePosition, bool canRead);


  v_io_size availableToRead();


  v_io_size availableToWrite();


  v_io_size read(void *data, v_buff_size count);


  v_io_size write(const void *data, v_buff_size count);




};
  
}}}

#endif
