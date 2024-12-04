

#ifndef booster_web_protocol_http_outgoing_ResponseFactory_hpp
#define booster_web_protocol_http_outgoing_ResponseFactory_hpp

#include "./Response.hpp"

#include "booster/data/mapping/ObjectMapper.hpp"
#include "booster/data/type/Type.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace outgoing {


class ResponseFactory {
public:
  

  static std::shared_ptr<Response> createResponse(const Status &status);


  static std::shared_ptr<Response> createResponse(const Status& status, const booster::String& text);


  static std::shared_ptr<Response> createResponse(const Status& status,
                                                  const booster::Void& dto,
                                                  const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper);
  
};
  
}}}}}

#endif
