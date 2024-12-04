

#ifndef booster_web_protocol_http_outgoing_Request_hpp
#define booster_web_protocol_http_outgoing_Request_hpp

#include "booster/web/protocol/http/outgoing/Body.hpp"
#include "booster/web/protocol/http/Http.hpp"
#include "booster/data/Bundle.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace outgoing {


class Request : public booster::base::Countable {
public:

  typedef protocol::http::Headers Headers;
private:
  booster::data::share::StringKeyLabel m_method;
  booster::data::share::StringKeyLabel m_path;
  Headers m_headers;
  std::shared_ptr<Body> m_body;
  data::Bundle m_bundle;
public:


  Request(const booster::data::share::StringKeyLabel& method,
          const booster::data::share::StringKeyLabel& path,
          const Headers& headers,
          const std::shared_ptr<Body>& body);
  
public:


  static std::shared_ptr<Request> createShared(const booster::data::share::StringKeyLabel& method,
                                               const booster::data::share::StringKeyLabel& path,
                                               const Headers& headers,
                                               const std::shared_ptr<Body>& body);


  const booster::data::share::StringKeyLabel& getMethod() const;


  const booster::data::share::StringKeyLabel& getPath() const;


  Headers& getHeaders();


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


  std::shared_ptr<Body> getBody();


  void send(data::stream::OutputStream* stream);


  static booster::async::CoroutineStarter sendAsync(std::shared_ptr<Request> _this,
                                                  const std::shared_ptr<data::stream::OutputStream>& stream);
  
};
  
}}}}}

#endif
