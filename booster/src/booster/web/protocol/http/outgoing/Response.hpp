

#ifndef booster_web_protocol_http_outgoing_Response_hpp
#define booster_web_protocol_http_outgoing_Response_hpp

#include "booster/web/protocol/http/outgoing/Body.hpp"
#include "booster/web/protocol/http/encoding/EncoderProvider.hpp"
#include "booster/web/protocol/http/Http.hpp"

#include "booster/network/ConnectionHandler.hpp"

#include "booster/async/Coroutine.hpp"
#include "booster/data/stream/BufferStream.hpp"

#include "booster/data/Bundle.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace outgoing {


class Response : public booster::base::Countable {
public:

  typedef http::Headers Headers;


  typedef booster::network::ConnectionHandler ConnectionHandler;
private:
  Status m_status;
  Headers m_headers;
  std::shared_ptr<Body> m_body;
  std::shared_ptr<ConnectionHandler> m_connectionUpgradeHandler;
  std::shared_ptr<const ConnectionHandler::ParameterMap> m_connectionUpgradeParameters;
  data::Bundle m_bundle;
public:

  Response(const Status& status, const std::shared_ptr<Body>& body);
public:


  static std::shared_ptr<Response> createShared(const Status& status, const std::shared_ptr<Body>& body);


  const Status& getStatus() const;


  Headers& getHeaders();


  std::shared_ptr<Body> getBody() const;


  void putHeader(const booster::String& key, const booster::String& value);


  bool putHeaderIfNotExists(const booster::String& key, const booster::String& value);


  bool putOrReplaceHeader(const booster::String& key, const booster::String& value);


  bool putOrReplaceHeader_Unsafe(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value);


  void putHeader_Unsafe(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value);


  bool putHeaderIfNotExists_Unsafe(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value);


  booster::String getHeader(const booster::data::share::StringKeyLabelCI& headerName) const;


  void putBundleData(const booster::String& key, const booster::Void& polymorph);


  template<typename WrapperType>
  WrapperType getBundleData(const booster::String& key) const {
    return m_bundle.template get<WrapperType>(key);
  }


  const data::Bundle& getBundle() const;


  void setConnectionUpgradeHandler(const std::shared_ptr<ConnectionHandler>& handler);


  std::shared_ptr<ConnectionHandler> getConnectionUpgradeHandler();


  void setConnectionUpgradeParameters(const std::shared_ptr<const ConnectionHandler::ParameterMap>& parameters);


  std::shared_ptr<const ConnectionHandler::ParameterMap> getConnectionUpgradeParameters();


  void send(data::stream::OutputStream* stream,
            data::stream::BufferOutputStream* headersWriteBuffer,
            http::encoding::EncoderProvider* contentEncoderProvider);


  static booster::async::CoroutineStarter sendAsync(const std::shared_ptr<Response>& _this,
                                                  const std::shared_ptr<data::stream::OutputStream>& stream,
                                                  const std::shared_ptr<data::stream::BufferOutputStream>& headersWriteBuffer,
                                                  const std::shared_ptr<http::encoding::EncoderProvider>& contentEncoderProvider);
  
};
  
}}}}}

#endif
