

#ifndef booster_web_protocol_http_incoming_Request_hpp
#define booster_web_protocol_http_incoming_Request_hpp

#include "booster/web/protocol/http/Http.hpp"
#include "booster/web/protocol/http/incoming/BodyDecoder.hpp"
#include "booster/web/url/mapping/Pattern.hpp"
#include "booster/network/Url.hpp"
#include "booster/data/Bundle.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {


class Request : public booster::base::Countable {
private:

  std::shared_ptr<booster::data::stream::IOStream> m_connection;
  http::RequestStartingLine m_startingLine;
  url::mapping::Pattern::MatchMap m_pathVariables;
  http::Headers m_headers;
  std::shared_ptr<booster::data::stream::InputStream> m_bodyStream;
  

  std::shared_ptr<const http::incoming::BodyDecoder> m_bodyDecoder;

  mutable bool m_queryParamsParsed; // used for lazy parsing of QueryParams
  mutable http::QueryParams m_queryParams;

  data::Bundle m_bundle;

public:
  
  Request(const std::shared_ptr<booster::data::stream::IOStream>& connection,
          const http::RequestStartingLine& startingLine,
          const http::Headers& headers,
          const std::shared_ptr<booster::data::stream::InputStream>& bodyStream,
          const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);
public:
  
  static std::shared_ptr<Request> createShared(const std::shared_ptr<booster::data::stream::IOStream>& connection,
                                               const http::RequestStartingLine& startingLine,
                                               const http::Headers& headers,
                                               const std::shared_ptr<booster::data::stream::InputStream>& bodyStream,
                                               const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);


  std::shared_ptr<booster::data::stream::IOStream> getConnection();


  const http::QueryParams& getQueryParameters() const;


  booster::String getQueryParameter(const booster::data::share::StringKeyLabel& name) const;


  booster::String getQueryParameter(const booster::data::share::StringKeyLabel& name, const booster::String& defaultValue) const;


  const http::RequestStartingLine& getStartingLine() const;


  void setPathVariables(const url::mapping::Pattern::MatchMap& pathVariables);


  const url::mapping::Pattern::MatchMap& getPathVariables() const;


  const http::Headers& getHeaders() const;


  std::shared_ptr<booster::data::stream::InputStream> getBodyStream() const;


  std::shared_ptr<const http::incoming::BodyDecoder> getBodyDecoder() const;


  void putHeader(const booster::String& key, const booster::String& value);


  bool putHeaderIfNotExists(const booster::String& key, const booster::String& value);


  bool putOrReplaceHeader(const booster::String& key, const booster::String& value);


  bool putOrReplaceHeader_Unsafe(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value);


  void putHeader_Unsafe(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value);


  bool putHeaderIfNotExists_Unsafe(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value);


  booster::String getHeader(const booster::data::share::StringKeyLabelCI& headerName) const;


  std::vector<booster::String> getHeaderValues(const booster::data::share::StringKeyLabelCI& headerName) const;


  booster::String getPathVariable(const booster::data::share::StringKeyLabel& name) const;


  booster::String getPathTail() const;


  void putBundleData(const booster::String& key, const booster::Void& polymorph);


  template<typename WrapperType>
  WrapperType getBundleData(const booster::String& key) const {
    return m_bundle.template get<WrapperType>(key);
  }


  const data::Bundle& getBundle() const;


  void transferBody(const base::ObjectHandle<data::stream::WriteCallback>& writeCallback) const;


  void transferBodyToStream(const base::ObjectHandle<data::stream::OutputStream>& toStream) const;


  booster::String readBodyToString() const;


  template<class Wrapper>
  Wrapper readBodyToDto(const base::ObjectHandle<data::mapping::ObjectMapper>& objectMapper) const {
    return objectMapper->readFromString<Wrapper>(m_bodyDecoder->decodeToString(m_headers, m_bodyStream.get(), m_connection.get()));
  }
  
  // Async


  async::CoroutineStarter transferBodyAsync(const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const;


  booster::async::CoroutineStarter transferBodyToStreamAsync(const std::shared_ptr<booster::data::stream::OutputStream>& toStream) const;


  async::CoroutineStarterForResult<const booster::String&> readBodyToStringAsync() const;


  template<class Wrapper>
  booster::async::CoroutineStarterForResult<const Wrapper&>
  readBodyToDtoAsync(const std::shared_ptr<booster::data::mapping::ObjectMapper>& objectMapper) const {
    return m_bodyDecoder->decodeToDtoAsync<Wrapper>(m_headers, m_bodyStream, m_connection, objectMapper);
  }
  
};
  
}}}}}

#endif

