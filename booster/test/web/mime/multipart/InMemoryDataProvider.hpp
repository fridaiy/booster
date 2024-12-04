

#ifndef booster_web_mime_multipart_InMemoryDataProvider_hpp
#define booster_web_mime_multipart_InMemoryDataProvider_hpp

#include "PartReader.hpp"
#include "Reader.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {

class InMemoryDataProvider : public PartReaderResourceProvider {
public:

  std::shared_ptr<data::resource::Resource> getResource(const std::shared_ptr<Part>& part) override;

  async::CoroutineStarter getResourceAsync(const std::shared_ptr<Part>& part,
                                           std::shared_ptr<data::resource::Resource>& resource) override;

};


std::shared_ptr<PartReader> createInMemoryPartReader(v_io_size maxDataSize);


std::shared_ptr<AsyncPartReader> createAsyncInMemoryPartReader(v_io_size maxDataSize);

}}}}

#endif //booster_web_mime_multipart_InMemoryDataProvider_hpp
