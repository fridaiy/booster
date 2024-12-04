

#ifndef booster_web_server_HttpServerError_hpp
#define booster_web_server_HttpServerError_hpp

#include "booster/web/protocol/http/incoming/Request.hpp"

namespace booster { namespace web { namespace server {


class HttpServerError : public std::runtime_error {
private:
  std::shared_ptr<protocol::http::incoming::Request> m_request;
public:


  HttpServerError(const std::shared_ptr<protocol::http::incoming::Request>& request, const std::string& message);


  const std::shared_ptr<protocol::http::incoming::Request>& getRequest() const;

};

}}}

#endif //booster_web_server_HttpServerError_hpp
