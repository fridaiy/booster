

#ifndef booster_web_protocol_http_incoming_ResponseHeadersReader_hpp
#define booster_web_protocol_http_incoming_ResponseHeadersReader_hpp

#include "booster/web/protocol/http/Http.hpp"
#include "booster/async/Coroutine.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {


class ResponseHeadersReader {
public:

  typedef booster::async::Action Action;
private:
  static constexpr v_uint32 SECTION_END = ('\r' << 24) | ('\n' << 16) | ('\r' << 8) | ('\n');
public:


  struct Result {

    http::ResponseStartingLine startingLine;


    http::Headers headers;


    v_buff_size bufferPosStart;


    v_buff_size bufferPosEnd;
  };
private:

  struct ReadHeadersIteration {
    v_uint32 accumulator = 0;
    v_buff_size progress = 0;
    bool done = false;
  };

private:

  v_io_size readHeadersSectionIterative(ReadHeadersIteration& iteration,
                                              const std::shared_ptr<booster::data::stream::IOStream>& connection,
                                              data::stream::ConsistentOutputStream* bufferStream,
                                              Result& result,
                                              async::Action& action);

private:
  booster::data::share::MemoryLabel m_buffer;
  v_buff_size m_maxHeadersSize;
public:


  ResponseHeadersReader(const booster::data::share::MemoryLabel& buffer, v_buff_size maxHeadersSize)
    : m_buffer(buffer)
    , m_maxHeadersSize(maxHeadersSize)
  {}


  Result readHeaders(const std::shared_ptr<booster::data::stream::IOStream>& connection, http::HttpError::Info& error);


  booster::async::CoroutineStarterForResult<const Result&> readHeadersAsync(const std::shared_ptr<booster::data::stream::IOStream>& connection);
  
};
  
}}}}}

#endif
