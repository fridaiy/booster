

#ifndef booster_web_server_interceptor_AllowCorsGlobal_hpp
#define booster_web_server_interceptor_AllowCorsGlobal_hpp

#include "booster/web/server/interceptor/ResponseInterceptor.hpp"
#include "booster/web/server/interceptor/RequestInterceptor.hpp"

namespace booster { namespace web { namespace server { namespace interceptor {

class AllowOptionsGlobal : public RequestInterceptor {
public:
  std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request) override;
};

class AllowCorsGlobal : public ResponseInterceptor {
private:
  booster::String m_origin;
  booster::String m_methods;
  booster::String m_headers;
  booster::String m_maxAge;
public:

  AllowCorsGlobal(const booster::String &origin = "*",
                  const booster::String &methods = "GET, POST, OPTIONS",
                  const booster::String &headers = "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization",
                  const booster::String &maxAge = "1728000");

  std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request,
                                              const std::shared_ptr<OutgoingResponse>& response) override;

};

}}}}

#endif // booster_web_server_interceptor_AllowCorsGlobal_hpp
