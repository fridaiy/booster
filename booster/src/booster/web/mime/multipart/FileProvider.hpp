

#ifndef booster_web_mime_multipart_FileProvider_hpp
#define booster_web_mime_multipart_FileProvider_hpp

#include "PartReader.hpp"
#include "Reader.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {

class FileProvider : public PartReaderResourceProvider {
private:
  booster::String m_filename;
public:

  FileProvider(const booster::String& filename);

  std::shared_ptr<data::resource::Resource> getResource(const std::shared_ptr<Part>& part) override;

  async::CoroutineStarter getResourceAsync(const std::shared_ptr<Part>& part,
                                           std::shared_ptr<data::resource::Resource>& resource) override;

};


std::shared_ptr<PartReader> createFilePartReader(const booster::String& filename, v_io_size maxDataSize = -1);


std::shared_ptr<AsyncPartReader> createAsyncFilePartReader(const booster::String& filename, v_io_size maxDataSize = -1);

}}}}

#endif //booster_web_mime_multipart_FileProvider_hpp
