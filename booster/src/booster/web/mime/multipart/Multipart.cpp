

#include "Multipart.hpp"

#include "booster/web/protocol/http/Http.hpp"
#include "booster/encoding/Base64.hpp"
#include "booster/utils/Random.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {

Multipart::Multipart(const booster::String& boundary)
  : m_boundary(boundary)
{}

booster::String Multipart::getBoundary() {
  return m_boundary;
}

std::shared_ptr<Part> Multipart::readNextPartSimple() {
  async::Action action;
  auto result = readNextPart(action);
  if(!action.isNone()) {
    throw std::runtime_error("[booster::web::mime::multipart::Multipart::readNextPartSimple()]. Error."
                             "Async method is called for non-async API.");
  }
  return result;
}

void Multipart::writeNextPartSimple(const std::shared_ptr<Part>& part) {
  async::Action action;
  writeNextPart(part, action);
  if(!action.isNone()) {
    throw std::runtime_error("[booster::web::mime::multipart::Multipart::writeNextPartSimple()]. Error."
                             "Async method is called for non-async API.");
  }
}

booster::String Multipart::generateRandomBoundary(v_int32 boundarySize) {
  std::unique_ptr<v_char8[]> buffer(new v_char8[static_cast<unsigned long>(boundarySize)]);
  utils::Random::randomBytes(buffer.get(), boundarySize);
  return encoding::Base64::encode(buffer.get(), boundarySize, encoding::Base64::ALPHABET_BASE64_URL_SAFE);
}

booster::String Multipart::parseBoundaryFromHeaders(const Headers& requestHeaders) {

  booster::String boundary;
  auto contentType = requestHeaders.getAsMemoryLabel<booster::data::share::StringKeyLabel>("Content-Type");

  if(contentType) {
    booster::web::protocol::http::HeaderValueData valueData;
    booster::web::protocol::http::Parser::parseHeaderValueData(valueData, contentType, ';');
    boundary = valueData.getTitleParamValue("boundary");
  }

  return boundary;

}

}}}}
