

#ifndef booster_web_server_handler_AuthorizationHandler_hpp
#define booster_web_server_handler_AuthorizationHandler_hpp

#include "booster/web/protocol/http/incoming/Request.hpp"
#include "booster/web/protocol/http/Http.hpp"
#include "booster/macro/codegen.hpp"
#include "booster/data/type/Type.hpp"

namespace booster { namespace web { namespace server { namespace handler {


class AuthorizationObject : public booster::base::Countable {
protected:
  AuthorizationObject() = default;
};


class AuthorizationHandler {
public:

  typedef booster::web::server::handler::AuthorizationObject AuthorizationObject;


  typedef booster::data::stream::BufferOutputStream BufferOutputStream;


  typedef booster::web::protocol::http::Headers Headers;
private:
  booster::String m_scheme;
  booster::String m_realm;
public:


  AuthorizationHandler(const booster::String& scheme, const booster::String& realm);


  virtual ~AuthorizationHandler() = default;


  virtual std::shared_ptr<AuthorizationObject> handleAuthorization(const booster::String& authorizationHeader) = 0;


  virtual void renderAuthenticateHeaderValue(BufferOutputStream& stream);


  virtual void addErrorResponseHeaders(Headers& headers);


  booster::String getScheme();


  booster::String getRealm();

};


class DefaultBasicAuthorizationObject : public AuthorizationObject {
public:


  booster::String userId;


  booster::String password;

};


class BasicAuthorizationHandler : public AuthorizationHandler {
public:


  BasicAuthorizationHandler(const booster::String& realm = "API");


  std::shared_ptr<AuthorizationObject> handleAuthorization(const booster::String &header) override;


  virtual std::shared_ptr<AuthorizationObject> authorize(const booster::String& userId, const booster::String& password);

};


class DefaultBearerAuthorizationObject : public AuthorizationObject {
public:


  booster::String token;

};


class BearerAuthorizationHandler : public AuthorizationHandler {
public:


  BearerAuthorizationHandler(const booster::String& realm = "API");


  std::shared_ptr<AuthorizationObject> handleAuthorization(const booster::String &header) override;


  virtual std::shared_ptr<AuthorizationObject> authorize(const booster::String& token);

};

}}}}

#endif
