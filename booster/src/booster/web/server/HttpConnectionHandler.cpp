

#include "./HttpConnectionHandler.hpp"

#include "booster/web/protocol/http/incoming/Request.hpp"
#include "booster/web/protocol/http/Http.hpp"

#include "booster/concurrency/Utils.hpp"

#include "booster/data/buffer/IOBuffer.hpp"

#include "booster/data/stream/BufferStream.hpp"
#include "booster/data/stream/StreamBufferedProxy.hpp"


namespace booster { namespace web { namespace server {

void HttpConnectionHandler::onTaskStart(const provider::ResourceHandle<data::stream::IOStream>& connection) {

  std::lock_guard<booster::concurrency::SpinLock> lock(m_connectionsLock);
  m_connections.insert({reinterpret_cast<v_uint64>(connection.object.get()), connection});

  if(!m_continue.load()) {
    connection.invalidator->invalidate(connection.object);
  }

}

void HttpConnectionHandler::onTaskEnd(const provider::ResourceHandle<data::stream::IOStream>& connection) {
  std::lock_guard<booster::concurrency::SpinLock> lock(m_connectionsLock);
  m_connections.erase(reinterpret_cast<v_uint64>(connection.object.get()));
}

void HttpConnectionHandler::invalidateAllConnections() {
  std::lock_guard<booster::concurrency::SpinLock> lock(m_connectionsLock);
  for(auto& c : m_connections) {
    const auto& handle = c.second;
    handle.invalidator->invalidate(handle.object);
  }
}

v_uint64 HttpConnectionHandler::getConnectionsCount() {
  std::lock_guard<booster::concurrency::SpinLock> lock(m_connectionsLock);
  return m_connections.size();
}

HttpConnectionHandler::HttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components)
  : m_components(components)
  , m_continue(true)
{}

std::shared_ptr<HttpConnectionHandler> HttpConnectionHandler::createShared(const std::shared_ptr<HttpRouter>& router){
  return std::make_shared<HttpConnectionHandler>(router);
}

void HttpConnectionHandler::setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler){
  m_components->errorHandler = errorHandler;
  if(!m_components->errorHandler) {
    m_components->errorHandler = std::make_shared<handler::DefaultErrorHandler>();
  }
}

void HttpConnectionHandler::addRequestInterceptor(const std::shared_ptr<interceptor::RequestInterceptor>& interceptor) {
  m_components->requestInterceptors.push_back(interceptor);
}

void HttpConnectionHandler::addResponseInterceptor(const std::shared_ptr<interceptor::ResponseInterceptor>& interceptor) {
  m_components->responseInterceptors.push_back(interceptor);
}
  
void HttpConnectionHandler::handleConnection(const provider::ResourceHandle<data::stream::IOStream>& connection,
                                             const std::shared_ptr<const ParameterMap>& params)
{

  (void)params;

  if (m_continue.load()) {

    connection.object->setOutputStreamIOMode(booster::data::stream::IOMode::BLOCKING);
    connection.object->setInputStreamIOMode(booster::data::stream::IOMode::BLOCKING);


    std::thread thread(&HttpProcessor::Task::run, std::move(HttpProcessor::Task(m_components, connection, this)));


    v_int32 concurrency = booster::concurrency::Utils::getHardwareConcurrency();
    if (concurrency > 1) {
      concurrency -= 1;
    }


    booster::concurrency::Utils::setThreadAffinityToCpuRange(thread.native_handle(),
                                                           0,
                                                           concurrency - 1 );

    thread.detach();
  }

}

void HttpConnectionHandler::stop() {
  m_continue.store(false);


  invalidateAllConnections();


  while(getConnectionsCount() > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

}}}
