

#include "Part.hpp"

#include "booster/web/protocol/http/Http.hpp"
#include "booster/utils/parser/Caret.hpp"

#include <cstring>

namespace booster { namespace web { namespace mime { namespace multipart {

Part::Part(const Headers &headers, const std::shared_ptr<data::resource::Resource>& payload)
  : m_headers(headers)
  , m_payload(payload)
{

  auto contentDisposition = m_headers.getAsMemoryLabel<booster::data::share::StringKeyLabel>("Content-Disposition");
  if(contentDisposition) {

    booster::web::protocol::http::HeaderValueData valueData;
    booster::web::protocol::http::Parser::parseHeaderValueData(valueData, contentDisposition, ';');

    m_name = valueData.getTitleParamValue("name");
    m_filename = valueData.getTitleParamValue("filename");

  }

}

void Part::setPayload(const std::shared_ptr<data::resource::Resource>& payload) {
  m_payload = payload;
}

std::shared_ptr<data::resource::Resource> Part::getPayload() {
  return m_payload;
}

booster::String Part::getName() const {
  return m_name;
}


booster::String Part::getFilename() const {
  return m_filename;
}


const Part::Headers& Part::getHeaders() const {
  return m_headers;
}

booster::String Part::getHeader(const booster::data::share::StringKeyLabelCI& headerName) const {
  return m_headers.get(headerName);
}

void Part::putHeader(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value) {
  m_headers.put(key, value);
}

bool Part::putHeaderIfNotExists(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value) {
  return m_headers.putIfNotExists(key, value);
}

void Part::setTag(const char* tagName, const std::shared_ptr<booster::base::Countable>& tagObject) {
  m_tagName = tagName;
  m_tagObject = tagObject;
}

const char* Part::getTagName() {
  return m_tagName;
}

std::shared_ptr<booster::base::Countable> Part::getTagObject() {
  return m_tagObject;
}

void Part::clearTag() {
  m_tagName = nullptr;
  m_tagObject.reset();
}

}}}}
