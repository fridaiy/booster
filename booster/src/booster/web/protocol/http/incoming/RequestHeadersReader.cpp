

#include "RequestHeadersReader.hpp"
#include "booster/base/Log.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace incoming {

v_io_size RequestHeadersReader::readHeadersSectionIterative(ReadHeadersIteration& iteration,
                                                                  data::stream::InputStreamBufferedProxy* stream,
                                                                  async::Action& action)
{

  v_buff_size desiredToRead = m_readChunkSize;
  if(m_bufferStream->getCurrentPosition() + desiredToRead > m_maxHeadersSize) {
    desiredToRead = m_maxHeadersSize - m_bufferStream->getCurrentPosition();
    if(desiredToRead <= 0) {
      return -1;
    }
  }

  m_bufferStream->reserveBytesUpfront(desiredToRead);
  auto bufferData = m_bufferStream->getData() + m_bufferStream->getCurrentPosition();
  auto res = stream->peek(bufferData, desiredToRead, action);
  if(res > 0) {

    m_bufferStream->setCurrentPosition(m_bufferStream->getCurrentPosition() + res);

    for(v_buff_size i = 0; i < res; i ++) {
      iteration.accumulator <<= 8;
      iteration.accumulator |= bufferData[i];
      if(iteration.accumulator == SECTION_END) {
        stream->commitReadOffset(i + 1);
        iteration.done = true;
        return res;
      }
    }

    stream->commitReadOffset(res);

  }
  
  return res;
  
}
  
RequestHeadersReader::Result RequestHeadersReader::readHeaders(data::stream::InputStreamBufferedProxy* stream,
                                                               http::HttpError::Info& error) {

  m_bufferStream->setCurrentPosition(0);

  RequestHeadersReader::Result result;
  ReadHeadersIteration iteration;
  async::Action action;

  while(!iteration.done) {

    error.ioStatus = readHeadersSectionIterative(iteration, stream, action);

    if(!action.isNone()) {
      BOOSTER_LOGe("[booster::web::protocol::http::incoming::RequestHeadersReader::readHeaders]", "Error. Async action is unexpected.")
      throw std::runtime_error("[booster::web::protocol::http::incoming::RequestHeadersReader::readHeaders]: Error. Async action is unexpected.");
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
    booster::utils::parser::Caret caret (reinterpret_cast<const char*>(m_bufferStream->getData()), m_bufferStream->getCurrentPosition());
    http::Status status;
    http::Parser::parseRequestStartingLine(result.startingLine, nullptr, caret, status);
    if(status.code == 0) {
      http::Parser::parseHeaders(result.headers, nullptr, caret, status);
    }
  }
  
  return result;
  
}
  
  
booster::async::CoroutineStarterForResult<const RequestHeadersReader::Result&>
RequestHeadersReader::readHeadersAsync(const std::shared_ptr<data::stream::InputStreamBufferedProxy>& stream)
{
  
  class ReaderCoroutine : public booster::async::CoroutineWithResult<ReaderCoroutine, const Result&> {
  private:
    std::shared_ptr<data::stream::InputStreamBufferedProxy> m_stream;
    RequestHeadersReader* m_this;
    ReadHeadersIteration m_iteration;
    RequestHeadersReader::Result m_result;
  public:
    
    ReaderCoroutine(RequestHeadersReader* _this,
                    const std::shared_ptr<data::stream::InputStreamBufferedProxy>& stream)
      : m_stream(stream)
      , m_this(_this)
    {
      m_this->m_bufferStream->setCurrentPosition(0);
    }
    
    Action act() override {

      async::Action action;
      auto res = m_this->readHeadersSectionIterative(m_iteration, m_stream.get(), action);

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

      booster::utils::parser::Caret caret (reinterpret_cast<const char*>(m_this->m_bufferStream->getData()), m_this->m_bufferStream->getCurrentPosition());
      http::Status status;
      http::Parser::parseRequestStartingLine(m_result.startingLine, nullptr, caret, status);
      if(status.code == 0) {
        http::Parser::parseHeaders(m_result.headers, nullptr, caret, status);
        if(status.code == 0) {
          return _return(m_result);
        } else {
          return error<Error>("[booster::web::protocol::http::incoming::RequestHeadersReader::readHeadersAsync()]: Error. Error occurred while parsing headers.");
        }
      } else {
        return error<Error>("[booster::web::protocol::http::incoming::RequestHeadersReader::readHeadersAsync()]: Error. Can't parse starting line.");
      }

    }
    
  };
  
  return ReaderCoroutine::startForResult(this, stream);
  
}

}}}}}
