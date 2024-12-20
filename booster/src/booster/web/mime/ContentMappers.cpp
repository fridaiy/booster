

#include "ContentMappers.hpp"

#include "booster/utils/parser/Caret.hpp"
#include "booster/utils/Conversion.hpp"

#include <algorithm>

namespace booster::web::mime {

std::pair<booster::String, booster::String> ContentMappers::typeAndSubtype(const data::share::StringKeyLabelCI& contentType) const {

  if(contentType == nullptr || contentType.getSize() == 0) return {};

  utils::parser::Caret caret(reinterpret_cast<const char*>(contentType.getData()), contentType.getSize());
  auto typeL = caret.putLabel();

  if(!caret.findChar('/')) return {};

  typeL.end();

  caret.canContinueAtChar('/', 1);

  auto stypeL = caret.putLabel();

  caret.findCharFromSet(";, \r\n\t");

  return {typeL.toString(), stypeL.toString()};

}

void ContentMappers::putMapper(const std::shared_ptr<data::mapping::ObjectMapper>& mapper) {
  std::unique_lock<std::shared_mutex> lock(m_mutex);
  if(m_defaultMapper == nullptr) {
    m_defaultMapper = mapper;
  }
  m_index[mapper->getInfo().mimeType][mapper->getInfo().mimeSubtype] = mapper;
  m_mappers[mapper->getInfo().httpContentType] = mapper;
}

void ContentMappers::setDefaultMapper(const booster::String& contentType) {
  std::unique_lock<std::shared_mutex> lock(m_mutex);
  m_defaultMapper = m_mappers.at(contentType);
}

void ContentMappers::setDefaultMapper(const std::shared_ptr<data::mapping::ObjectMapper>& mapper) {
  std::unique_lock<std::shared_mutex> lock(m_mutex);
  m_defaultMapper = mapper;
  if(m_defaultMapper) {
    m_index[m_defaultMapper->getInfo().mimeType][m_defaultMapper->getInfo().mimeSubtype] = m_defaultMapper;
    m_mappers[mapper->getInfo().httpContentType] = m_defaultMapper;
  }
}

std::shared_ptr<data::mapping::ObjectMapper> ContentMappers::getMapper(const booster::String& contentType) const {
  std::shared_lock<std::shared_mutex> lock(m_mutex);
  auto it = m_mappers.find(contentType);
  if(it == m_mappers.end()) {
    return nullptr;
  }
  return it->second;
}

std::shared_ptr<data::mapping::ObjectMapper> ContentMappers::getDefaultMapper() const {
  std::shared_lock<std::shared_mutex> lock(m_mutex);
  return m_defaultMapper;
}

std::shared_ptr<data::mapping::ObjectMapper> ContentMappers::selectMapper(const protocol::http::HeaderValueData& values) const {

  for(auto& t : values.tokens) {

    if(t == "*/*") return m_defaultMapper;

    auto tst = typeAndSubtype(t);
    if(!tst.first || !tst.second) continue;

    auto it = m_index.find(tst.first);

    if(it == m_index.end()) continue;

    auto ist = it->second.find(tst.second);

    if(ist != it->second.end()) {
      return ist->second; // return immediately - quality = 1
    } else if(tst.second == "*" && !it->second.empty()) {
      if(m_defaultMapper && tst.first == m_defaultMapper->getInfo().mimeType) {
        return m_defaultMapper;
      }
      return it->second.begin()->second; // return immediately - quality = 1
    }

  }

  std::vector<MatchedMapper> matches;

  for(auto& tp : values.titleParams) {

    auto tst = typeAndSubtype(tp.first);
    if(!tst.first || !tst.second) continue;

    bool success;
    auto q = utils::Conversion::strToFloat64(tp.second.toString(), success);
    if(!success) continue;

    if(tst.first == "*" && tst.second == "*") {
      matches.push_back({m_defaultMapper, q});
      continue;
    }

    auto it = m_index.find(tst.first);

    if(it == m_index.end()) continue;

    auto ist = it->second.find(tst.second);

    if(ist != it->second.end()) {
      matches.push_back({ist->second, q});
      continue;
    } else if(tst.second == "*" && !it->second.empty()) {
      if(m_defaultMapper && tst.first == m_defaultMapper->getInfo().mimeType) {
        matches.push_back({m_defaultMapper, q});
      } else {
        matches.push_back({it->second.begin()->second, q});
      }
      continue;
    }

  }

  if(matches.empty()) {
    return nullptr;
  }

  std::sort(matches.begin(), matches.end());
  return matches.at(0).mapper;

}

std::shared_ptr<data::mapping::ObjectMapper> ContentMappers::selectMapperForContent(const booster::String& contentTypeHeader) const {

  std::shared_lock<std::shared_mutex> lock(m_mutex);

  if(!contentTypeHeader || contentTypeHeader->empty()) {
    return m_defaultMapper;
  }

  protocol::http::HeaderValueData values;
  protocol::http::Parser::parseHeaderValueData(values, contentTypeHeader, ';');

  if(values.tokens.empty()) {
    return nullptr;
  }

  return getMapper(values.tokens.begin()->toString());

}

std::shared_ptr<data::mapping::ObjectMapper> ContentMappers::selectMapper(const booster::String& acceptHeader) const {

  std::shared_lock<std::shared_mutex> lock(m_mutex);

  if(!acceptHeader || acceptHeader->empty()) {
    return m_defaultMapper;
  }

  protocol::http::HeaderValueData values;
  protocol::http::Parser::parseHeaderValueData(values, acceptHeader, ',');

  return selectMapper(values);

}

std::shared_ptr<data::mapping::ObjectMapper> ContentMappers::selectMapper(const std::vector<booster::String>& acceptableContentTypes) const {

  std::shared_lock<std::shared_mutex> lock(m_mutex);

  if(acceptableContentTypes.empty()) {
    return m_defaultMapper;
  }

  protocol::http::HeaderValueData values;
  for(auto& ct : acceptableContentTypes) {
    if(ct == nullptr || ct->empty()) continue;
    protocol::http::Parser::parseHeaderValueData(values, ct, ',');
  }

  return selectMapper(values);

}

void ContentMappers::clear() {
  std::unique_lock<std::shared_mutex> lock(m_mutex);
  m_defaultMapper = nullptr;
  m_mappers.clear();
}

}
