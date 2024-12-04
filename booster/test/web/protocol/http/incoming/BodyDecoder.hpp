

#ifndef booster_web_protocol_http_incoming_BodyDecoder_hpp
#define booster_web_protocol_http_incoming_BodyDecoder_hpp

#include "booster/web/protocol/http/Http.hpp"
#include "booster/data/mapping/ObjectMapper.hpp"
#include "booster/data/stream/BufferStream.hpp"
#include "booster/async/Coroutine.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {


class BodyDecoder {
private:
  
  template<class Wrapper>
  class ToDtoDecoder : public booster::async::CoroutineWithResult<ToDtoDecoder<Wrapper>, const Wrapper&> {
  private:
    const BodyDecoder* m_decoder;
    Headers m_headers;
    std::shared_ptr<data::stream::InputStream> m_bodyStream;
    std::shared_ptr<data::stream::IOStream> m_connection;
    std::shared_ptr<data::mapping::ObjectMapper> m_objectMapper;
    std::shared_ptr<data::stream::BufferOutputStream> m_outputStream;
  public:
    
    ToDtoDecoder(const BodyDecoder* decoder,
                 Headers& headers,
                 const std::shared_ptr<data::stream::InputStream>& bodyStream,
                 const std::shared_ptr<data::stream::IOStream>& connection,
                 const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper)
      : m_decoder(decoder)
      , m_headers(headers)
      , m_bodyStream(bodyStream)
      , m_connection(connection)
      , m_objectMapper(objectMapper)
      , m_outputStream(std::make_shared<data::stream::BufferOutputStream>())
    {}
    
    booster::async::Action act() override {
      return m_decoder->decodeAsync(m_headers, m_bodyStream, m_outputStream, m_connection)
        .next(this->yieldTo(&ToDtoDecoder::onDecoded));
    }
    
    booster::async::Action onDecoded() {
      auto body = m_outputStream->toString();
      booster::utils::parser::Caret caret(body);
      auto dto = m_objectMapper->readFromCaret<Wrapper>(caret);
      if(caret.hasError()) {
        return this->template error<booster::async::Error>(caret.getErrorMessage());
      }
      return this->_return(dto);
    }
    
  };
  
public:


  virtual ~BodyDecoder() = default;


  virtual void decode(const Headers& headers,
                      data::stream::InputStream* bodyStream,
                      data::stream::WriteCallback* writeCallback,
                      data::stream::IOStream* connection) const = 0;


  virtual booster::async::CoroutineStarter decodeAsync(const Headers& headers,
                                                     const std::shared_ptr<data::stream::InputStream>& bodyStream,
                                                     const std::shared_ptr<data::stream::WriteCallback>& writeCallback,
                                                     const std::shared_ptr<data::stream::IOStream>& connection) const = 0;


  booster::String decodeToString(const Headers& headers,
                               data::stream::InputStream* bodyStream,
                               data::stream::IOStream* connection) const
  {
    booster::data::stream::BufferOutputStream stream;
    decode(headers, bodyStream, &stream, connection);
    return stream.toString();
  }


  template<class Wrapper>
  Wrapper decodeToDto(const Headers& headers,
                      data::stream::InputStream* bodyStream,
                      data::stream::IOStream* connection,
                      data::mapping::ObjectMapper* objectMapper) const
  {
    return objectMapper->readFromString<Wrapper>(decodeToString(headers, bodyStream, connection));
  }


  booster::async::CoroutineStarterForResult<const booster::String&>
  decodeToStringAsync(const Headers& headers,
                      const std::shared_ptr<data::stream::InputStream>& bodyStream,
                      const std::shared_ptr<data::stream::IOStream>& connection) const;


  template<class Wrapper>
  booster::async::CoroutineStarterForResult<const Wrapper&>
  decodeToDtoAsync(const Headers& headers,
                   const std::shared_ptr<data::stream::InputStream>& bodyStream,
                   const std::shared_ptr<data::stream::IOStream>& connection,
                   const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper) const
  {
    return ToDtoDecoder<Wrapper>::startForResult(this, headers, bodyStream, connection, objectMapper);
  }
  
};
  
}}}}}

#endif
