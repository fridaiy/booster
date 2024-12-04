

#ifndef booster_web_server_HttpConnectionHandler_hpp
#define booster_web_server_HttpConnectionHandler_hpp

#include "booster/web/server/HttpProcessor.hpp"
#include "booster/network/ConnectionHandler.hpp"
#include "booster/concurrency/SpinLock.hpp"

#include <unordered_map>

namespace booster { namespace web { namespace server {


class HttpConnectionHandler : public base::Countable, public network::ConnectionHandler, public HttpProcessor::TaskProcessingListener {
protected:

  void onTaskStart(const provider::ResourceHandle<data::stream::IOStream>& connection) override;
  void onTaskEnd(const provider::ResourceHandle<data::stream::IOStream>& connection) override;

  void invalidateAllConnections();

private:
  std::shared_ptr<HttpProcessor::Components> m_components;
  std::atomic_bool m_continue;
  std::unordered_map<v_uint64, provider::ResourceHandle<data::stream::IOStream>> m_connections;
  booster::concurrency::SpinLock m_connectionsLock;
public:


  HttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components);


  HttpConnectionHandler(const std::shared_ptr<HttpRouter>& router)
    : HttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router))
  {}


  HttpConnectionHandler(const std::shared_ptr<HttpRouter>& router,
                        const std::shared_ptr<HttpProcessor::Config>& config)
    : HttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router, config))
  {}

public:


  static std::shared_ptr<HttpConnectionHandler> createShared(const std::shared_ptr<HttpRouter>& router);


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
