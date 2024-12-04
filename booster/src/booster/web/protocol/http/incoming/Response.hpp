

#ifndef booster_web_protocol_http_incoming_Response_hpp
#define booster_web_protocol_http_incoming_Response_hpp

#include "booster/web/protocol/http/Http.hpp"
#include "booster/web/protocol/http/incoming/BodyDecoder.hpp"
#include "booster/data/Bundle.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {


class Response : public booster::base::Countable {
private:
  v_int32 m_statusCode;
  booster::String m_statusDescription;
  http::Headers m_headers;
  std::shared_ptr<booster::data::stream::InputStream> m_bodyStream;
  

  std::shared_ptr<const http::incoming::BodyDecoder> m_bodyDecoder;
  
  std::shared_ptr<booster::data::stream::IOStream> m_connection;

  data::Bundle m_bundle;
  
public:

  Response(v_int32 statusCode,
           const booster::String& statusDescription,
           const http::Headers& headers,
           const std::shared_ptr<booster::data::stream::InputStream>& bodyStream,
           const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);
public:


  static std::shared_ptr<Response> createShared(v_int32 statusCode,
                                                const booster::String& statusDescription,
                                                const http::Headers& headers,
                                                const std::shared_ptr<booster::data::stream::InputStream>& bodyStream,
                                                const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);


  v_int32 getStatusCode() const;


  booster::String getStatusDescription() const;


  const http::Headers& getHeaders() const;


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


  std::shared_ptr<data::stream::InputStream> getBodyStream() const;


  std::shared_ptr<const http::incoming::BodyDecoder> getBodyDecoder() const;


  void transferBody(const base::ObjectHandle<data::stream::WriteCallback>& writeCallback) const;


  void transferBodyToStream(const base::ObjectHandle<data::stream::OutputStream>& toStream) const;


  booster::String readBodyToString() const;


  template<class Wrapper>
  Wrapper readBodyToDto(const base::ObjectHandle<data::mapping::ObjectMapper>& objectMapper) const {
    return m_bodyDecoder->decodeToDto<Wrapper>(m_headers, m_bodyStream.get(), m_connection.get(), objectMapper.get());
  }
  
  // Async


  async::CoroutineStarter transferBodyAsync(const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const;



  booster::async::CoroutineStarter transferBodyToStreamAsync(const std::shared_ptr<data::stream::OutputStream>& toStream) const;


  booster::async::CoroutineStarterForResult<const booster::String&> readBodyToStringAsync() const {
    return m_bodyDecoder->decodeToStringAsync(m_headers, m_bodyStream, m_connection);
  }


  template<class Wrapper>
  booster::async::CoroutineStarterForResult<const Wrapper&>
  readBodyToDtoAsync(const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper) const {
    return m_bodyDecoder->decodeToDtoAsync<Wrapper>(m_headers, m_bodyStream, m_connection, objectMapper);
  }
  
};
  
}}}}}

#endif