

#ifndef booster_web_protocol_http_outgoing_Body_hpp
#define booster_web_protocol_http_outgoing_Body_hpp

#include "booster/web/protocol/http/Http.hpp"

#include "booster/data/stream/Stream.hpp"
#include "booster/async/Coroutine.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace outgoing {


class Body : public data::stream::ReadCallback {
protected:
  typedef http::Headers Headers;
public:


  virtual ~Body() override = default;

  ///////////////////////////
  // From the ReadCallback //
  //
  //virtual v_io_size read(void *buffer, v_buff_size count, async::Action& action) = 0;


  virtual void declareHeaders(Headers& headers) = 0;


  virtual p_char8 getKnownData() = 0;


  virtual v_int64 getKnownSize() = 0;
  
};
  
}}}}}

#endif
