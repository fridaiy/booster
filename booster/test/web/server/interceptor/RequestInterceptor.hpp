

#ifndef booster_web_server_interceptor_RequestInterceptor_hpp
#define booster_web_server_interceptor_RequestInterceptor_hpp

#include "booster/web/protocol/http/outgoing/Response.hpp"
#include "booster/web/protocol/http/incoming/Request.hpp"
#include "booster/web/protocol/http/Http.hpp"

namespace booster { namespace web { namespace server { namespace interceptor {


class RequestInterceptor {
public:

  typedef booster::web::protocol::http::incoming::Request IncomingRequest;


  typedef booster::web::protocol::http::outgoing::Response OutgoingResponse;
public:
  

  virtual ~RequestInterceptor() = default;
  

  virtual std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request) = 0;
  
};
  
}}}}

#endif
