


#include "FIFOStream.hpp"
#include "booster/utils/Binary.hpp"

namespace booster { namespace data { namespace stream {

data::stream::DefaultInitializedContext FIFOInputStream::DEFAULT_CONTEXT(data::stream::StreamType::STREAM_FINITE);

FIFOInputStream::FIFOInputStream(v_buff_size initialSize)
  : m_memoryHandle(std::make_shared<std::string>(initialSize, static_cast<char>(0)))
  , m_fifo(std::make_shared<data::buffer::FIFOBuffer>(reinterpret_cast<void*>(m_memoryHandle->data()), m_memoryHandle->size(), 0, 0, false))
  , m_maxCapacity(-1) {

}

void FIFOInputStream::reset() {
  m_fifo->setBufferPosition(0, 0, false);
}

v_io_size FIFOInputStream::read(void *data, v_buff_size count, async::Action& action) {
  (void) action;
  return m_fifo->read(data, count);
}

void FIFOInputStream::setInputStreamIOMode(IOMode ioMode) {
  m_ioMode = ioMode;
}

IOMode FIFOInputStream::getInputStreamIOMode() {
  return m_ioMode;
}

Context& FIFOInputStream::getInputStreamContext() {
  return DEFAULT_CONTEXT;
}

std::shared_ptr<std::string> FIFOInputStream::getDataMemoryHandle() {
  return m_memoryHandle;
}

v_io_size FIFOInputStream::write(const void *data, v_buff_size count, async::Action &action) {
  (void) action;
  reserveBytesUpfront(count);
  return m_fifo->write(data, count);
}

void FIFOInputStream::reserveBytesUpfront(v_buff_size count) {

  v_buff_size capacityNeeded = availableToRead() + count;

  if(capacityNeeded > m_fifo->getBufferSize()) {

    v_buff_size newCapacity = utils::Binary::nextP2(capacityNeeded);

    if(newCapacity < 0 || (m_maxCapacity > 0 && newCapacity > m_maxCapacity)) {
      newCapacity = m_maxCapacity;
    }

    if(newCapacity < capacityNeeded) {
      throw std::runtime_error("[booster::data::stream::BufferOutputStream::reserveBytesUpfront()]: Error. Unable to allocate requested memory.");
    }

    // ToDo: In-Memory-Resize
    auto newHandle = std::make_shared<std::string>(newCapacity, static_cast<char>(0));
    v_io_size oldSize = m_fifo->availableToRead();
    m_fifo->read(reinterpret_cast<void*>(newHandle->data()), oldSize);
    auto newFifo = std::make_shared<data::buffer::FIFOBuffer>(reinterpret_cast<void*>(newHandle->data()), newHandle->size(), 0, oldSize, oldSize > 0);
    m_memoryHandle = newHandle;
    m_fifo = newFifo;
  }

}

v_io_size FIFOInputStream::readAndWriteToStream(data::stream::OutputStream *stream,
                                                v_buff_size count,
                                                async::Action &action) {
  return m_fifo->readAndWriteToStream(stream, count, action);
}

v_io_size FIFOInputStream::readFromStreamAndWrite(data::stream::InputStream *stream,
                                                  v_buff_size count,
                                                  async::Action &action) {
  reserveBytesUpfront(count);
  return m_fifo->readFromStreamAndWrite(stream, count, action);
}

v_io_size FIFOInputStream::flushToStream(data::stream::OutputStream *stream) {
  return m_fifo->flushToStream(stream);
}

async::CoroutineStarter FIFOInputStream::flushToStreamAsync(const std::shared_ptr<data::stream::OutputStream> &stream) {
  return m_fifo->flushToStreamAsync(stream);
}

v_io_size FIFOInputStream::availableToWrite() {
  return m_fifo->availableToWrite();
}

v_io_size FIFOInputStream::peek(void *data, v_buff_size count, async::Action &action) {
  (void) action;
  return m_fifo->peek(data, count);
}

v_io_size FIFOInputStream::availableToRead() const {
  return m_fifo->availableToRead();
}

v_io_size FIFOInputStream::commitReadOffset(v_buff_size count) {
  return m_fifo->commitReadOffset(count);
}

}}}
