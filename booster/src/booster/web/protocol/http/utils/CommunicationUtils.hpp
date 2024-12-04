

#ifndef booster_web_protocol_http_utils_CommunicationUtils_hpp
#define booster_web_protocol_http_utils_CommunicationUtils_hpp

#include "booster/web/protocol/http/incoming/Request.hpp"
#include "booster/web/protocol/http/outgoing/Response.hpp"
#include "booster/web/protocol/http/encoding/ProviderCollection.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace utils {


class CommunicationUtils {
public:

  enum class ConnectionState : int {
    ALIVE = 0, // Continue processing connection.
    DELEGATED = 1, // Stop current connection processing as connection was delegated to other processor.
    CLOSING = 2, // Move connection to "closing" pool.
    DEAD = 3 // Drop immediately
  };

public:


  static void considerConnectionState(const std::shared_ptr<protocol::http::incoming::Request>& request,
                                      const std::shared_ptr<protocol::http::outgoing::Response>& response,
                                      ConnectionState& connectionState);

  static std::shared_ptr<encoding::EncoderProvider> selectEncoder(const std::shared_ptr<http::incoming::Request>& request,
                                                                  const std::shared_ptr<http::encoding::ProviderCollection>& providers);
  
};
  
}}}}}

#endif
