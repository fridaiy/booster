

#ifndef booster_data_Stream
#define booster_data_Stream

#include "booster/data/share/LazyStringMap.hpp"
#include "booster/async/Coroutine.hpp"
#include "booster/data/buffer/IOBuffer.hpp"
#include "booster/data/buffer/Processor.hpp"

#include "booster/IODefinitions.hpp"

namespace booster { namespace data{ namespace stream {


enum StreamType : v_int32 {


  STREAM_FINITE = 0,


  STREAM_INFINITE = 1

};



class Context {
public:

  typedef booster::data::share::LazyStringMap<booster::data::share::StringKeyLabel> Properties;
private:
  Properties m_properties;
protected:

  Properties& getMutableProperties();
public:


  Context() = default;


  Context(Properties&& properties);


  virtual ~Context() = default;


  virtual void init() = 0;


  virtual async::CoroutineStarter initAsync() = 0;


  virtual bool isInitialized() const = 0;


  virtual StreamType getStreamType() const = 0;


  const Properties& getProperties() const;

  inline bool operator == (const Context& other){
    return this == &other;
  }

  inline bool operator != (const Context& other){
    return this != &other;
  }

};


class DefaultInitializedContext : public booster::data::stream::Context {
private:
  StreamType m_streamType;
public:


  DefaultInitializedContext(StreamType streamType);


  DefaultInitializedContext(StreamType streamType, Properties&& properties);


  void init() override;


  async::CoroutineStarter initAsync() override;


  bool isInitialized() const override;


  StreamType getStreamType() const override;

};


enum IOMode : v_int32 {


  BLOCKING = 0,


  ASYNCHRONOUS = 1
};


class WriteCallback {
public:


  virtual ~WriteCallback() = default;


  virtual v_io_size write(const void *data, v_buff_size count, async::Action& action) = 0;

  v_io_size write(data::buffer::InlineWriteData& inlineData, async::Action& action);

  v_io_size writeSimple(const void *data, v_buff_size count);

  v_io_size writeExactSizeDataSimple(data::buffer::InlineWriteData& inlineData);

  v_io_size writeExactSizeDataSimple(const void *data, v_buff_size count);

  async::Action writeExactSizeDataAsyncInline(data::buffer::InlineWriteData& inlineData, async::Action&& nextAction);

  async::CoroutineStarter writeExactSizeDataAsync(const void* data, v_buff_size size);


  v_io_size writeSimple(const char* data){
    return writeSimple(data, static_cast<v_buff_size>(std::strlen(data)));
  }


  v_io_size writeSimple(const booster::String& str){
    return writeSimple(str->data(), static_cast<v_buff_size>(str->size()));
  }


  v_io_size writeCharSimple(v_char8 c){
    return writeSimple(&c, 1);
  }

};


class OutputStream : public WriteCallback {
public:


  virtual ~OutputStream() override = default;


  virtual void setOutputStreamIOMode(IOMode ioMode) = 0;


  virtual IOMode getOutputStreamIOMode() = 0;


  virtual Context& getOutputStreamContext() = 0;

};


class ReadCallback {
public:


  virtual ~ReadCallback() = default;


  virtual v_io_size read(void *buffer, v_buff_size count, async::Action& action) = 0;

  v_io_size read(data::buffer::InlineReadData& inlineData, async::Action& action);

  v_io_size readExactSizeDataSimple(data::buffer::InlineReadData& inlineData);

  v_io_size readExactSizeDataSimple(void *data, v_buff_size count);

  async::Action readExactSizeDataAsyncInline(data::buffer::InlineReadData& inlineData, async::Action&& nextAction);

  async::Action readSomeDataAsyncInline(data::buffer::InlineReadData& inlineData, async::Action&& nextAction);

  v_io_size readSimple(void *data, v_buff_size count);

};


class InputStream : public ReadCallback {
public:


  virtual ~InputStream() override = default;


  virtual void setInputStreamIOMode(IOMode ioMode) = 0;


  virtual IOMode getInputStreamIOMode() = 0;


  virtual Context& getInputStreamContext() = 0;

};


class BufferedInputStream : public InputStream {
 public:

  virtual ~BufferedInputStream() override = default;


  virtual v_io_size peek(void *data, v_buff_size count, async::Action& action) = 0;


  virtual v_io_size availableToRead() const = 0;


  virtual v_io_size commitReadOffset(v_buff_size count) = 0;
};


class IOStream : public InputStream, public OutputStream {
public:


  void initContexts();


  async::CoroutineStarter initContextsAsync();

};


class ConsistentOutputStream : public OutputStream {
public:


  v_io_size writeAsString(v_int8 value);


  v_io_size writeAsString(v_uint8 value);


  v_io_size writeAsString(v_int16 value);


  v_io_size writeAsString(v_uint16 value);


  v_io_size writeAsString(v_int32 value);


  v_io_size writeAsString(v_uint32 value);


  v_io_size writeAsString(v_int64 value);


  v_io_size writeAsString(v_uint64 value);


  v_io_size writeAsString(v_float32 value);


  v_io_size writeAsString(v_float64 value);


  v_io_size writeAsString(bool value);

};

ConsistentOutputStream& operator << (ConsistentOutputStream& s, const booster::String& str);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const Int8& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const UInt8& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const Int16& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const UInt16& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const Int32& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const UInt32& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const Int64& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const UInt64& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const Float32& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const Float64& value);
ConsistentOutputStream& operator << (ConsistentOutputStream& s, const Boolean& value);

ConsistentOutputStream& operator << (ConsistentOutputStream& s, const char* str);

template<typename T>
ConsistentOutputStream& operator << (ConsistentOutputStream& s, T value) {
  s.writeAsString(value);
  return s;
}


class AsyncTransferError : public booster::async::Error {
public:

  AsyncTransferError(const char* what) : booster::async::Error(what) {}
};


class StatelessDataTransferProcessor : public data::buffer::Processor {
public:
  static StatelessDataTransferProcessor INSTANCE;
public:
  v_io_size suggestInputStreamReadSize() override;
  v_int32 iterate(data::buffer::InlineReadData& dataIn, data::buffer::InlineReadData& dataOut) override;
};


v_io_size transfer(const base::ObjectHandle<ReadCallback>& readCallback,
                   const base::ObjectHandle<WriteCallback>& writeCallback,
                   v_io_size transferSize,
                   void* buffer,
                   v_buff_size bufferSize,
                   const base::ObjectHandle<data::buffer::Processor>& processor = &StatelessDataTransferProcessor::INSTANCE);


async::CoroutineStarter transferAsync(const base::ObjectHandle<ReadCallback>& readCallback,
                                      const base::ObjectHandle<WriteCallback>& writeCallback,
                                      v_buff_size transferSize,
                                      const base::ObjectHandle<data::buffer::IOBuffer>& buffer,
                                      const base::ObjectHandle<data::buffer::Processor>& processor = &StatelessDataTransferProcessor::INSTANCE);

  
}}}

#endif
