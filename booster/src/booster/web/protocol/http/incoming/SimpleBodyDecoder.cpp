

#include "SimpleBodyDecoder.hpp"

#include "booster/web/protocol/http/encoding/Chunked.hpp"
#include "booster/utils/Conversion.hpp"

#include <vector>

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {

const std::string SimpleBodyDecoder::RESPONSE_100_CONTINUE = "HTTP/1.1 100 Continue\r\n\r\n";

SimpleBodyDecoder::SimpleBodyDecoder(const std::shared_ptr<encoding::ProviderCollection>& contentDecoders)
  : m_contentDecoders(contentDecoders)
{}

base::ObjectHandle<data::buffer::Processor>
SimpleBodyDecoder::getStreamProcessor (const data::share::StringKeyLabelCI& transferEncoding,
                                       const data::share::StringKeyLabelCI& contentEncoding ) const
{

  if(!transferEncoding && !contentEncoding) {
    return &data::stream::StatelessDataTransferProcessor::INSTANCE;
  }

  if(contentEncoding && !m_contentDecoders) {
    throw std::runtime_error("[booster::web::protocol::http::incoming::SimpleBodyDecoder::getStreamProcessor()]: "
                             "Error. Server content decoders are NOT configured.");
  }

  std::vector<base::ObjectHandle<data::buffer::Processor>> processors;
  if(transferEncoding) {
    if(transferEncoding != Header::Value::TRANSFER_ENCODING_CHUNKED) {
      throw std::runtime_error("[booster::web::protocol::http::incoming::SimpleBodyDecoder::getStreamProcessor()]: "
                               "Error. Unsupported Transfer-Encoding. '" + transferEncoding.std_str() + "'.");
    }
    processors.push_back(std::make_shared<http::encoding::DecoderChunked>());
  }

  if(contentEncoding) {
    auto provider = m_contentDecoders->get(contentEncoding);
    if(!provider) {
      throw std::runtime_error("[booster::web::protocol::http::incoming::SimpleBodyDecoder::getStreamProcessor()]: "
                               "Error. Unsupported Content-Encoding. '" + contentEncoding.std_str() + "'.");
    }
    processors.push_back(provider->getProcessor());
  }

  if(processors.size() > 1) {
    return std::make_shared<data::buffer::ProcessingPipeline>(processors);
  } else if(processors.size() == 1) {
    return processors[0].getPtr();
  }

  return &data::stream::StatelessDataTransferProcessor::INSTANCE;

}

void SimpleBodyDecoder::handleExpectHeader(const Headers& headers, data::stream::IOStream* connection) const {

  auto expect = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::EXPECT);
  if(expect == Header::Value::EXPECT_100_CONTINUE) {
    auto res = connection->writeExactSizeDataSimple(RESPONSE_100_CONTINUE.data(), static_cast<v_buff_size>(RESPONSE_100_CONTINUE.size()));
    if(res != static_cast<v_io_size>(RESPONSE_100_CONTINUE.size())) {
      throw std::runtime_error("[booster::web::protocol::http::incoming::SimpleBodyDecoder::handleExpectHeader()]: "
                               "Error. Unable to send 100-continue response.");
    }
  }

}

booster::async::CoroutineStarter SimpleBodyDecoder::handleExpectHeaderAsync(const Headers& headers,
                                                                          const std::shared_ptr<data::stream::IOStream>& connection) const
{
  auto expect = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::EXPECT);
  if(expect == Header::Value::EXPECT_100_CONTINUE) {
    return connection->writeExactSizeDataAsync(RESPONSE_100_CONTINUE.data(), static_cast<v_buff_size>(RESPONSE_100_CONTINUE.size()));
  }
  return nullptr;
}

void SimpleBodyDecoder::decode(const Headers& headers,
                               data::stream::InputStream* bodyStream,
                               data::stream::WriteCallback* writeCallback,
                               data::stream::IOStream* connection) const
{

  handleExpectHeader(headers, connection);
  auto transferEncoding = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::TRANSFER_ENCODING);

  if(transferEncoding) {

    auto contentEncoding = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::CONTENT_ENCODING);
    auto processor = getStreamProcessor(transferEncoding, contentEncoding);

    data::buffer::IOBuffer buffer;
    data::stream::transfer(bodyStream, writeCallback, 0 , buffer.getData(), buffer.getSize(), processor);

  } else {

    auto contentLengthStr = headers.getAsMemoryLabel<data::share::StringKeyLabel>(Header::CONTENT_LENGTH);
    if(contentLengthStr) {

      bool success;
      auto contentLength = utils::Conversion::strToInt64(contentLengthStr.toString(), success);

      if (success && contentLength > 0) {

        auto contentEncoding = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::CONTENT_ENCODING);
        auto processor = getStreamProcessor(nullptr, contentEncoding);
        data::buffer::IOBuffer buffer;
        data::stream::transfer(bodyStream, writeCallback, contentLength, buffer.getData(), buffer.getSize(), processor);

      }

    } else {

      auto connectionStr = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::CONNECTION);

      if(connectionStr && connectionStr == "close") {

        auto contentEncoding = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::CONTENT_ENCODING);
        auto processor = getStreamProcessor(nullptr, contentEncoding);
        data::buffer::IOBuffer buffer;
        data::stream::transfer(bodyStream, writeCallback,  0 , buffer.getData(), buffer.getSize(), processor);

      } else {
        throw std::runtime_error("[booster::web::protocol::http::incoming::SimpleBodyDecoder::decode()]: Error. Invalid Request.");
      }

    }

  }

}

async::CoroutineStarter SimpleBodyDecoder::decodeAsync(const Headers& headers,
                                                       const std::shared_ptr<data::stream::InputStream>& bodyStream,
                                                       const std::shared_ptr<data::stream::WriteCallback>& writeCallback,
                                                       const std::shared_ptr<data::stream::IOStream>& connection) const
{

  auto pipeline = handleExpectHeaderAsync(headers, connection);
  auto transferEncoding = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::TRANSFER_ENCODING);

  if(transferEncoding) {

    auto contentEncoding = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::CONTENT_ENCODING);
    auto processor = getStreamProcessor(transferEncoding, contentEncoding);
    auto buffer = data::buffer::IOBuffer::createShared();
    return std::move(pipeline.next(data::stream::transferAsync(bodyStream, writeCallback, 0 , buffer, processor)));

  } else {

    auto contentLengthStr = headers.getAsMemoryLabel<data::share::StringKeyLabel>(Header::CONTENT_LENGTH);
    if(contentLengthStr) {

      bool success;
      auto contentLength = utils::Conversion::strToInt64(contentLengthStr.toString(), success);

      if (success && contentLength > 0) {

        auto contentEncoding = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::CONTENT_ENCODING);
        auto processor = getStreamProcessor(nullptr, contentEncoding);
        auto buffer = data::buffer::IOBuffer::createShared();
        return std::move(pipeline.next(data::stream::transferAsync(bodyStream, writeCallback, contentLength, buffer, processor)));

      }

    } else {

      auto connectionStr = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::CONNECTION);

      if(connectionStr && connectionStr == "close") {

        auto contentEncoding = headers.getAsMemoryLabel<data::share::StringKeyLabelCI>(Header::CONTENT_ENCODING);
        auto processor = getStreamProcessor(nullptr, contentEncoding);
        auto buffer = data::buffer::IOBuffer::createShared();
        return std::move(pipeline.next(data::stream::transferAsync(bodyStream, writeCallback,  0 , buffer, processor)));

      }

    }

  }

  return std::move(pipeline);

}
  
}}}}}
