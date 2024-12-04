

#include "IOBuffer.hpp"

namespace booster { namespace data { namespace buffer {

const v_buff_size IOBuffer::BUFFER_SIZE = 4096;

IOBuffer::IOBuffer()
  : m_entry(new v_char8[BUFFER_SIZE])
{}

std::shared_ptr<IOBuffer> IOBuffer::createShared(){
  return std::make_shared<IOBuffer>();
}

IOBuffer::~IOBuffer() {
  delete [] m_entry;
}

void* IOBuffer::getData(){
  return m_entry;
}

v_buff_size IOBuffer::getSize(){
  return BUFFER_SIZE;
}
  
}}}
