

#ifndef booster_web_protocol_http_encoding_EncoderProvider_hpp
#define booster_web_protocol_http_encoding_EncoderProvider_hpp

#include "booster/data/buffer/Processor.hpp"
#include "booster/Types.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace encoding {


class EncoderProvider {
public:


  virtual ~EncoderProvider() = default;


  virtual booster::String getEncodingName() = 0;


  virtual std::shared_ptr<data::buffer::Processor> getProcessor() = 0;

};

}}}}}

#endif // booster_web_protocol_http_encoding_EncoderProvider_hpp
