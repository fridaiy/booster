

#include "File.hpp"

#include "booster/data/stream/FileStream.hpp"

namespace booster { namespace data { namespace resource {

booster::String File::concatDirAndName(const booster::String& dir, const booster::String& filename) {
  if(dir && dir->size() > 0) {
    auto lastChar = dir->data()[dir->size() - 1];
    if(lastChar != '/' && lastChar != '\\') {
      return dir + "/" + filename;
    }
    return dir + filename;
  }
  return filename;
}

File::File(const booster::String& fullFileName)
  : m_handle(std::make_shared<FileHandle>(fullFileName))
{}

File::File(const booster::String& tmpDirectory, const booster::String& tmpFileName)
  : m_handle(std::make_shared<FileHandle>(concatDirAndName(tmpDirectory, tmpFileName)))
{}

std::shared_ptr<data::stream::OutputStream> File::openOutputStream() {
  if(m_handle) {
    return std::make_shared<data::stream::FileOutputStream>(m_handle->fileName->c_str(), "wb", m_handle);
  }
  throw std::runtime_error("[booster::data::resource::File::openOutputStream()]: Error. FileHandle is NOT initialized.");
}

std::shared_ptr<data::stream::InputStream> File::openInputStream() {
  if(m_handle) {
    return std::make_shared<data::stream::FileInputStream>(m_handle->fileName->c_str(), m_handle);
  }
  throw std::runtime_error("[booster::data::resource::File::openInputStream()]: Error. FileHandle is NOT initialized.");
}

booster::String File::getInMemoryData() {
  return nullptr;
}

v_int64 File::getKnownSize() {
  return -1;
}

booster::String File::getLocation() {
  if(m_handle) {
    return m_handle->fileName;
  }
  return nullptr;
}

}}}
