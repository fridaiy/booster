

#include "PartReader.hpp"
#include "booster/base/Log.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {

const char* const StreamPartReader::TAG_NAME = "[booster::web::mime::multipart::StreamPartReader::TAG]";

StreamPartReader::StreamPartReader(const std::shared_ptr<PartReaderResourceProvider>& resourceProvider,
                                   v_io_size maxDataSize)
  : m_resourceProvider(resourceProvider)
  , m_maxDataSize(maxDataSize)
{}

void StreamPartReader::onNewPart(const std::shared_ptr<Part>& part) {

  if(!m_resourceProvider) {
    throw std::runtime_error("[booster::web::mime::multipart::StreamPartReader::onNewPart()]: Error. Stream provider is nullptr.");
  }

  auto tag = part->getTagObject();

  if(tag) {
    throw std::runtime_error("[booster::web::mime::multipart::StreamPartReader::onNewPart()]: Error. "
                             "Part tag object is not nullptr. Seems like this part is already being processed by another part reader.");
  }

  auto tagObject = std::make_shared<TagObject>();
  tagObject->resource = m_resourceProvider->getResource(part);
  tagObject->outputStream = tagObject->resource->openOutputStream();
  part->setTag(TAG_NAME, tagObject);

}

void StreamPartReader::onPartData(const std::shared_ptr<Part>& part, const char* data, booster::v_io_size size) {

  auto tag = part->getTagObject();
  if(!tag) {
    throw std::runtime_error("[booster::web::mime::multipart::StreamPartReader::onPartData()]: Error. "
                             "Part tag object is nullptr.");

  }

  if(part->getTagName() != TAG_NAME) {
    throw std::runtime_error("[booster::web::mime::multipart::StreamPartReader::onPartData()]: Error. "
                             "Wrong tag name. Seems like this part is already being processed by another part reader.");

  }

  auto tagObject = std::static_pointer_cast<TagObject>(tag);

  if(size > 0) {
    if(m_maxDataSize > 0 && tagObject->size + size > m_maxDataSize) {
      BOOSTER_LOGe("[booster::web::mime::multipart::StreamPartReader::onPartData()]", "Error. Part size exceeds specified maxDataSize={}", m_maxDataSize)
      throw std::runtime_error("[booster::web::mime::multipart::StreamPartReader::onPartData()]: Error. Part size exceeds specified maxDataSize");
    }
    auto res = tagObject->outputStream->writeExactSizeDataSimple(data, size);
    if(res != size) {
      BOOSTER_LOGe("[booster::web::mime::multipart::StreamPartReader::onPartData()]", "Error. Failed to stream all data. Streamed {}/{}", res, size)
      throw std::runtime_error("[booster::web::mime::multipart::StreamPartReader::onPartData()]: Error. Failed to stream all data.");
    }
    tagObject->size += res;
  } else {
    part->setPayload(tagObject->resource);
    part->clearTag();
  }

}


const char* const AsyncStreamPartReader::TAG_NAME = "[booster::web::mime::multipart::AsyncStreamPartReader::TAG]";

AsyncStreamPartReader::AsyncStreamPartReader(const std::shared_ptr<PartReaderResourceProvider>& resourceProvider,
                                             v_io_size maxDataSize)
  : m_resourceProvider(resourceProvider)
  , m_maxDataSize(maxDataSize)
{}

async::CoroutineStarter AsyncStreamPartReader::onNewPartAsync(const std::shared_ptr<Part>& part) {

  class OnNewPartCoroutine : public async::Coroutine<OnNewPartCoroutine> {
  private:
    std::shared_ptr<Part> m_part;
    std::shared_ptr<PartReaderResourceProvider> m_resourceProvider;
    std::shared_ptr<data::resource::Resource> m_obtainedResource;
  public:

    OnNewPartCoroutine(const std::shared_ptr<Part>& part,
                       const std::shared_ptr<PartReaderResourceProvider>& resourceProvider)
      : m_part(part)
      , m_resourceProvider(resourceProvider)
    {}

    Action act() override {

      if(!m_resourceProvider) {
        throw std::runtime_error("[booster::web::mime::multipart::AsyncStreamPartReader::onNewPartAsync(){OnNewPartCoroutine}]: Error. Stream provider is nullptr.");
      }

      auto tag = m_part->getTagObject();

      if(tag) {
        throw std::runtime_error("[booster::web::mime::multipart::AsyncStreamPartReader::onNewPartAsync(){OnNewPartCoroutine}]: Error. "
                                 "Part tag object is not nullptr. Seems like this part is already being processed by another part reader.");
      }

      return m_resourceProvider->getResourceAsync(m_part, m_obtainedResource).next(yieldTo(&OnNewPartCoroutine::onResourceObtained));

    }

    Action onResourceObtained() {
      auto tagObject = std::make_shared<TagObject>();
      tagObject->resource = m_obtainedResource;
      tagObject->outputStream = m_obtainedResource->openOutputStream();
      m_part->setTag(TAG_NAME, tagObject);
      return finish();
    }

  };

  return OnNewPartCoroutine::start(part, m_resourceProvider);

}

async::CoroutineStarter AsyncStreamPartReader::onPartDataAsync(const std::shared_ptr<Part>& part, const char* data, booster::v_io_size size) {

  auto tag = part->getTagObject();
  if(!tag) {
    throw std::runtime_error("[booster::web::mime::multipart::AsyncStreamPartReader::onPartDataAsync()]: Error. "
                             "Part tag object is nullptr.");

  }

  if(part->getTagName() != TAG_NAME) {
    throw std::runtime_error("[booster::web::mime::multipart::AsyncStreamPartReader::onPartDataAsync()]: Error. "
                             "Wrong tag name. Seems like this part is already being processed by another part reader.");

  }

  auto tagObject = std::static_pointer_cast<TagObject>(tag);

  if(size > 0) {
    if(m_maxDataSize > 0 && tagObject->size + size > m_maxDataSize) {
      BOOSTER_LOGe("[booster::web::mime::multipart::AsyncStreamPartReader::onPartDataAsync()]", "Error. Part size exceeds specified maxDataSize={}", m_maxDataSize)
      throw std::runtime_error("[booster::web::mime::multipart::AsyncStreamPartReader::onPartDataAsync()]: Error. Part size exceeds specified maxDataSize");
    }
    return tagObject->outputStream->writeExactSizeDataAsync(data, size);
  } else {
    part->setPayload(tagObject->resource);
    part->clearTag();
    return nullptr;
  }

}

}}}}
