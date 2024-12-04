

#ifndef booster_web_server_interceptor_ResponseInterceptor_hpp
#define booster_web_server_interceptor_ResponseInterceptor_hpp

#include "booster/web/protocol/http/incoming/Request.hpp"
#include "booster/web/protocol/http/outgoing/Response.hpp"
#include "booster/web/protocol/http/Http.hpp"

namespace booster { namespace web { namespace server { namespace interceptor {


class ResponseInterceptor {
public:

  typedef booster::web::protocol::http::incoming::Request IncomingRequest;


  typedef booster::web::protocol::http::outgoing::Response OutgoingResponse;
public:


  virtual ~ResponseInterceptor() = default;


  virtual std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request,
                                                      const std::shared_ptr<OutgoingResponse>& response) = 0;

};

}}}}

#endif
