

#ifndef booster_data_stream_StreamBufferedProxy_hpp
#define booster_data_stream_StreamBufferedProxy_hpp

#include "Stream.hpp"
#include "booster/data/buffer/FIFOBuffer.hpp"
#include "booster/data/share/MemoryLabel.hpp"
#include "booster/async/Coroutine.hpp"

namespace booster { namespace data{ namespace stream {
  
class OutputStreamBufferedProxy : public booster::base::Countable, public OutputStream {
private:
  std::shared_ptr<OutputStream> m_outputStream;
  booster::data::share::MemoryLabel m_memoryLabel;
  buffer::FIFOBuffer m_buffer;
public:
  OutputStreamBufferedProxy(const std::shared_ptr<OutputStream>& outputStream,
                            const booster::data::share::MemoryLabel& memoryLabel)
    : m_outputStream(outputStream)
    , m_memoryLabel(memoryLabel)
    , m_buffer(const_cast<void *>(memoryLabel.getData()), memoryLabel.getSize())
  {}
public:
  
  static std::shared_ptr<OutputStreamBufferedProxy> createShared(const std::shared_ptr<OutputStream>& outputStream, const booster::data::share::MemoryLabel& memoryLabel) {
    return std::make_shared<OutputStreamBufferedProxy>(outputStream, memoryLabel);
  }
  
  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;


  void setOutputStreamIOMode(booster::data::stream::IOMode ioMode) override;


  booster::data::stream::IOMode getOutputStreamIOMode() override;


  Context& getOutputStreamContext() override;

  v_io_size flush();
  booster::async::CoroutineStarter flushAsync();

  void setBufferPosition(v_io_size readPosition, v_io_size writePosition, bool canRead) {
    m_buffer.setBufferPosition(readPosition, writePosition, canRead);
  }
  
};
  
class InputStreamBufferedProxy : public booster::base::Countable, public BufferedInputStream {
protected:
  std::shared_ptr<InputStream> m_inputStream;
  booster::data::share::MemoryLabel m_memoryLabel;
  buffer::FIFOBuffer m_buffer;
public:
  InputStreamBufferedProxy(const std::shared_ptr<InputStream>& inputStream,
                           const booster::data::share::MemoryLabel& memoryLabel,
                           v_io_size bufferReadPosition,
                           v_io_size bufferWritePosition,
                           bool bufferCanRead)
    : m_inputStream(inputStream)
    , m_memoryLabel(memoryLabel)
    , m_buffer(const_cast<void*>(memoryLabel.getData()), memoryLabel.getSize(), bufferReadPosition, bufferWritePosition, bufferCanRead)
  {}
public:
  
  static std::shared_ptr<InputStreamBufferedProxy> createShared(const std::shared_ptr<InputStream>& inputStream,
                                                                const booster::data::share::MemoryLabel& memoryLabel)
  {
    return std::make_shared<InputStreamBufferedProxy>(inputStream, memoryLabel, 0, 0, false);
  }
  
  static std::shared_ptr<InputStreamBufferedProxy> createShared(const std::shared_ptr<InputStream>& inputStream,
                                                                const booster::data::share::MemoryLabel& memoryLabel,
                                                                v_io_size bufferReadPosition,
                                                                v_io_size bufferWritePosition,
                                                                bool bufferCanRead)
  {
    return std::make_shared<InputStreamBufferedProxy>(inputStream, memoryLabel, bufferReadPosition, bufferWritePosition, bufferCanRead);
  }
  
  v_io_size read(void *data, v_buff_size count, async::Action& action) override;

  v_io_size peek(void *data, v_buff_size count, async::Action& action) override;

  v_io_size commitReadOffset(v_buff_size count) override;

  v_io_size availableToRead() const override;


  void setInputStreamIOMode(booster::data::stream::IOMode ioMode) override;


  booster::data::stream::IOMode getInputStreamIOMode() override;


  Context& getInputStreamContext() override;

  void setBufferPosition(v_io_size readPosition, v_io_size writePosition, bool canRead) {
    m_buffer.setBufferPosition(readPosition, writePosition, canRead);
  }
  
};
  
}}}

#endif
