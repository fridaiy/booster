

#include "AuthorizationHandler.hpp"

#include "booster/encoding/Base64.hpp"
#include "booster/utils/parser/Caret.hpp"

namespace booster { namespace web { namespace server { namespace handler {

AuthorizationHandler::AuthorizationHandler(const booster::String& scheme, const booster::String& realm)
  : m_scheme(scheme)
  , m_realm(realm)
{}

void AuthorizationHandler::renderAuthenticateHeaderValue(BufferOutputStream& stream) {
  stream << m_scheme << " " << "realm=\"" << m_realm << "\"";
}

void AuthorizationHandler::addErrorResponseHeaders(Headers& headers) {
  BufferOutputStream stream;
  renderAuthenticateHeaderValue(stream);
  headers.put_LockFree(protocol::http::Header::WWW_AUTHENTICATE, stream.toString());
}

booster::String AuthorizationHandler::getScheme() {
  return m_scheme;
}

booster::String AuthorizationHandler::getRealm() {
  return m_realm;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BasicAuthorizationHandler

BasicAuthorizationHandler::BasicAuthorizationHandler(const booster::String& realm)
  : AuthorizationHandler("Basic", realm)
{}

std::shared_ptr<handler::AuthorizationObject> BasicAuthorizationHandler::handleAuthorization(const booster::String &header) {

  if(header && header->size() > 6 && utils::String::compare(header->data(), 6, "Basic ", 6) == 0) {

    booster::String auth = booster::encoding::Base64::decode(header->c_str() + 6, static_cast<v_buff_size>(header->size() - 6));
    utils::parser::Caret caret(auth);

    if (caret.findChar(':')) {
      booster::String userId(&caret.getData()[0], caret.getPosition());
      booster::String password(&caret.getData()[caret.getPosition() + 1],
                             caret.getDataSize() - caret.getPosition() - 1);
      auto authResult = authorize(userId, password);
      if(authResult) {
        return authResult;
      }

      Headers responseHeaders;
      addErrorResponseHeaders(responseHeaders);
      throw protocol::http::HttpError(protocol::http::Status::CODE_401, "Unauthorized", responseHeaders);

    }

  }

  Headers responseHeaders;
  addErrorResponseHeaders(responseHeaders);
  throw protocol::http::HttpError(protocol::http::Status::CODE_401, "Authorization Required", responseHeaders);

}

std::shared_ptr<AuthorizationObject> BasicAuthorizationHandler::authorize(const booster::String &userId,
                                                                          const booster::String &password)
{
  auto authorizationObject = std::make_shared<DefaultBasicAuthorizationObject>();
  authorizationObject->userId = userId;
  authorizationObject->password = password;
  return authorizationObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BearerAuthorizationHandler

BearerAuthorizationHandler::BearerAuthorizationHandler(const booster::String& realm)
  : AuthorizationHandler("Bearer", realm)
{}

std::shared_ptr<AuthorizationObject> BearerAuthorizationHandler::handleAuthorization(const booster::String &header) {

  if(header && header->size() > 7 && utils::String::compare(header->data(), 7, "Bearer ", 7) == 0) {

    booster::String token = booster::String(header->c_str() + 7, static_cast<v_buff_size>(header->size() - 7));

    auto authResult = authorize(token);
    if(authResult) {
      return authResult;
    }

    Headers responseHeaders;
    addErrorResponseHeaders(responseHeaders);
    throw protocol::http::HttpError(protocol::http::Status::CODE_401, "Unauthorized", responseHeaders);

  }

  Headers responseHeaders;
  addErrorResponseHeaders(responseHeaders);
  throw protocol::http::HttpError(protocol::http::Status::CODE_401, "Authorization Required", responseHeaders);

}

std::shared_ptr<AuthorizationObject> BearerAuthorizationHandler::authorize(const booster::String& token) {
  auto authObject = std::make_shared<DefaultBearerAuthorizationObject>();
  authObject->token = token;
  return authObject;
}

}}}}
