

#include "InMemoryData.hpp"

namespace booster { namespace data { namespace resource {

InMemoryData::OutputDataHandle::~OutputDataHandle() {
  dataHandle->data = data + stream->toString();
}

InMemoryData::InMemoryData(const booster::String& data)
  : m_handle(std::make_shared<DataHandle>(data))
{}

std::shared_ptr<data::stream::OutputStream> InMemoryData::openOutputStream() {
  auto outputDataHandle = std::make_shared<OutputDataHandle>();
  if(!m_handle) {
    m_handle = std::make_shared<DataHandle>("");
  }
  if(!m_handle->data){
    m_handle->data = "";
  }
  outputDataHandle->dataHandle = m_handle;
  outputDataHandle->data = m_handle->data;

  auto stream = std::make_shared<data::stream::BufferOutputStream>(1024, outputDataHandle);
  outputDataHandle->stream = stream.get();
  return stream;
}

std::shared_ptr<data::stream::InputStream> InMemoryData::openInputStream() {
  if(!m_handle) {
    m_handle = std::make_shared<DataHandle>("");
  }
  if(!m_handle->data){
    m_handle->data = "";
  }
  return std::make_shared<data::stream::BufferInputStream>(m_handle->data, m_handle);
}

booster::String InMemoryData::getInMemoryData() {
  if(m_handle && m_handle->data) {
    return m_handle->data;
  }
  return nullptr;
}

v_int64 InMemoryData::getKnownSize() {
  if(m_handle && m_handle->data) {
    return static_cast<v_int64>(m_handle->data->size());
  }
  return 0;
}

booster::String InMemoryData::getLocation() {
  return nullptr;
}

}}}
