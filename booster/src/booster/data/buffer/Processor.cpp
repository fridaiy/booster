

#include "Processor.hpp"

namespace booster { namespace data{ namespace buffer {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InlineReadData

InlineReadData::InlineReadData()
  : currBufferPtr(nullptr)
  , bytesLeft(0)
{}

InlineReadData::InlineReadData(void* data, v_buff_size size)
  : currBufferPtr(data)
  , bytesLeft(size)
{}

void InlineReadData::set(void* data, v_buff_size size) {
  currBufferPtr = data;
  bytesLeft = size;
}

void InlineReadData::inc(v_buff_size amount) {
  currBufferPtr = &(reinterpret_cast<p_char8>(currBufferPtr))[amount];
  bytesLeft -= amount;
}

void InlineReadData::setEof() {
  currBufferPtr = &(reinterpret_cast<p_char8>(currBufferPtr))[bytesLeft];
  bytesLeft = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InlineWriteData

InlineWriteData::InlineWriteData()
  : currBufferPtr(nullptr)
  , bytesLeft(0)
{}

InlineWriteData::InlineWriteData(const void* data, v_buff_size size)
  : currBufferPtr(data)
  , bytesLeft(size)
{}

void InlineWriteData::set(const void* data, v_buff_size size) {
  currBufferPtr = data;
  bytesLeft = size;
}

void InlineWriteData::inc(v_buff_size amount) {
  currBufferPtr = &(reinterpret_cast<p_char8>(const_cast<void*>(currBufferPtr)))[amount];
  bytesLeft -= amount;
}

void InlineWriteData::setEof() {
  currBufferPtr = &(reinterpret_cast<p_char8>(const_cast<void*>(currBufferPtr)))[bytesLeft];
  bytesLeft = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ProcessingPipeline

ProcessingPipeline::ProcessingPipeline(const std::vector<base::ObjectHandle<Processor>>& processors)
  : m_processors(processors), m_intermediateData(processors.size() - 1)
{
}

v_io_size ProcessingPipeline::suggestInputStreamReadSize() {
  return m_processors[0]->suggestInputStreamReadSize();
}

v_int32 ProcessingPipeline::iterate(data::buffer::InlineReadData& dataIn,
                                    data::buffer::InlineReadData& dataOut)
{

  if(dataOut.bytesLeft > 0) {
    return Error::FLUSH_DATA_OUT;
  }

  size_t i = 0;
  v_int32 res = Error::OK;

  while(res == Error::OK) {

    auto& p = m_processors[i];

    data::buffer::InlineReadData* currDataIn = &dataIn;
    if(i > 0) {
      currDataIn = &m_intermediateData[i - 1];
    }

    data::buffer::InlineReadData* currDataOut = &dataOut;
    if(i < m_intermediateData.size()) {
      currDataOut = &m_intermediateData[i];
    }

    while(res == Error::OK) {
      res = p->iterate(*currDataIn, *currDataOut);
    }

    const size_t numOfProcessors = m_processors.size();

    switch (res) {
      case Error::PROVIDE_DATA_IN:
        if (i > 0) {
          i --;
          res = Error::OK;
        }
        break;


      case Error::FLUSH_DATA_OUT:
        if (i < numOfProcessors - 1) {
          i ++;
          res = Error::OK;
        }
        break;


      case Error::FINISHED:
        if (i < numOfProcessors - 1) {
          i ++;
          res = Error::OK;
        }
        break;

      default:
        break;
    }

  }

  return res;

}

}}}
