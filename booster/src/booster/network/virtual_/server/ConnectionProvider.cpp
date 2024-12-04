

#include "ConnectionProvider.hpp"

#include <chrono>

namespace booster { namespace network { namespace virtual_ { namespace server {

void ConnectionProvider::ConnectionInvalidator::invalidate(const std::shared_ptr<data::stream::IOStream>& connection) {
  auto socket = std::static_pointer_cast<Socket>(connection);
  socket->close();
}

ConnectionProvider::ConnectionProvider(const std::shared_ptr<virtual_::Interface>& _interface)
  : m_invalidator(std::make_shared<ConnectionInvalidator>())
  , m_interface(_interface)
  , m_listenerLock(_interface->bind())
  , m_open(true)
  , m_maxAvailableToRead(-1)
  , m_maxAvailableToWrite(-1)
{
  setProperty(PROPERTY_HOST, m_interface->getName());
  setProperty(PROPERTY_PORT, "0");
}

std::shared_ptr<ConnectionProvider> ConnectionProvider::createShared(const std::shared_ptr<virtual_::Interface>& _interface) {
  return std::make_shared<ConnectionProvider>(_interface);
}

void ConnectionProvider::setSocketMaxAvailableToReadWrtie(v_io_size maxToRead, v_io_size maxToWrite) {
  m_maxAvailableToRead = maxToRead;
  m_maxAvailableToWrite = maxToWrite;
}

void ConnectionProvider::stop() {
  m_open = false;
  m_listenerLock.reset();
  m_interface->notifyAcceptors();
}

provider::ResourceHandle<data::stream::IOStream> ConnectionProvider::get() {
  auto socket = m_interface->accept(m_open, std::chrono::milliseconds(500));
  if(socket) {
    socket->setMaxAvailableToReadWrtie(m_maxAvailableToRead, m_maxAvailableToWrite);
  }
  return provider::ResourceHandle<data::stream::IOStream>(socket, m_invalidator);
}

}}}}
