

#ifndef booster_data_stream_FIFOStream_hpp
#define booster_data_stream_FIFOStream_hpp

#include "Stream.hpp"
#include "booster/data/buffer/FIFOBuffer.hpp"

namespace booster { namespace data { namespace stream {



class FIFOInputStream : public BufferedInputStream, public WriteCallback {
 public:
  static data::stream::DefaultInitializedContext DEFAULT_CONTEXT;
 private:
  std::shared_ptr<std::string> m_memoryHandle;
  std::shared_ptr<data::buffer::FIFOBuffer> m_fifo;
  v_buff_size m_maxCapacity;
  IOMode m_ioMode;
 public:


  FIFOInputStream(v_buff_size initialSize = 4096);

  static std::shared_ptr<FIFOInputStream> createShared(v_buff_size initialSize = 4096) {
    return std::make_shared<FIFOInputStream>(initialSize);
  }


  void reset();


  v_io_size read(void *data, v_buff_size count, async::Action& action) override;


  void setInputStreamIOMode(IOMode ioMode) override;


  IOMode getInputStreamIOMode() override;


  Context& getInputStreamContext() override;


  std::shared_ptr<std::string> getDataMemoryHandle();


  v_io_size write(const void *data, v_buff_size count, async::Action &action) override;


  v_io_size peek(void *data, v_buff_size count, async::Action& action) override;


  v_io_size availableToRead() const override;


  v_io_size commitReadOffset(v_buff_size count) override;


  void reserveBytesUpfront(v_buff_size count);


  v_io_size readAndWriteToStream(data::stream::OutputStream* stream, v_buff_size count, async::Action& action);


  v_io_size readFromStreamAndWrite(data::stream::InputStream* stream, v_buff_size count, async::Action& action);


  v_io_size flushToStream(data::stream::OutputStream* stream);


  async::CoroutineStarter flushToStreamAsync(const std::shared_ptr<data::stream::OutputStream>& stream);


  v_io_size availableToWrite();
};

}}}

#endif // booster_data_stream_FIFOStream_hpp
