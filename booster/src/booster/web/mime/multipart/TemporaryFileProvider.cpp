

#include "TemporaryFileProvider.hpp"

#include "booster/data/resource/TemporaryFile.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {

TemporaryFileProvider::TemporaryFileProvider(const booster::String& tmpDirectory, v_int32 randomWordSizeBytes)
  : m_tmpDirectory(tmpDirectory)
  , m_randomWordSizeBytes(randomWordSizeBytes)
{}

std::shared_ptr<data::resource::Resource> TemporaryFileProvider::getResource(const std::shared_ptr<Part>& part) {
  (void)part;
  return std::make_shared<data::resource::TemporaryFile>(m_tmpDirectory, m_randomWordSizeBytes);
}

async::CoroutineStarter TemporaryFileProvider::getResourceAsync(const std::shared_ptr<Part>& part,
                                                                std::shared_ptr<data::resource::Resource>& stream)
{
  (void)part;
  stream = std::make_shared<data::resource::TemporaryFile>(m_tmpDirectory, m_randomWordSizeBytes);
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other functions

std::shared_ptr<PartReader> createTemporaryFilePartReader(const booster::String& tmpDirectory,
                                                          v_int32 randomWordSizeBytes,
                                                          v_io_size maxDataSize)
{
  auto provider = std::make_shared<TemporaryFileProvider>(tmpDirectory, randomWordSizeBytes);
  auto reader = std::make_shared<StreamPartReader>(provider, maxDataSize);
  return reader;
}

std::shared_ptr<AsyncPartReader> createAsyncTemporaryFilePartReader(const booster::String& tmpDirectory,
                                                                    v_int32 randomWordSizeBytes,
                                                                    v_io_size maxDataSize)
{
  auto provider = std::make_shared<TemporaryFileProvider>(tmpDirectory, randomWordSizeBytes);
  auto reader = std::make_shared<AsyncStreamPartReader>(provider, maxDataSize);
  return reader;
}

}}}}
