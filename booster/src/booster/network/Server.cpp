

#include "Server.hpp"

#include "booster/base/Log.hpp"

#include <thread>
#include <chrono>

namespace booster { namespace network {

const v_int32 Server::STATUS_CREATED = 0;
const v_int32 Server::STATUS_STARTING = 1;
const v_int32 Server::STATUS_RUNNING = 2;
const v_int32 Server::STATUS_STOPPING = 3;
const v_int32 Server::STATUS_DONE = 4;

Server::Server(const std::shared_ptr<ConnectionProvider> &connectionProvider,
               const std::shared_ptr<ConnectionHandler> &connectionHandler)
    : m_status(STATUS_CREATED)
    , m_connectionProvider(connectionProvider)
    , m_connectionHandler(connectionHandler)
    , m_threaded(false) {}

// This isn't implemented as static since threading is dropped and therefore static isn't needed anymore.
void Server::conditionalMainLoop() {

  setStatus(STATUS_STARTING, STATUS_RUNNING);
  std::shared_ptr<const std::unordered_map<booster::String, booster::String>> params;

  while (getStatus() == STATUS_RUNNING) {

    if (m_condition()) {

      auto connectionHandle = m_connectionProvider->get();

      if (connectionHandle.object) {
        if (getStatus() == STATUS_RUNNING) {
          if (m_condition()) {
            m_connectionHandler->handleConnection(connectionHandle, params );
          } else {
            setStatus(STATUS_STOPPING);
          }
        } else {
          BOOSTER_LOGd("[booster::network::server::mainLoop()]", "Error. Server already stopped - closing connection...")
        }
      }

    } else {
      setStatus(STATUS_STOPPING);
    }
  }
  setStatus(STATUS_DONE);
}

void Server::mainLoop(Server *instance) {

  instance->setStatus(STATUS_STARTING, STATUS_RUNNING);
  std::shared_ptr<const std::unordered_map<booster::String, booster::String>> params;

  while (instance->getStatus() == STATUS_RUNNING) {

    auto connectionHandle = instance->m_connectionProvider->get();

    if (connectionHandle) {
      if (instance->getStatus() == STATUS_RUNNING) {
        instance->m_connectionHandler->handleConnection(connectionHandle, params );
      } else {
        BOOSTER_LOGd("[booster::network::server::mainLoop()]", "Error. Server already stopped - closing connection...")
      }
    }
  }

  instance->setStatus(STATUS_DONE);

}

void Server::run(std::function<bool()> conditional) {
  std::unique_lock<std::mutex> ul(m_mutex);
  switch (getStatus()) {
    case STATUS_STARTING:
      throw std::runtime_error("[booster::network::server::run()] Error. Server already starting");
    case STATUS_RUNNING:
      throw std::runtime_error("[booster::network::server::run()] Error. Server already started");
    default:
      break;
  }

  m_threaded = false;
  setStatus(STATUS_CREATED, STATUS_STARTING);

  if (conditional) {
    m_condition = std::move(conditional);
    ul.unlock(); // early unlock
    conditionalMainLoop();
  } else {
    ul.unlock();
    mainLoop(this);
  }
}

void Server::run(bool startAsNewThread) {
  std::unique_lock<std::mutex> ul(m_mutex);
  BOOSTER_LOGw("[booster::network::server::run(bool)]", "Using booster::network::server::run(bool) is deprecated and will be removed in the next release. Please implement your own threading (See https://github.com/booster/booster-threaded-starter).")
  switch (getStatus()) {
    case STATUS_STARTING:
      throw std::runtime_error("[booster::network::server::run()] Error. Server already starting");
    case STATUS_RUNNING:
      throw std::runtime_error("[booster::network::server::run()] Error. Server already started");
    default:
      break;
  }

  m_threaded = startAsNewThread;
  setStatus(STATUS_CREATED, STATUS_STARTING);

  if (m_threaded) {
    m_thread = std::thread(mainLoop, this);
  } else {
    ul.unlock(); // early unlock
    mainLoop(this);
  }
}

void Server::stop() {
  std::lock_guard<std::mutex> lg(m_mutex);
  switch (getStatus()) {
    case STATUS_CREATED:
      return;
    case STATUS_STARTING:
    case STATUS_RUNNING:
      setStatus(STATUS_STOPPING);
      break;
    default:
      break;
  }

  if (m_threaded && m_thread.joinable()) {
    m_thread.join();
  }
}

bool Server::setStatus(v_int32 expectedStatus, v_int32 newStatus) {
  v_int32 expected = expectedStatus;
  return m_status.compare_exchange_strong(expected, newStatus);
}

void Server::setStatus(v_int32 status) {
  m_status.store(status);
}

v_int32 Server::getStatus() {
  return m_status.load();
}

Server::~Server() {
  stop();
}

}}
