

#ifndef booster_web_server_AsyncHttpConnectionHandler_hpp
#define booster_web_server_AsyncHttpConnectionHandler_hpp

#include "booster/web/server/HttpProcessor.hpp"
#include "booster/network/ConnectionHandler.hpp"
#include "booster/async/Executor.hpp"
#include "booster/concurrency/SpinLock.hpp"

#include <unordered_map>

namespace booster { namespace web { namespace server {


class AsyncHttpConnectionHandler : public base::Countable, public network::ConnectionHandler, public HttpProcessor::TaskProcessingListener {
protected:

  void onTaskStart(const provider::ResourceHandle<data::stream::IOStream>& connection) override;
  void onTaskEnd(const provider::ResourceHandle<data::stream::IOStream>& connection) override;

  void invalidateAllConnections();

private:
  std::shared_ptr<booster::async::Executor> m_executor;
  std::shared_ptr<HttpProcessor::Components> m_components;
  std::atomic_bool m_continue;
  std::unordered_map<v_uint64, provider::ResourceHandle<data::stream::IOStream>> m_connections;
  booster::concurrency::SpinLock m_connectionsLock;
public:


  AsyncHttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components, v_int32 threadCount = booster::async::Executor::VALUE_SUGGESTED);


  AsyncHttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components, const std::shared_ptr<booster::async::Executor>& executor);


  AsyncHttpConnectionHandler(const std::shared_ptr<HttpRouter>& router, v_int32 threadCount = booster::async::Executor::VALUE_SUGGESTED)
    : AsyncHttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router), threadCount)
  {}


  AsyncHttpConnectionHandler(const std::shared_ptr<HttpRouter>& router, const std::shared_ptr<booster::async::Executor>& executor)
    : AsyncHttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router), executor)
  {}


  AsyncHttpConnectionHandler(const std::shared_ptr<HttpRouter>& router,
                             const std::shared_ptr<HttpProcessor::Config>& config,
                             v_int32 threadCount = booster::async::Executor::VALUE_SUGGESTED)
    : AsyncHttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router, config), threadCount)
  {}


  AsyncHttpConnectionHandler(const std::shared_ptr<HttpRouter>& router,
                             const std::shared_ptr<HttpProcessor::Config>& config,
                             const std::shared_ptr<booster::async::Executor>& executor)
    : AsyncHttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router, config), executor)
  {}

public:
  
  static std::shared_ptr<AsyncHttpConnectionHandler> createShared(const std::shared_ptr<HttpRouter>& router,
                                                                  v_int32 threadCount = booster::async::Executor::VALUE_SUGGESTED);
  
  static std::shared_ptr<AsyncHttpConnectionHandler> createShared(const std::shared_ptr<HttpRouter>& router,
                                                                  const std::shared_ptr<booster::async::Executor>& executor);
  
  static std::shared_ptr<AsyncHttpConnectionHandler> createShared(const std::shared_ptr<HttpProcessor::Components>& components,
                                                                  const std::shared_ptr<booster::async::Executor>& executor);

  static std::shared_ptr<AsyncHttpConnectionHandler> createShared(const std::shared_ptr<HttpProcessor::Components>& components,
                                                                  v_int32 threadCount = booster::async::Executor::VALUE_SUGGESTED);

  void setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler);


  void addRequestInterceptor(const std::shared_ptr<interceptor::RequestInterceptor>& interceptor);


  void addResponseInterceptor(const std::shared_ptr<interceptor::ResponseInterceptor>& interceptor);

  
  void handleConnection(const provider::ResourceHandle<IOStream>& connection,
                        const std::shared_ptr<const ParameterMap>& params) override;


  void stop() override;


  v_uint64 getConnectionsCount();
  
};
  
}}}

#endif

