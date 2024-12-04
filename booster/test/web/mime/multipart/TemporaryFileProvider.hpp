

#ifndef booster_web_mime_multipart_TemporaryFileProvider_hpp
#define booster_web_mime_multipart_TemporaryFileProvider_hpp

#include "PartReader.hpp"
#include "Reader.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {

class TemporaryFileProvider : public PartReaderResourceProvider {
private:
  booster::String m_tmpDirectory;
  v_int32 m_randomWordSizeBytes;
public:

  TemporaryFileProvider(const booster::String& tmpDirectory, v_int32 randomWordSizeBytes = 8);

  std::shared_ptr<data::resource::Resource> getResource(const std::shared_ptr<Part>& part) override;

  async::CoroutineStarter getResourceAsync(const std::shared_ptr<Part>& part,
                                           std::shared_ptr<data::resource::Resource>& resource) override;

};


std::shared_ptr<PartReader> createTemporaryFilePartReader(const booster::String& tmpDirectory,
                                                          v_int32 randomWordSizeBytes = 8,
                                                          v_io_size maxDataSize = -1);


std::shared_ptr<AsyncPartReader> createAsyncTemporaryFilePartReader(const booster::String& tmpDirectory,
                                                                    v_int32 randomWordSizeBytes = 8,
                                                                    v_io_size maxDataSize = -1);

}}}}

#endif //booster_web_mime_multipart_TemporaryFileProvider_hpp
