

#include "TemporaryFile.hpp"

#include "./File.hpp"

#include "booster/data/stream/FileStream.hpp"
#include "booster/data/stream/BufferStream.hpp"
#include "booster/encoding/Hex.hpp"
#include "booster/utils/Random.hpp"

namespace booster { namespace data { namespace resource {

TemporaryFile::FileHandle::~FileHandle() {
  if(fileName) {
    std::remove(fileName->c_str());
  }
}

booster::String TemporaryFile::constructRandomFilename(const booster::String &dir, v_int32 randomWordSizeBytes, const booster::String &extension) {

  std::unique_ptr<v_char8[]> buff(new v_char8[static_cast<unsigned long>(randomWordSizeBytes)]);
  utils::Random::randomBytes(buff.get(), randomWordSizeBytes);
  data::stream::BufferOutputStream s(randomWordSizeBytes * 2 + 4);
  encoding::Hex::encode(&s, buff.get(), randomWordSizeBytes, encoding::Hex::ALPHABET_LOWER);
  if (extension->at(0) != '.') {
    s << ".";
  }
  s << extension;

  return File::concatDirAndName(dir, s.toString());

}

TemporaryFile::TemporaryFile(const booster::String& tmpDirectory, v_int32 randomWordSizeBytes)
  : m_handle(std::make_shared<FileHandle>(constructRandomFilename(tmpDirectory, randomWordSizeBytes, "tmp")))
{}

TemporaryFile::TemporaryFile(const booster::String& tmpDirectory, const booster::String& tmpFileName)
  : m_handle(std::make_shared<FileHandle>(File::concatDirAndName(tmpDirectory, tmpFileName)))
{}

TemporaryFile::TemporaryFile(const booster::String& tmpDirectory, v_int32 randomWordSizeBytes, const booster::String& extension)
  : m_handle(std::make_shared<FileHandle>(constructRandomFilename(tmpDirectory, randomWordSizeBytes, extension)))
{}

std::shared_ptr<data::stream::OutputStream> TemporaryFile::openOutputStream() {
  if(m_handle) {
    return std::make_shared<data::stream::FileOutputStream>(m_handle->fileName->c_str(), "wb", m_handle);
  }
  throw std::runtime_error("[booster::data::resource::TemporaryFile::openOutputStream()]: Error. FileHandle is NOT initialized.");
}

std::shared_ptr<data::stream::InputStream> TemporaryFile::openInputStream() {
  if(m_handle) {
    return std::make_shared<data::stream::FileInputStream>(m_handle->fileName->c_str(), m_handle);
  }
  throw std::runtime_error("[booster::data::resource::TemporaryFile::openInputStream()]: Error. FileHandle is NOT initialized.");
}

booster::String TemporaryFile::getInMemoryData() {
  return nullptr;
}

v_int64 TemporaryFile::getKnownSize() {
  return -1;
}

booster::String TemporaryFile::getLocation() {
  if(m_handle) {
    return m_handle->fileName;
  }
  return nullptr;
}

bool TemporaryFile::moveFile(const booster::String& fullFileName) {
  if(m_handle) {
    return std::rename(m_handle->fileName->c_str(), fullFileName->c_str()) == 0;
  }
  return false;
}

}}}
