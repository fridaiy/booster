

#include "StreamBufferedProxy.hpp"

namespace booster { namespace data{ namespace stream {
  
v_io_size OutputStreamBufferedProxy::write(const void *data, v_buff_size count, async::Action& action) {
  if(m_buffer.availableToWrite() > 0) {
    return m_buffer.write(data, count);
  } else {
    auto bytesFlushed = m_buffer.readAndWriteToStream(m_outputStream.get(), m_buffer.getBufferSize(), action);
    if(bytesFlushed > 0) {
      return m_buffer.write(data, count);
    }
    return bytesFlushed;
  }
}

void OutputStreamBufferedProxy::setOutputStreamIOMode(booster::data::stream::IOMode ioMode) {
  m_outputStream->setOutputStreamIOMode(ioMode);
}

booster::data::stream::IOMode OutputStreamBufferedProxy::getOutputStreamIOMode() {
  return m_outputStream->getOutputStreamIOMode();
}

Context& OutputStreamBufferedProxy::getOutputStreamContext() {
  return m_outputStream->getOutputStreamContext();
}

v_io_size OutputStreamBufferedProxy::flush() {
  return m_buffer.flushToStream(m_outputStream.get());
}

booster::async::CoroutineStarter OutputStreamBufferedProxy::flushAsync() {
  return m_buffer.flushToStreamAsync(m_outputStream);
}
  
v_io_size InputStreamBufferedProxy::read(void *data, v_buff_size count, async::Action& action) {
  
  if(m_buffer.availableToRead() > 0) {
    return m_buffer.read(data, count);
  } else {
    auto bytesBuffered = m_buffer.readFromStreamAndWrite(m_inputStream.get(), m_buffer.getBufferSize(), action);
    if(bytesBuffered > 0) {
      return m_buffer.read(data, count);
    }
    return bytesBuffered;
  }
  
}

v_io_size InputStreamBufferedProxy::peek(void *data, v_buff_size count, async::Action& action) {

  if(m_buffer.availableToRead() > 0) {
    return m_buffer.peek(data, count);
  } else {
    auto bytesBuffered = m_buffer.readFromStreamAndWrite(m_inputStream.get(), m_buffer.getBufferSize(), action);
    if(bytesBuffered > 0) {
      return m_buffer.peek(data, count);
    }
    return bytesBuffered;
  }

}

v_io_size InputStreamBufferedProxy::commitReadOffset(v_buff_size count) {
  return m_buffer.commitReadOffset(count);
}

void InputStreamBufferedProxy::setInputStreamIOMode(booster::data::stream::IOMode ioMode) {
  m_inputStream->setInputStreamIOMode(ioMode);
}

booster::data::stream::IOMode InputStreamBufferedProxy::getInputStreamIOMode() {
  return m_inputStream->getInputStreamIOMode();
}

Context& InputStreamBufferedProxy::getInputStreamContext() {
  return m_inputStream->getInputStreamContext();
}

v_io_size InputStreamBufferedProxy::availableToRead() const {
  return m_buffer.availableToRead();
}

}}}
