

#ifndef booster_web_protocol_http_incoming_RequestHeadersReader_hpp
#define booster_web_protocol_http_incoming_RequestHeadersReader_hpp

#include "booster/web/protocol/http/Http.hpp"
#include "booster/async/Coroutine.hpp"
#include "booster/data/stream/StreamBufferedProxy.hpp"
#include "booster/data/stream/BufferStream.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {


class RequestHeadersReader {
public:

  typedef booster::async::Action Action;
private:
  static constexpr v_uint32 SECTION_END = ('\r' << 24) | ('\n' << 16) | ('\r' << 8) | ('\n');
public:


  struct Result {

    http::RequestStartingLine startingLine;


    http::Headers headers;

  };

private:

  struct ReadHeadersIteration {
    v_uint32 accumulator = 0;
    bool done = false;
  };

private:
  v_io_size readHeadersSectionIterative(ReadHeadersIteration& iteration,
                                              data::stream::InputStreamBufferedProxy* stream,
                                              async::Action& action);
private:
  booster::data::stream::BufferOutputStream* m_bufferStream;
  v_buff_size m_readChunkSize;
  v_buff_size m_maxHeadersSize;
public:


  RequestHeadersReader(booster::data::stream::BufferOutputStream* bufferStream,
                       v_buff_size readChunkSize = 2048,
                       v_buff_size maxHeadersSize = 4096)
    : m_bufferStream(bufferStream)
    , m_readChunkSize(readChunkSize)
    , m_maxHeadersSize(maxHeadersSize)
  {}


  Result readHeaders(data::stream::InputStreamBufferedProxy* stream, http::HttpError::Info& error);


  booster::async::CoroutineStarterForResult<const RequestHeadersReader::Result&> readHeadersAsync(const std::shared_ptr<data::stream::InputStreamBufferedProxy>& stream);
  
};
  
}}}}}

#endif
