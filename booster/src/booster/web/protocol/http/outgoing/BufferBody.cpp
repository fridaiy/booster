

#include "BufferBody.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace outgoing {

BufferBody::BufferBody(const booster::String &buffer, const data::share::StringKeyLabel &contentType)
  : m_buffer(buffer ? buffer : "")
  , m_contentType(contentType)
  , m_inlineData(reinterpret_cast<void*>(m_buffer->data()), static_cast<v_buff_size>(m_buffer->size()))
{}

std::shared_ptr<BufferBody> BufferBody::createShared(const booster::String &buffer,
                                                     const data::share::StringKeyLabel &contentType) {
  return std::make_shared<BufferBody>(buffer, contentType);
}

v_io_size BufferBody::read(void *buffer, v_buff_size count, async::Action &action) {

  (void) action;

  v_buff_size desiredToRead = m_inlineData.bytesLeft;

  if (desiredToRead > 0) {

    if (desiredToRead > count) {
      desiredToRead = count;
    }

    std::memcpy(buffer, m_inlineData.currBufferPtr, static_cast<size_t>(desiredToRead));
    m_inlineData.inc(desiredToRead);

    return desiredToRead;

  }

  return 0;

}

void BufferBody::declareHeaders(Headers &headers) {
  if (m_contentType) {
    headers.putIfNotExists(Header::CONTENT_TYPE, m_contentType);
  }
}

p_char8 BufferBody::getKnownData() {
  return reinterpret_cast<p_char8>(m_buffer->data());
}

v_int64 BufferBody::getKnownSize() {
  return static_cast<v_int64>(m_buffer->size());
}

}}}}}
