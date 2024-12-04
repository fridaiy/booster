

#include "ConnectionPool.hpp"

namespace booster { namespace network {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ConnectionAcquisitionProxy

v_io_size ConnectionAcquisitionProxy::write(const void *buff, v_buff_size count, async::Action& action) {
  return _handle.object->write(buff, count, action);
}

v_io_size ConnectionAcquisitionProxy::read(void *buff, v_buff_size count, async::Action& action) {
  return _handle.object->read(buff, count, action);
}

void ConnectionAcquisitionProxy::setOutputStreamIOMode(booster::data::stream::IOMode ioMode) {
  return _handle.object->setOutputStreamIOMode(ioMode);
}

booster::data::stream::IOMode ConnectionAcquisitionProxy::getOutputStreamIOMode() {
  return _handle.object->getOutputStreamIOMode();
}

void ConnectionAcquisitionProxy::setInputStreamIOMode(booster::data::stream::IOMode ioMode) {
  return _handle.object->setInputStreamIOMode(ioMode);
}

booster::data::stream::IOMode ConnectionAcquisitionProxy::getInputStreamIOMode() {
  return _handle.object->getInputStreamIOMode();
}

booster::data::stream::Context& ConnectionAcquisitionProxy::getOutputStreamContext() {
  return _handle.object->getOutputStreamContext();
}

booster::data::stream::Context& ConnectionAcquisitionProxy::getInputStreamContext() {
  return _handle.object->getInputStreamContext();
}

}}
