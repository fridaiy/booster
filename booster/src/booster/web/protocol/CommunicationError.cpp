

#include "CommunicationError.hpp"

namespace booster { namespace web { namespace protocol {
  
CommunicationError::CommunicationError(booster::v_io_size ioStatus, const booster::String& message)
  :std::runtime_error(*message)
  , m_ioStatus(ioStatus)
  , m_message(message)
{}
  
booster::v_io_size CommunicationError::getIOStatus() {
  return m_ioStatus;
}

booster::String& CommunicationError::getMessage(){
  return m_message;
}
  
}}}
