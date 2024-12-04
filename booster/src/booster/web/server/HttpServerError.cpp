

#include "HttpServerError.hpp"

namespace booster { namespace web { namespace server {

HttpServerError::HttpServerError(const std::shared_ptr<protocol::http::incoming::Request>& request, const std::string& message)
  : std::runtime_error(message)
  , m_request(request)
{}

const std::shared_ptr<protocol::http::incoming::Request>& HttpServerError::getRequest() const {
  return m_request;
}

}}}
