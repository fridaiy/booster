

#ifndef booster_data_stream_BufferStream_hpp
#define booster_data_stream_BufferStream_hpp

#include "Stream.hpp"

namespace booster { namespace data{ namespace stream {


class BufferOutputStream : public ConsistentOutputStream {
public:
  static data::stream::DefaultInitializedContext DEFAULT_CONTEXT;
private:
  p_char8 m_data;
  v_buff_size m_capacity;
  v_buff_size m_position;
  v_buff_size m_maxCapacity;
  IOMode m_ioMode;
private:
  std::shared_ptr<void> m_capturedData;
public:


  BufferOutputStream(v_buff_size initialCapacity = 2048, const std::shared_ptr<void>& captureData = nullptr);


  ~BufferOutputStream() override;


  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;


  void setOutputStreamIOMode(IOMode ioMode) override;


  IOMode getOutputStreamIOMode() override;


  Context& getOutputStreamContext() override;


  void reserveBytesUpfront(v_buff_size count);


  p_char8 getData();


  v_buff_size getCapacity();


  v_buff_size getCurrentPosition();


  void setCurrentPosition(v_buff_size position);


  void reset(v_buff_size initialCapacity = 2048);


  booster::String toString();


  std::string toStdString() const;


  booster::String getSubstring(v_buff_size pos, v_buff_size count);


  booster::v_io_size flushToStream(OutputStream* stream);


  static booster::async::CoroutineStarter flushToStreamAsync(const std::shared_ptr<BufferOutputStream>& _this, const std::shared_ptr<OutputStream>& stream);

};


class BufferInputStream : public BufferedInputStream {
public:
  static data::stream::DefaultInitializedContext DEFAULT_CONTEXT;
private:
  std::shared_ptr<std::string> m_memoryHandle;
  p_char8 m_data;
  v_buff_size m_size;
  v_buff_size m_position;
  IOMode m_ioMode;
private:
  std::shared_ptr<void> m_capturedData;
public:


  BufferInputStream(const std::shared_ptr<std::string>& memoryHandle,
                    const void* data,
                    v_buff_size size,
                    const std::shared_ptr<void>& captureData = nullptr);


  BufferInputStream(const booster::String& data, const std::shared_ptr<void>& captureData = nullptr);


  void reset(const std::shared_ptr<std::string>& memoryHandle,
             p_char8 data,
             v_buff_size size,
             const std::shared_ptr<void>& captureData = nullptr);



  void reset();


  v_io_size read(void *data, v_buff_size count, async::Action& action) override;


  void setInputStreamIOMode(IOMode ioMode) override;


  IOMode getInputStreamIOMode() override;


  Context& getInputStreamContext() override;


  std::shared_ptr<std::string> getDataMemoryHandle();


  p_char8 getData();


  v_buff_size getDataSize();


  v_buff_size getCurrentPosition();


  void setCurrentPosition(v_buff_size position);


  v_io_size peek(void *data, v_buff_size count, async::Action& action) override;


  v_io_size availableToRead() const override;


  v_io_size commitReadOffset(v_buff_size count) override;

};

}}}

#endif // booster_data_stream_BufferStream_hpp
