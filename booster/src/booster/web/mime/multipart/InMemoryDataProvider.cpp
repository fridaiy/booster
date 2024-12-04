

#include "InMemoryDataProvider.hpp"

#include "booster/data/resource/InMemoryData.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {

std::shared_ptr<data::resource::Resource> InMemoryDataProvider::getResource(const std::shared_ptr<Part>& part) {
  (void)part;
  return std::make_shared<data::resource::InMemoryData>();
}

async::CoroutineStarter InMemoryDataProvider::getResourceAsync(const std::shared_ptr<Part>& part,
                                                               std::shared_ptr<data::resource::Resource>& resource)
{
  (void)part;
  resource = std::make_shared<data::resource::InMemoryData>();
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other functions

std::shared_ptr<PartReader> createInMemoryPartReader(v_io_size maxDataSize) {
  auto provider = std::make_shared<InMemoryDataProvider>();
  auto reader = std::make_shared<StreamPartReader>(provider, maxDataSize);
  return reader;
}

std::shared_ptr<AsyncPartReader> createAsyncInMemoryPartReader(v_io_size maxDataSize) {
  auto provider = std::make_shared<InMemoryDataProvider>();
  auto reader = std::make_shared<AsyncStreamPartReader>(provider, maxDataSize);
  return reader;
}

}}}}
