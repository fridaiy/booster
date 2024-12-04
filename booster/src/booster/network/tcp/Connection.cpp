

#include "./Connection.hpp"

#if defined(WIN32) || defined(_WIN32)
  #include <io.h>
  #include <winsock2.h>
#else
  #include <unistd.h>
  #include <sys/socket.h>
#endif

#include <thread>
#include <chrono>
#include <fcntl.h>

namespace booster { namespace network { namespace tcp {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Connection

booster::data::stream::DefaultInitializedContext Connection::DEFAULT_CONTEXT(data::stream::StreamType::STREAM_INFINITE);

Connection::Connection(v_io_handle handle)
  : m_handle(handle)
{

#if defined(WIN32) || defined(_WIN32)

  // in Windows, there is no reliable method to get if a socket is blocking or not.
  // Eevery socket is created blocking in Windows so we assume this state and pray.

  setStreamIOMode(data::stream::BLOCKING);

#else

  auto flags = fcntl(m_handle, F_GETFL);

  if (flags < 0) {
    throw std::runtime_error("[booster::network::tcp::Connection::Connection()]: Error. Can't get socket flags.");
  }

  if((flags & O_NONBLOCK) > 0) {
    m_mode = data::stream::IOMode::ASYNCHRONOUS;
  } else {
    m_mode = data::stream::IOMode::BLOCKING;
  }

#endif

}

Connection::~Connection(){
  close();
}

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlogical-op"
#endif

v_io_size Connection::write(const void *buff, v_buff_size count, async::Action& action){

#if defined(WIN32) || defined(_WIN32)

  auto result = ::send(m_handle, (const char*) buff, (int)count, 0);

  if(result == SOCKET_ERROR) {

    auto e = WSAGetLastError();

    if(e == WSAEWOULDBLOCK){
      if(m_mode == data::stream::ASYNCHRONOUS) {
        action = booster::async::Action::createIOWaitAction(m_handle, booster::async::Action::IOEventType::IO_EVENT_WRITE);
      }
      return IOError::RETRY_WRITE; // For async io. In case socket is non-blocking
    } else if(e == WSAEINTR) {
      return IOError::RETRY_WRITE;
    } else if(e == WSAECONNRESET) {
      return IOError::BROKEN_PIPE;
    } else {
      //BOOSTER_LOGd("Connection", "write errno={}", e)
      return IOError::BROKEN_PIPE; // Consider all other errors as a broken pipe.
    }
  }
  return result;

#else

  errno = 0;
  v_int32 flags = 0;

#ifdef MSG_NOSIGNAL
  flags |= MSG_NOSIGNAL;
#endif

  auto result = ::send(m_handle, buff, static_cast<size_t>(count), flags);

  if(result < 0) {
    auto e = errno;

    bool retry = ((e == EAGAIN) || (e == EWOULDBLOCK));

    if(retry){
      if(m_mode == data::stream::ASYNCHRONOUS) {
        action = booster::async::Action::createIOWaitAction(m_handle, booster::async::Action::IOEventType::IO_EVENT_WRITE);
      }
      return IOError::RETRY_WRITE; // For async io. In case socket is non-blocking
    }

    if(e == EINTR) {
      return IOError::RETRY_WRITE;
    }

    if(e == EPIPE) {
      return IOError::BROKEN_PIPE;
    }

    //BOOSTER_LOGd("Connection", "write errno={}", e)
    return IOError::BROKEN_PIPE; // Consider all other errors as a broken pipe.
  }
  return result;

#endif

}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlogical-op"
#endif

v_io_size Connection::read(void *buff, v_buff_size count, async::Action& action){

#if defined(WIN32) || defined(_WIN32)

  auto result = ::recv(m_handle, (char*)buff, (int)count, 0);

  if(result == SOCKET_ERROR) {

    auto e = WSAGetLastError();

    if(e == WSAEWOULDBLOCK){
      if(m_mode == data::stream::ASYNCHRONOUS) {
        action = booster::async::Action::createIOWaitAction(m_handle, booster::async::Action::IOEventType::IO_EVENT_READ);
      }
      return IOError::RETRY_READ; // For async io. In case socket is non-blocking
    } else if(e == WSAEINTR) {
      return IOError::RETRY_READ;
    } else if(e == WSAECONNRESET) {
      return IOError::BROKEN_PIPE;
    } else {
      //BOOSTER_LOGd("Connection", "write errno={}", e)
      return IOError::BROKEN_PIPE; // Consider all other errors as a broken pipe.
    }
  }
  return result;

#else

  errno = 0;

  auto result = ::read(m_handle, buff, static_cast<size_t>(count));

  if(result < 0) {
    auto e = errno;

    bool retry = ((e == EAGAIN) || (e == EWOULDBLOCK));

    if(retry){
      if(m_mode == data::stream::ASYNCHRONOUS) {
        action = booster::async::Action::createIOWaitAction(m_handle, booster::async::Action::IOEventType::IO_EVENT_READ);
      }
      return IOError::RETRY_READ; // For async io. In case socket is non-blocking
    }

    if(e == EINTR) {
      return IOError::RETRY_READ;
    }

    if(e == ECONNRESET) {
      return IOError::BROKEN_PIPE;
    }

    //BOOSTER_LOGd("Connection", "write errno={}", e)
    return IOError::BROKEN_PIPE; // Consider all other errors as a broken pipe.
  }
  return result;

#endif

}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#if defined(WIN32) || defined(_WIN32)
void Connection::setStreamIOMode(booster::data::stream::IOMode ioMode) {

  u_long flags;

  switch(ioMode) {
    case data::stream::BLOCKING:
      flags = 0;
      if(NO_ERROR != ioctlsocket(m_handle, FIONBIO, &flags)) {
          throw std::runtime_error("[booster::network::tcp::Connection::setStreamIOMode()]: Error. Can't set stream I/O mode to IOMode::BLOCKING.");
      }
      m_mode = data::stream::BLOCKING;
      break;
    case data::stream::ASYNCHRONOUS:
      flags = 1;
      if(NO_ERROR != ioctlsocket(m_handle, FIONBIO, &flags)) {
          throw std::runtime_error("[booster::network::tcp::Connection::setStreamIOMode()]: Error. Can't set stream I/O mode to IOMode::ASYNCHRONOUS.");
      }
      m_mode = data::stream::ASYNCHRONOUS;
      break;
    default:
      break;
  }

}
#else
void Connection::setStreamIOMode(booster::data::stream::IOMode ioMode) {

  auto flags = fcntl(m_handle, F_GETFL);
  if (flags < 0) {
    throw std::runtime_error("[booster::network::tcp::Connection::setStreamIOMode()]: Error. Can't get socket flags.");
  }

  switch(ioMode) {

    case booster::data::stream::IOMode::BLOCKING:
      flags = flags & (~O_NONBLOCK);
      if (fcntl(m_handle, F_SETFL, flags) < 0) {
        throw std::runtime_error("[booster::network::tcp::Connection::setStreamIOMode()]: Error. Can't set stream I/O mode to IOMode::BLOCKING.");
      }
      m_mode = data::stream::BLOCKING;
      break;

    case booster::data::stream::IOMode::ASYNCHRONOUS:
      flags = (flags | O_NONBLOCK);
      if (fcntl(m_handle, F_SETFL, flags) < 0) {
        throw std::runtime_error("[booster::network::tcp::Connection::setStreamIOMode()]: Error. Can't set stream I/O mode to IOMode::ASYNCHRONOUS.");
      }
      m_mode = data::stream::ASYNCHRONOUS;
      break;

    default:
      break;

  }
}
#endif

void Connection::setOutputStreamIOMode(booster::data::stream::IOMode ioMode) {
  setStreamIOMode(ioMode);
}

booster::data::stream::IOMode Connection::getOutputStreamIOMode() {
  return m_mode;
}

booster::data::stream::Context& Connection::getOutputStreamContext() {
  return DEFAULT_CONTEXT;
}

void Connection::setInputStreamIOMode(booster::data::stream::IOMode ioMode) {
  setStreamIOMode(ioMode);
}

booster::data::stream::IOMode Connection::getInputStreamIOMode() {
  return m_mode;
}

booster::data::stream::Context& Connection::getInputStreamContext() {
  return DEFAULT_CONTEXT;
}

void Connection::close(){
#if defined(WIN32) || defined(_WIN32)
	::closesocket(m_handle);
#else
	::close(m_handle);
#endif
}

}}}
