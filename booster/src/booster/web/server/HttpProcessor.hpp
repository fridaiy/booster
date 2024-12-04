

#ifndef booster_web_server_HttpProcessor_hpp
#define booster_web_server_HttpProcessor_hpp

#include "./HttpRouter.hpp"

#include "./interceptor/RequestInterceptor.hpp"
#include "./interceptor/ResponseInterceptor.hpp"
#include "./handler/ErrorHandler.hpp"

#include "booster/web/protocol/http/encoding/ProviderCollection.hpp"

#include "booster/web/protocol/http/incoming/RequestHeadersReader.hpp"
#include "booster/web/protocol/http/incoming/Request.hpp"

#include "booster/web/protocol/http/outgoing/Response.hpp"
#include "booster/web/protocol/http/utils/CommunicationUtils.hpp"

#include "booster/data/stream/StreamBufferedProxy.hpp"
#include "booster/async/Processor.hpp"

namespace booster { namespace web { namespace server {


class HttpProcessor {
public:
  typedef std::list<std::shared_ptr<web::server::interceptor::RequestInterceptor>> RequestInterceptors;
  typedef std::list<std::shared_ptr<web::server::interceptor::ResponseInterceptor>> ResponseInterceptors;
  typedef web::protocol::http::incoming::RequestHeadersReader RequestHeadersReader;
  typedef protocol::http::utils::CommunicationUtils::ConnectionState ConnectionState;

  HttpProcessor() = default;

public:


  struct Config {


    v_buff_size headersInBufferInitial = 2048;


    v_buff_size headersOutBufferInitial = 2048;


    v_buff_size headersReaderChunkSize = 2048;


    v_buff_size headersReaderMaxSize = 4096;

  };

public:


  struct Components {


    Components(const std::shared_ptr<HttpRouter>& pRouter,
               const std::shared_ptr<protocol::http::encoding::ProviderCollection>& pContentEncodingProviders,
               const std::shared_ptr<const booster::web::protocol::http::incoming::BodyDecoder>& pBodyDecoder,
               const std::shared_ptr<handler::ErrorHandler>& pErrorHandler,
               const RequestInterceptors& pRequestInterceptors,
               const ResponseInterceptors& pResponseInterceptors,
               const std::shared_ptr<Config>& pConfig);


    Components(const std::shared_ptr<HttpRouter>& pRouter);


    Components(const std::shared_ptr<HttpRouter>& pRouter, const std::shared_ptr<Config>& pConfig);


    std::shared_ptr<HttpRouter> router;


    std::shared_ptr<protocol::http::encoding::ProviderCollection> contentEncodingProviders;


    std::shared_ptr<const booster::web::protocol::http::incoming::BodyDecoder> bodyDecoder;


    std::shared_ptr<handler::ErrorHandler> errorHandler;


    RequestInterceptors requestInterceptors;


    ResponseInterceptors responseInterceptors;


    std::shared_ptr<Config> config;

  };

private:

  struct ProcessingResources {

    ProcessingResources(const std::shared_ptr<Components>& pComponents,
                        const provider::ResourceHandle<booster::data::stream::IOStream>& pConnection);

    std::shared_ptr<Components> components;
    provider::ResourceHandle<booster::data::stream::IOStream> connection;
    booster::data::stream::BufferOutputStream headersInBuffer;
    booster::data::stream::BufferOutputStream headersOutBuffer;
    RequestHeadersReader headersReader;
    std::shared_ptr<booster::data::stream::InputStreamBufferedProxy> inStream;

  };

  static
  std::shared_ptr<protocol::http::outgoing::Response>
  processNextRequest(ProcessingResources& resources,
                     const std::shared_ptr<protocol::http::incoming::Request>& request,
                     ConnectionState& connectionState);
  static ConnectionState processNextRequest(ProcessingResources& resources);

public:


  class TaskProcessingListener {
  public:
    virtual void onTaskStart(const provider::ResourceHandle<data::stream::IOStream>& connection) = 0;
    virtual void onTaskEnd(const provider::ResourceHandle<data::stream::IOStream>& connection) = 0;
  };

public:


  class Task : public base::Countable {
  private:
    std::shared_ptr<Components> m_components;
    provider::ResourceHandle<booster::data::stream::IOStream> m_connection;
    TaskProcessingListener* m_taskListener;
  public:


    Task(const std::shared_ptr<Components>& components,
         const provider::ResourceHandle<booster::data::stream::IOStream>& connection,
         TaskProcessingListener* taskListener);

    Task(const Task&) = delete;
    Task &operator=(const Task&) = delete;


     Task(Task &&other);


    Task &operator=(Task &&other);


    ~Task() override;

  public:


    void run();

  };
  
public:


  class Coroutine : public booster::async::Coroutine<HttpProcessor::Coroutine> {
  private:
    std::shared_ptr<Components> m_components;
    provider::ResourceHandle<booster::data::stream::IOStream> m_connection;
    booster::data::stream::BufferOutputStream m_headersInBuffer;
    RequestHeadersReader m_headersReader;
    std::shared_ptr<booster::data::stream::BufferOutputStream> m_headersOutBuffer;
    std::shared_ptr<booster::data::stream::InputStreamBufferedProxy> m_inStream;
    ConnectionState m_connectionState;
  private:
    booster::web::server::HttpRouter::BranchRouter::Route m_currentRoute;
    std::shared_ptr<protocol::http::incoming::Request> m_currentRequest;
    std::shared_ptr<protocol::http::outgoing::Response> m_currentResponse;
    TaskProcessingListener* m_taskListener;
  private:
    bool m_shouldInterceptResponse;
  public:


    Coroutine(const std::shared_ptr<Components>& components,
              const provider::ResourceHandle<booster::data::stream::IOStream>& connection,
              TaskProcessingListener* taskListener);

    ~Coroutine() override;

    Action act() override;

    Action parseHeaders();
    
    Action onHeadersParsed(const RequestHeadersReader::Result& headersReadResult);
    
    Action onRequestFormed();
    Action onResponse(const std::shared_ptr<protocol::http::outgoing::Response>& response);
    Action onResponseFormed();
    Action onRequestDone();
    
    Action handleError(Error* error) override;
    
  };
  
};
  
}}}

#endif
