

#ifndef booster_web_server_HttpRequestHandler_hpp
#define booster_web_server_HttpRequestHandler_hpp

#include "booster/web/protocol/http/outgoing/ResponseFactory.hpp"
#include "booster/web/protocol/http/outgoing/Response.hpp"
#include "booster/web/protocol/http/incoming/Request.hpp"

namespace booster { namespace web { namespace server {


class HttpRequestHandler {
public:


  typedef booster::web::protocol::http::Status Status;


  typedef booster::web::protocol::http::Header Header;


  typedef booster::web::protocol::http::Headers Headers;


  typedef booster::web::protocol::http::QueryParams QueryParams;


  typedef booster::web::protocol::http::incoming::Request IncomingRequest;


  typedef booster::web::protocol::http::outgoing::Response OutgoingResponse;


  typedef booster::web::protocol::http::outgoing::ResponseFactory ResponseFactory;


  typedef booster::web::protocol::http::HttpError HttpError;

public:


  virtual std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) {
    (void)request;
    throw HttpError(Status::CODE_501, "Endpoint not implemented.", {});
  }


  virtual booster::async::CoroutineStarterForResult<const std::shared_ptr<OutgoingResponse>&>
  handleAsync(const std::shared_ptr<IncomingRequest>& request) {
    (void)request;
    throw HttpError(Status::CODE_501, "Asynchronous endpoint not implemented.", {});
  }


  virtual ~HttpRequestHandler() = default;
};

}}}

#endif // booster_web_server_HttpRequestHandler_hpp
