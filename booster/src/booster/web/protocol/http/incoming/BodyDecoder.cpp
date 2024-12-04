

#include "BodyDecoder.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BodyDecoder

booster::async::CoroutineStarterForResult<const booster::String&>
BodyDecoder::decodeToStringAsync(const Headers& headers,
                                 const std::shared_ptr<data::stream::InputStream>& bodyStream,
                                 const std::shared_ptr<data::stream::IOStream>& connection) const
{

  class ToStringDecoder : public booster::async::CoroutineWithResult<ToStringDecoder, const booster::String&> {
  private:
    const BodyDecoder* m_decoder;
    Headers m_headers;
    std::shared_ptr<data::stream::InputStream> m_bodyStream;
    std::shared_ptr<data::stream::IOStream> m_connection;
    std::shared_ptr<data::stream::BufferOutputStream> m_outputStream;
  public:

    ToStringDecoder(const BodyDecoder* decoder,
                    const Headers& headers,
                    const std::shared_ptr<data::stream::InputStream>& bodyStream,
                    const std::shared_ptr<data::stream::IOStream>& connection)
      : m_decoder(decoder)
      , m_headers(headers)
      , m_bodyStream(bodyStream)
      , m_connection(connection)
      , m_outputStream(std::make_shared<data::stream::BufferOutputStream>())
    {}

    Action act() override {
      return m_decoder->decodeAsync(m_headers, m_bodyStream, m_outputStream, m_connection)
        .next(yieldTo(&ToStringDecoder::onDecoded));
    }

    Action onDecoded() {
      return _return(m_outputStream->toString());
    }

  };

  return ToStringDecoder::startForResult(this, headers, bodyStream, connection);

}

}}}}}
