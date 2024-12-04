

#include "Socket.hpp"

namespace booster { namespace network { namespace virtual_ {

Socket::Socket(const std::shared_ptr<Pipe>& pipeIn, const std::shared_ptr<Pipe>& pipeOut)
  : m_pipeIn(pipeIn)
  , m_pipeOut(pipeOut)
{}

Socket::~Socket() {
  close();
}

std::shared_ptr<Socket> Socket::createShared(const std::shared_ptr<Pipe>& pipeIn, const std::shared_ptr<Pipe>& pipeOut) {
  return std::make_shared<Socket>(pipeIn, pipeOut);
}

void Socket::setMaxAvailableToReadWrtie(v_io_size maxToRead, v_io_size maxToWrite) {
  m_pipeIn->getReader()->setMaxAvailableToRead(maxToRead);
  m_pipeOut->getWriter()->setMaxAvailableToWrite(maxToWrite);
}
  
v_io_size Socket::read(void *data, v_buff_size count, async::Action& action) {
  return m_pipeIn->getReader()->read(data, count, action);
}

v_io_size Socket::write(const void *data, v_buff_size count, async::Action& action) {
  return m_pipeOut->getWriter()->write(data, count, action);
}

void Socket::setOutputStreamIOMode(booster::data::stream::IOMode ioMode) {
  m_pipeOut->getWriter()->setOutputStreamIOMode(ioMode);
}

booster::data::stream::IOMode Socket::getOutputStreamIOMode() {
  return m_pipeOut->getWriter()->getOutputStreamIOMode();
}

void Socket::setInputStreamIOMode(booster::data::stream::IOMode ioMode) {
  m_pipeIn->getReader()->setInputStreamIOMode(ioMode);
}

booster::data::stream::IOMode Socket::getInputStreamIOMode() {
  return m_pipeIn->getReader()->getInputStreamIOMode();
}

booster::data::stream::Context& Socket::getOutputStreamContext() {
  return m_pipeOut->getWriter()->getOutputStreamContext();
}


booster::data::stream::Context& Socket::getInputStreamContext() {
  return m_pipeIn->getReader()->getInputStreamContext();
}

void Socket::close() {
  m_pipeIn->close();
  m_pipeOut->close();
}
  
}}}
