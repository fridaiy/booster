

#include "Request.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {

Request::Request(const std::shared_ptr<booster::data::stream::IOStream>& connection,
                 const http::RequestStartingLine& startingLine,
                 const http::Headers& headers,
                 const std::shared_ptr<booster::data::stream::InputStream>& bodyStream,
                 const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder)
  : m_connection(connection)
  , m_startingLine(startingLine)
  , m_headers(headers)
  , m_bodyStream(bodyStream)
  , m_bodyDecoder(bodyDecoder)
  , m_queryParamsParsed(false)
{}

std::shared_ptr<Request> Request::createShared(const std::shared_ptr<booster::data::stream::IOStream>& connection,
                                               const http::RequestStartingLine& startingLine,
                                               const http::Headers& headers,
                                               const std::shared_ptr<booster::data::stream::InputStream>& bodyStream,
                                               const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder)
{
  return std::make_shared<Request>(connection, startingLine, headers, bodyStream, bodyDecoder);
}

std::shared_ptr<booster::data::stream::IOStream> Request::getConnection() {
  return m_connection;
}

const http::RequestStartingLine& Request::getStartingLine() const {
  return m_startingLine;
}

void Request::setPathVariables(const url::mapping::Pattern::MatchMap& pathVariables) {
  m_pathVariables = pathVariables;
}

const url::mapping::Pattern::MatchMap& Request::getPathVariables() const {
  return m_pathVariables;
}

const http::Headers& Request::getHeaders() const {
  return m_headers;
}

const http::QueryParams& Request::getQueryParameters() const {
  if(!m_queryParamsParsed) {
    booster::network::Url::Parser::parseQueryParams(m_queryParams, m_pathVariables.getTail());
    m_queryParamsParsed = true;
  }
  return m_queryParams;
}

booster::String Request::getQueryParameter(const booster::data::share::StringKeyLabel& name) const {
  return getQueryParameters().get(name);
}

booster::String Request::getQueryParameter(const booster::data::share::StringKeyLabel& name, const booster::String& defaultValue) const {
  auto value = getQueryParameter(name);
  return value ? value : defaultValue;
}

std::shared_ptr<booster::data::stream::InputStream> Request::getBodyStream() const {
  return m_bodyStream;
}

std::shared_ptr<const http::incoming::BodyDecoder> Request::getBodyDecoder() const {
  return m_bodyDecoder;
}

void Request::putHeader(const booster::String& key, const booster::String& value) {
  m_headers.put(key, value);
}

bool Request::putHeaderIfNotExists(const booster::String& key, const booster::String& value) {
  return m_headers.putIfNotExists(key, value);
}

bool Request::putOrReplaceHeader(const String &key, const String &value) {
  return m_headers.putOrReplace(key, value);
}

bool Request::putOrReplaceHeader_Unsafe(const data::share::StringKeyLabelCI& key,
                                        const data::share::StringKeyLabel &value) {
  return m_headers.putOrReplace(key, value);
}

void Request::putHeader_Unsafe(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value) {
  m_headers.put(key, value);
}

bool Request::putHeaderIfNotExists_Unsafe(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value) {
  return m_headers.putIfNotExists(key, value);
}

booster::String Request::getHeader(const booster::data::share::StringKeyLabelCI& headerName) const{
  return m_headers.get(headerName);
}

std::vector<booster::String> Request::getHeaderValues(const booster::data::share::StringKeyLabelCI& headerName) const {
  std::vector<booster::String> result;
  auto range = m_headers.getAll_Unsafe().equal_range(headerName);
  for (auto it = range.first; it != range.second; ++it) {
    result.emplace_back(it->second.toString());
  }
  return result;
}

booster::String Request::getPathVariable(const booster::data::share::StringKeyLabel& name) const {
  return m_pathVariables.getVariable(name);
}

booster::String Request::getPathTail() const {
  return m_pathVariables.getTail();
}

void Request::putBundleData(const booster::String& key, const booster::Void& polymorph) {
  m_bundle.put(key, polymorph);
}

const data::Bundle& Request::getBundle() const {
  return m_bundle;
}

void Request::transferBody(const base::ObjectHandle<data::stream::WriteCallback>& writeCallback) const {
  m_bodyDecoder->decode(m_headers, m_bodyStream.get(), writeCallback.get(), m_connection.get());
}

void Request::transferBodyToStream(const base::ObjectHandle<booster::data::stream::OutputStream>& toStream) const {
  m_bodyDecoder->decode(m_headers, m_bodyStream.get(), toStream.get(), m_connection.get());
}

booster::String Request::readBodyToString() const {
  return m_bodyDecoder->decodeToString(m_headers, m_bodyStream.get(), m_connection.get());
}

async::CoroutineStarter Request::transferBodyAsync(const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const {
  return m_bodyDecoder->decodeAsync(m_headers, m_bodyStream, writeCallback, m_connection);
}

async::CoroutineStarter Request::transferBodyToStreamAsync(const std::shared_ptr<booster::data::stream::OutputStream>& toStream) const {
  return m_bodyDecoder->decodeAsync(m_headers, m_bodyStream, toStream, m_connection);
}

async::CoroutineStarterForResult<const booster::String&> Request::readBodyToStringAsync() const {
  return m_bodyDecoder->decodeToStringAsync(m_headers, m_bodyStream, m_connection);
}

}}}}}
