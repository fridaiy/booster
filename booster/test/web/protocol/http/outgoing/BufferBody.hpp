

#ifndef booster_web_protocol_http_outgoing_BufferBody_hpp
#define booster_web_protocol_http_outgoing_BufferBody_hpp

#include "./Body.hpp"
#include "booster/web/protocol/http/Http.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace outgoing {


class BufferBody : public booster::base::Countable, public Body {
private:
  booster::String m_buffer;
  booster::data::share::StringKeyLabel m_contentType;
  data::buffer::InlineReadData m_inlineData;
public:
  BufferBody(const booster::String& buffer, const data::share::StringKeyLabel& contentType);
public:


  static std::shared_ptr<BufferBody> createShared(const booster::String& buffer,
                                                  const data::share::StringKeyLabel& contentType = data::share::StringKeyLabel());


  v_io_size read(void *buffer, v_buff_size count, async::Action& action) override;


  void declareHeaders(Headers& headers) override;


  p_char8 getKnownData() override;


  v_int64 getKnownSize() override;
  
};
  
}}}}}

#endif
