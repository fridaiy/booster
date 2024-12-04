

#ifndef booster_web_protocol_CommunicationError_hpp
#define booster_web_protocol_CommunicationError_hpp

#include "booster/IODefinitions.hpp"

namespace booster { namespace web { namespace protocol {


class CommunicationError : public std::runtime_error {
private:
  booster::v_io_size m_ioStatus;
  booster::String m_message;
public:


  CommunicationError(booster::v_io_size ioStatus, const booster::String& message);


  booster::v_io_size getIOStatus();


  booster::String& getMessage();
  
};


template<class Status>
struct ProtocolErrorInfo {


  ProtocolErrorInfo()
    : ioStatus(0)
  {}


  ProtocolErrorInfo(booster::v_io_size pIOStatus, const Status& pStatus)
    : ioStatus(pIOStatus)
    , status(pStatus)
  {}


  booster::v_io_size ioStatus;


  Status status;

};


template<class Status>
class ProtocolError : public CommunicationError {
public:

  typedef ProtocolErrorInfo<Status> Info;
private:
  Info m_info;
public:


  ProtocolError(const Info& info, const booster::String& message)
    : CommunicationError(info.ioStatus, message)
    , m_info(info)
  {}


  Info getInfo() const {
    return m_info;
  }
  
};



template<class Status>
class AsyncProtocolError : public booster::AsyncIOError {
public:

  typedef ProtocolErrorInfo<Status> Info;
private:
  Info m_info;
  booster::String m_message;
public:


  AsyncProtocolError(const Info& info, const booster::String& message)
    : booster::AsyncIOError("AsyncProtocolError", info.ioStatus)
    , m_info(info)
    , m_message(message)
  {}


  booster::String getMessage() {
    return m_message;
  }


  Info getInfo() {
    return m_info;
  }

};
  
}}}

#endif
