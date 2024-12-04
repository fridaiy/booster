

#ifndef booster_web_protocol_http_incoming_SimpleBodyDecoder_hpp
#define booster_web_protocol_http_incoming_SimpleBodyDecoder_hpp

#include "BodyDecoder.hpp"
#include "booster/web/protocol/http/encoding/ProviderCollection.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {


class SimpleBodyDecoder : public BodyDecoder {
private:
  static const std::string RESPONSE_100_CONTINUE;
private:
  std::shared_ptr<encoding::ProviderCollection> m_contentDecoders;
private:
  base::ObjectHandle<data::buffer::Processor> getStreamProcessor(const data::share::StringKeyLabelCI& transferEncoding,
                                                                 const data::share::StringKeyLabelCI& contentEncoding) const;
  void handleExpectHeader(const Headers& headers, data::stream::IOStream* connection) const;
  booster::async::CoroutineStarter handleExpectHeaderAsync(const Headers& headers,
                                                         const std::shared_ptr<data::stream::IOStream>& connection) const;
public:


  SimpleBodyDecoder(const std::shared_ptr<encoding::ProviderCollection>& contentDecoders = nullptr);


  void decode(const Headers& headers,
              data::stream::InputStream* bodyStream,
              data::stream::WriteCallback* writeCallback,
              data::stream::IOStream* connection) const override;


  booster::async::CoroutineStarter decodeAsync(const Headers& headers,
                                             const std::shared_ptr<data::stream::InputStream>& bodyStream,
                                             const std::shared_ptr<data::stream::WriteCallback>& writeCallback,
                                             const std::shared_ptr<data::stream::IOStream>& connection) const override;
  
  
};
  
}}}}}

#endif
