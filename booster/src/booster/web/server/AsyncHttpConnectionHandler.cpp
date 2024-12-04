

#include "./AsyncHttpConnectionHandler.hpp"

namespace booster { namespace web { namespace server {

void AsyncHttpConnectionHandler::onTaskStart(const provider::ResourceHandle<data::stream::IOStream>& connection) {

  std::lock_guard<booster::concurrency::SpinLock> lock(m_connectionsLock);
  m_connections.insert({reinterpret_cast<v_uint64>(connection.object.get()), connection});

  if(!m_continue.load()) {
    connection.invalidator->invalidate(connection.object);
  }

}

void AsyncHttpConnectionHandler::onTaskEnd(const provider::ResourceHandle<data::stream::IOStream>& connection) {
  std::lock_guard<booster::concurrency::SpinLock> lock(m_connectionsLock);
  m_connections.erase(reinterpret_cast<v_uint64>(connection.object.get()));
}

void AsyncHttpConnectionHandler::invalidateAllConnections() {
  std::lock_guard<booster::concurrency::SpinLock> lock(m_connectionsLock);
  for(auto& c : m_connections) {
    const auto& handle = c.second;
    handle.invalidator->invalidate(handle.object);
  }
}

v_uint64 AsyncHttpConnectionHandler::getConnectionsCount() {
  std::lock_guard<booster::concurrency::SpinLock> lock(m_connectionsLock);
  return m_connections.size();
}

AsyncHttpConnectionHandler::AsyncHttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components,
                                                       v_int32 threadCount)
  : m_executor(std::make_shared<booster::async::Executor>(threadCount))
  , m_components(components)
  , m_continue(true)
{
  m_executor->detach();
}

AsyncHttpConnectionHandler::AsyncHttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components,
                                                       const std::shared_ptr<booster::async::Executor>& executor)
  : m_executor(executor)
  , m_components(components)
  , m_continue(true)
{}

std::shared_ptr<AsyncHttpConnectionHandler> AsyncHttpConnectionHandler::createShared(const std::shared_ptr<HttpRouter>& router, v_int32 threadCount){
  return std::make_shared<AsyncHttpConnectionHandler>(router, threadCount);
}

std::shared_ptr<AsyncHttpConnectionHandler> AsyncHttpConnectionHandler::createShared(const std::shared_ptr<HttpRouter>& router, const std::shared_ptr<booster::async::Executor>& executor){
  return std::make_shared<AsyncHttpConnectionHandler>(router, executor);
}

std::shared_ptr<AsyncHttpConnectionHandler> AsyncHttpConnectionHandler::createShared(const std::shared_ptr<HttpProcessor::Components>& components, const std::shared_ptr<booster::async::Executor>& executor){
  return std::make_shared<AsyncHttpConnectionHandler>(components, executor);
}

std::shared_ptr<AsyncHttpConnectionHandler> AsyncHttpConnectionHandler::createShared(const std::shared_ptr<HttpProcessor::Components>& components, v_int32 threadCount){
  return std::make_shared<AsyncHttpConnectionHandler>(components, threadCount);
}

void AsyncHttpConnectionHandler::setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler){
  m_components->errorHandler = errorHandler;
  if(!m_components->errorHandler) {
    m_components->errorHandler = std::make_shared<handler::DefaultErrorHandler>();
  }
}

void AsyncHttpConnectionHandler::addRequestInterceptor(const std::shared_ptr<interceptor::RequestInterceptor>& interceptor) {
  m_components->requestInterceptors.push_back(interceptor);
}

void AsyncHttpConnectionHandler::addResponseInterceptor(const std::shared_ptr<interceptor::ResponseInterceptor>& interceptor) {
  m_components->responseInterceptors.push_back(interceptor);
}

void AsyncHttpConnectionHandler::handleConnection(const provider::ResourceHandle<IOStream>& connection,
                                                  const std::shared_ptr<const ParameterMap>& params)
{

  (void)params;

  if (m_continue.load()) {

    connection.object->setOutputStreamIOMode(booster::data::stream::IOMode::ASYNCHRONOUS);
    connection.object->setInputStreamIOMode(booster::data::stream::IOMode::ASYNCHRONOUS);

    m_executor->execute<HttpProcessor::Coroutine>(m_components, connection, this);

  }
  
}

void AsyncHttpConnectionHandler::stop() {
  m_continue.store(false);


  invalidateAllConnections();


  while(getConnectionsCount() > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
  
}}}
