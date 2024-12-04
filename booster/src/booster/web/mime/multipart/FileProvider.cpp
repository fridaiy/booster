

#include "FileProvider.hpp"

#include "booster/data/resource/File.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {

FileProvider::FileProvider(const booster::String& filename)
  : m_filename(filename)
{}

std::shared_ptr<data::resource::Resource> FileProvider::getResource(const std::shared_ptr<Part>& part) {
  (void)part;
  return std::make_shared<data::resource::File>(m_filename->c_str());
}

async::CoroutineStarter FileProvider::getResourceAsync(const std::shared_ptr<Part>& part,
                                                       std::shared_ptr<data::resource::Resource>& stream)
{
  (void)part;
  stream = std::make_shared<data::resource::File>(m_filename->c_str());
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other functions

std::shared_ptr<PartReader> createFilePartReader(const booster::String& filename, v_io_size maxDataSize) {
  auto provider = std::make_shared<FileProvider>(filename);
  auto reader = std::make_shared<StreamPartReader>(provider, maxDataSize);
  return reader;
}

std::shared_ptr<AsyncPartReader> createAsyncFilePartReader(const booster::String& filename, v_io_size maxDataSize) {
  auto provider = std::make_shared<FileProvider>(filename);
  auto reader = std::make_shared<AsyncStreamPartReader>(provider, maxDataSize);
  return reader;
}

}}}}
