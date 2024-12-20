

#include "ResponseHeadersReader.hpp"

#include "booster/data/stream/BufferStream.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {

v_io_size ResponseHeadersReader::readHeadersSectionIterative(ReadHeadersIteration& iteration,
                                                                   const std::shared_ptr<booster::data::stream::IOStream>& connection,
                                                                   data::stream::ConsistentOutputStream* bufferStream,
                                                                   Result& result,
                                                                   async::Action& action)
{

  v_buff_size desiredToRead = m_buffer.getSize();
  if(iteration.progress + desiredToRead > m_maxHeadersSize) {
    desiredToRead = m_maxHeadersSize - iteration.progress;
    if(desiredToRead <= 0) {
      return -1;
    }
  }

  auto bufferData = reinterpret_cast<p_char8>(const_cast<void*>(m_buffer.getData()));
  auto res = connection->read(bufferData, desiredToRead, action);
  if(res > 0) {

    bufferStream->writeSimple(bufferData, res);

    for(v_buff_size i = 0; i < res; i ++) {
      iteration.accumulator <<= 8;
      iteration.accumulator |= bufferData[i];
      if(iteration.accumulator == SECTION_END) {
        result.bufferPosStart = i + 1;
        result.bufferPosEnd = res;
        iteration.done = true;
        return res;
      }
    }

  }

  return res;

}

ResponseHeadersReader::Result ResponseHeadersReader::readHeaders(const std::shared_ptr<booster::data::stream::IOStream>& connection,
                                                                 http::HttpError::Info& error) {
  
  Result result;
  ReadHeadersIteration iteration;
  async::Action action;
  
  booster::data::stream::BufferOutputStream buffer;

  while(!iteration.done) {

    error.ioStatus = readHeadersSectionIterative(iteration, connection, &buffer, result, action);

    if(!action.isNone()) {
      throw std::runtime_error("[booster::web::protocol::http::incoming::ResponseHeadersReader::readHeaders]: Error. Async action is unexpected.");
    }

    if(error.ioStatus > 0) {
      continue;
    } else if(error.ioStatus == IOError::RETRY_READ || error.ioStatus == IOError::RETRY_WRITE) {
      continue;
    } else {
      break;
    }

  }

  if(error.ioStatus > 0) {
    auto headersText = buffer.toString();
    booster::utils::parser::Caret caret (headersText);
    http::Status status;
    http::Parser::parseResponseStartingLine(result.startingLine, headersText.getPtr(), caret, status);
    if(status.code == 0) {
      http::Parser::parseHeaders(result.headers, headersText.getPtr(), caret, status);
    }
  }
  
  return result;
  
}

booster::async::CoroutineStarterForResult<const ResponseHeadersReader::Result&>
ResponseHeadersReader::readHeadersAsync(const std::shared_ptr<booster::data::stream::IOStream>& connection)
{
  
  class ReaderCoroutine : public booster::async::CoroutineWithResult<ReaderCoroutine, const Result&> {
  private:
    ResponseHeadersReader* m_this;
    std::shared_ptr<booster::data::stream::IOStream> m_connection;
    ReadHeadersIteration m_iteration;
    ResponseHeadersReader::Result m_result;
    booster::data::stream::BufferOutputStream m_bufferStream;
  public:
    
    ReaderCoroutine(ResponseHeadersReader* _this,
                    const std::shared_ptr<booster::data::stream::IOStream>& connection)
      : m_this(_this)
      , m_connection(connection)
    {}
    
    Action act() override {

      async::Action action;
      auto res = m_this->readHeadersSectionIterative(m_iteration, m_connection, &m_bufferStream, m_result, action);

      if(!action.isNone()) {
        return action;
      }

      if(m_iteration.done) {
        return yieldTo(&ReaderCoroutine::parseHeaders);
      } else {

        if (res > 0) {
          return repeat();
        } else if (res == IOError::RETRY_READ || res == IOError::RETRY_WRITE) {
          return repeat();
        }

      }

      return error<Error>("[booster::web::protocol::http::incoming::RequestHeadersReader::readHeadersAsync()]: Error. Error reading connection stream.");

    }
    
    Action parseHeaders() {
      
      auto headersText = m_bufferStream.toString();
      booster::utils::parser::Caret caret (headersText);
      http::Status status;
      http::Parser::parseResponseStartingLine(m_result.startingLine, headersText.getPtr(), caret, status);
      if(status.code == 0) {
        http::Parser::parseHeaders(m_result.headers, headersText.getPtr(), caret, status);
        if(status.code == 0) {
          return _return(m_result);
        } else {
          return error<Error>("[booster::web::protocol::http::incoming::ResponseHeadersReader::readHeadersAsync()]: Error. Error occurred while parsing headers.");
        }
      } else {
        return error<Error>("[booster::web::protocol::http::incoming::ResponseHeadersReader::readHeadersAsync()]: Error. Can't parse starting line.");
      }
      
    }
    
  };
  
  return ReaderCoroutine::startForResult(this, connection);
  
}
  
}}}}}
