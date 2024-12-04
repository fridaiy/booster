

#ifndef booster_web_server_handler_ErrorHandler_hpp
#define booster_web_server_handler_ErrorHandler_hpp

#include "booster/web/protocol/http/incoming/Request.hpp"
#include "booster/web/protocol/http/outgoing/Response.hpp"
#include "booster/web/protocol/http/Http.hpp"

namespace booster { namespace web { namespace server { namespace handler {


class ErrorHandler {
public:

  typedef web::protocol::http::Headers Headers;
public:

  virtual ~ErrorHandler() = default;


  virtual std::shared_ptr<protocol::http::outgoing::Response> handleError(const std::exception_ptr& exceptionPtr) = 0;

};


class DefaultErrorHandler : public booster::base::Countable, public ErrorHandler {
public:

  struct HttpServerErrorStacktrace {
    std::shared_ptr<protocol::http::incoming::Request> request;
    protocol::http::Status status;
    Headers headers;
    std::list<booster::String> stack;
  };

private:
  void unwrapErrorStack(HttpServerErrorStacktrace& stacktrace, const std::exception& e);
public:


  DefaultErrorHandler() = default;

  std::shared_ptr<protocol::http::outgoing::Response> handleError(const std::exception_ptr& error) override;


  virtual std::shared_ptr<protocol::http::outgoing::Response> renderError(const HttpServerErrorStacktrace& stacktrace);

};
  
}}}}

#endif
