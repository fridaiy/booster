

#ifndef booster_web_mime_multipart_PartReader_hpp
#define booster_web_mime_multipart_PartReader_hpp

#include "./Multipart.hpp"
#include "booster/data/stream/Stream.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {


class PartReader {
public:


  virtual ~PartReader() = default;


  virtual void onNewPart(const std::shared_ptr<Part>& part) = 0;


  virtual void onPartData(const std::shared_ptr<Part>& part, const char* data, booster::v_io_size size) = 0;

};


class AsyncPartReader {
public:


  virtual ~AsyncPartReader() = default;


  virtual async::CoroutineStarter onNewPartAsync(const std::shared_ptr<Part>& part) = 0;


  virtual async::CoroutineStarter onPartDataAsync(const std::shared_ptr<Part>& part, const char* data, booster::v_io_size size) = 0;

};


class PartReaderResourceProvider {
public:


  virtual ~PartReaderResourceProvider() = default;


  virtual std::shared_ptr<data::resource::Resource> getResource(const std::shared_ptr<Part>& part) = 0;


  virtual async::CoroutineStarter getResourceAsync(const std::shared_ptr<Part>& part,
                                                   std::shared_ptr<data::resource::Resource>& resource) = 0;

};


class StreamPartReader : public PartReader {
private:
  static const char* const TAG_NAME;
private:

  class TagObject : public booster::base::Countable {
  public:
    v_io_size size = 0;
    std::shared_ptr<data::resource::Resource> resource;
    std::shared_ptr<data::stream::OutputStream> outputStream;
  };

private:
  std::shared_ptr<PartReaderResourceProvider> m_resourceProvider;
  v_io_size m_maxDataSize;
public:


  StreamPartReader(const std::shared_ptr<PartReaderResourceProvider>& resourceProvider, v_io_size maxDataSize = -1);


  void onNewPart(const std::shared_ptr<Part>& part) override;


  void onPartData(const std::shared_ptr<Part>& part, const char* data, booster::v_io_size size) override;

};


class AsyncStreamPartReader : public AsyncPartReader {
private:
  static const char* const TAG_NAME;
private:

  class TagObject : public booster::base::Countable {
  public:
    v_io_size size = 0;
    std::shared_ptr<data::resource::Resource> resource;
    std::shared_ptr<data::stream::OutputStream> outputStream;
  };

private:
  std::shared_ptr<PartReaderResourceProvider> m_resourceProvider;
  v_io_size m_maxDataSize;
public:


  AsyncStreamPartReader(const std::shared_ptr<PartReaderResourceProvider>& resourceProvider, v_io_size maxDataSize = -1);


  async::CoroutineStarter onNewPartAsync(const std::shared_ptr<Part>& part) override;


  async::CoroutineStarter onPartDataAsync(const std::shared_ptr<Part>& part, const char* data, booster::v_io_size size) override;

};

}}}}

#endif //booster_web_mime_multipart_PartReader_hpp
