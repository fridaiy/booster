

#ifndef booster_web_server_api_Controller_hpp
#define booster_web_server_api_Controller_hpp

#include "./Endpoint.hpp"

#include "booster/web/mime/ContentMappers.hpp"
#include "booster/web/server/handler/AuthorizationHandler.hpp"
#include "booster/web/server/handler/ErrorHandler.hpp"
#include "booster/web/server/handler/AuthorizationHandler.hpp"
#include "booster/web/server/HttpServerError.hpp"
#include "booster/web/protocol/http/incoming/Response.hpp"
#include "booster/web/protocol/http/outgoing/Request.hpp"
#include "booster/web/protocol/http/outgoing/ResponseFactory.hpp"

#include "booster/utils/Conversion.hpp"
#include "booster/base/Log.hpp"

#include <list>
#include <unordered_map>

namespace booster { namespace web { namespace server { namespace api {


class ApiController : public booster::base::Countable {
protected:
  typedef ApiController __ControllerType;
public:


  typedef booster::web::protocol::http::outgoing::ResponseFactory ResponseFactory;


  typedef booster::web::protocol::http::incoming::Request IncomingRequest;


  typedef booster::web::protocol::http::outgoing::Request OutgoingRequest;


  typedef booster::web::protocol::http::incoming::Response IncomingResponse;


  typedef booster::web::protocol::http::outgoing::Response OutgoingResponse;


  typedef booster::web::protocol::http::Status Status;


  typedef booster::web::protocol::http::Header Header;


  typedef booster::web::protocol::http::QueryParams QueryParams;


  typedef booster::web::server::HttpRequestHandler RequestHandler;


  typedef booster::web::server::handler::AuthorizationHandler AuthorizationHandler;
  
public:


  typedef booster::data::mapping::ObjectMapper ObjectMapper;


  typedef booster::String String;


  typedef booster::Int8 Int8;


  typedef booster::UInt8 UInt8;


  typedef booster::Int16 Int16;


  typedef booster::UInt16 UInt16;


  typedef booster::Int32 Int32;


  typedef booster::UInt32 UInt32;


  typedef booster::Int64 Int64;


  typedef booster::UInt64 UInt64;


  typedef booster::Float32 Float32;


  typedef booster::Float64 Float64;


  typedef booster::Boolean Boolean;


  typedef std::function<std::shared_ptr<Endpoint::Info>()> EndpointInfoBuilder;

  template <class T>
  using Object = booster::Object<T>;

  template <class T>
  using List = booster::List<T>;

  template <class Value>
  using Fields = booster::Fields<Value>;

  template <class T>
  using Enum = booster::data::type::Enum<T>;

protected:
  

  template<class CoroutineT, class ControllerT>
  class HandlerCoroutine : public booster::async::CoroutineWithResult<CoroutineT, const std::shared_ptr<OutgoingResponse>&> {
  public:
    
    HandlerCoroutine(ControllerT* pController, const std::shared_ptr<IncomingRequest>& pRequest)
      : controller(pController)
      , request(pRequest)
    {}
    
    ControllerT* const controller;
    std::shared_ptr<IncomingRequest> request;
    
  };
  

  template<class T>
  class Handler : public RequestHandler {
  public:
    typedef std::shared_ptr<OutgoingResponse> (T::*Method)(const std::shared_ptr<IncomingRequest>&);
    typedef booster::async::CoroutineStarterForResult<const std::shared_ptr<OutgoingResponse>&>
            (T::*MethodAsync)(const std::shared_ptr<IncomingRequest>&);

  private:

    class ErrorHandlingCoroutine : public booster::async::CoroutineWithResult<ErrorHandlingCoroutine, const std::shared_ptr<OutgoingResponse>&> {
    private:
      Handler* m_handler;
      std::shared_ptr<IncomingRequest> m_request;
    public:

      ErrorHandlingCoroutine(Handler* handler, const std::shared_ptr<IncomingRequest>& request)
        : m_handler(handler)
        , m_request(request)
      {}

      async::Action act() override {
        return (m_handler->m_controller->*m_handler->m_methodAsync)(m_request)
          .callbackTo(&ErrorHandlingCoroutine::onResponse);
      }

      async::Action onResponse(const std::shared_ptr<OutgoingResponse>& response) {
        return this->_return(response);
      }

      async::Action handleError(async::Error* error) override {

        std::exception_ptr ePtr = error->getExceptionPtr();
        if(!ePtr) {
          ePtr = std::make_exception_ptr(*error);
        }

        try {
          try {
            std::rethrow_exception(ePtr);
          } catch (...) {
            std::throw_with_nested(HttpServerError(m_request, "[ApiController]: Error processing async request"));
          }
        } catch (...) {
          ePtr = std::current_exception();
          auto response = m_handler->m_controller->handleError(ePtr);
          if (response != nullptr) {
            return this->_return(response);
          }
        }

        return new async::Error(ePtr);

      }

    };

  private:
    T* m_controller;
    Method m_method;
    MethodAsync m_methodAsync;
  public:
    Handler(T* controller, Method method, MethodAsync methodAsync)
      : m_controller(controller)
      , m_method(method)
      , m_methodAsync(methodAsync)
    {}
  public:
    
    static std::shared_ptr<Handler> createShared(T* controller, Method method, MethodAsync methodAsync){
      return std::make_shared<Handler>(controller, method, methodAsync);
    }
    
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {

      if(m_method == nullptr) {
        if(m_methodAsync == nullptr) {
          throw HttpServerError(request, "[ApiController]: Error. Handler method is nullptr");
        }
        throw HttpServerError(request, "[ApiController]: Error. Non-async call to async endpoint");
      }

      try {
        try {
          return (m_controller->*m_method)(request);
        } catch (...) {
          std::throw_with_nested(HttpServerError(request, "[ApiController]: Error processing request"));
        }
      } catch (...) {
        auto ePtr = std::current_exception();
        auto response = m_controller->handleError(ePtr);
        if (response != nullptr) {
          return response;
        }
        std::rethrow_exception(ePtr);
      }
      
    }
    
    booster::async::CoroutineStarterForResult<const std::shared_ptr<OutgoingResponse>&>
    handleAsync(const std::shared_ptr<protocol::http::incoming::Request>& request) override {

      if(m_methodAsync == nullptr) {
        if(m_method == nullptr) {
          throw HttpServerError(request, "[ApiController]: Error. Handler method is nullptr");
        }
        throw HttpServerError(request, "[ApiController]: Error. Async call to non-async endpoint");
      }

      //if(m_controller->m_errorHandler) {
        return ErrorHandlingCoroutine::startForResult(this, request);
      //}

      //return (m_controller->*m_methodAsync)(request);

    }

    Method setMethod(Method method) {
      auto prev = m_method;
      m_method = method;
      return prev;
    }

    Method getMethod() {
      return m_method;
    }

    MethodAsync setMethodAsync(MethodAsync methodAsync) {
      auto prev = m_methodAsync;
      m_methodAsync = methodAsync;
      return prev;
    }

    MethodAsync getMethodAsync() {
      return m_methodAsync;
    }
    
  };

protected:


  void setEndpointInfo(const std::string& endpointName, const std::shared_ptr<Endpoint::Info>& info);


  std::shared_ptr<Endpoint::Info> getEndpointInfo(const std::string& endpointName);


  void setEndpointHandler(const std::string& endpointName, const std::shared_ptr<RequestHandler>& handler);


  std::shared_ptr<RequestHandler> getEndpointHandler(const std::string& endpointName);
  
protected:
  Endpoints m_endpoints;
  std::shared_ptr<handler::ErrorHandler> m_errorHandler;
  std::shared_ptr<handler::AuthorizationHandler> m_defaultAuthorizationHandler;
  std::shared_ptr<mime::ContentMappers> m_contentMappers;
  std::unordered_map<std::string, std::shared_ptr<Endpoint::Info>> m_endpointInfo;
  std::unordered_map<std::string, std::shared_ptr<RequestHandler>> m_endpointHandlers;
  const booster::String m_routerPrefix;
public:

  ApiController(const std::shared_ptr<mime::ContentMappers>& contentMappers, const booster::String &routerPrefix = nullptr)
    : m_contentMappers(contentMappers)
    , m_routerPrefix(routerPrefix)
  {

  }

  ApiController(const std::shared_ptr<booster::data::mapping::ObjectMapper>& defaultObjectMapper, const booster::String &routerPrefix = nullptr)
    : m_contentMappers(std::make_shared<mime::ContentMappers>())
    , m_routerPrefix(routerPrefix)
  {
    m_contentMappers->setDefaultMapper(defaultObjectMapper);
  }
public:
  
  template<class T>
  static std::shared_ptr<Endpoint> createEndpoint(Endpoints& endpoints,
                                                  const std::shared_ptr<Handler<T>>& handler,
                                                  const EndpointInfoBuilder& infoBuilder)
  {
    auto endpoint = Endpoint::createShared(handler, infoBuilder);
    endpoints.append(endpoint);
    return endpoint;
  }
  

  const Endpoints& getEndpoints();


  void setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler);


  std::shared_ptr<OutgoingResponse> handleError(const std::exception_ptr& exceptionPtr) const;


  void setDefaultAuthorizationHandler(const std::shared_ptr<handler::AuthorizationHandler>& authorizationHandler);


  std::shared_ptr<handler::AuthorizationHandler> getDefaultAuthorizationHandler();


  std::shared_ptr<handler::AuthorizationObject> handleDefaultAuthorization(const String &authHeader) const;


  const std::shared_ptr<mime::ContentMappers>& getContentMappers() const;
  
  // Helper methods
  
  std::shared_ptr<OutgoingResponse> createResponse(const Status& status,
                                                   const booster::String& str) const;
  
  std::shared_ptr<OutgoingResponse> createResponse(const Status& status) const;

  std::shared_ptr<OutgoingResponse> createDtoResponse(const Status& status,
                                                      const booster::Void& dto,
                                                      const std::shared_ptr<booster::data::mapping::ObjectMapper>& objectMapper) const;
  
  std::shared_ptr<OutgoingResponse> createDtoResponse(const Status& status,
                                                      const booster::Void& dto) const;

  std::shared_ptr<ApiController::OutgoingResponse> createDtoResponse(const Status& status,
                                                                     const booster::Void& dto,
                                                                     const std::vector<booster::String>& acceptableContentTypes) const;

public:

  template<typename T>
  struct TypeInterpretation {

    static T fromString(const booster::String& typeName, const booster::String& text, bool& success) {
      (void) text;
      success = false;
      BOOSTER_LOGe("[booster::web::server::api::ApiController::TypeInterpretation::fromString()]",
                 "Error. No conversion from '{}' to '{}' is defined.", "booster::String", typeName)
      throw std::runtime_error("[booster::web::server::api::ApiController::TypeInterpretation::fromString()]: Error. "
                               "No conversion from 'booster::String' to '" + *typeName + "' is defined. "
                               "Please define type conversion.");
    }

  };

};

template<>
struct ApiController::TypeInterpretation <booster::String> {
  static booster::String fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    success = true;
    return text;
  }
};

template<>
struct ApiController::TypeInterpretation <booster::Int8> {
  static booster::Int8 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    //TODO: check the range and perhaps throw an exception if the variable doesn't fit
    return static_cast<Int8::UnderlyingType>(utils::Conversion::strToInt32(text, success));
  }
};

template<>
struct ApiController::TypeInterpretation <booster::UInt8> {
  static booster::UInt8 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    //TODO: check the range and perhaps throw an exception if the variable doesn't fit
    return static_cast<UInt8::UnderlyingType>(utils::Conversion::strToUInt32(text, success));
  }
};

template<>
struct ApiController::TypeInterpretation <booster::Int16> {
  static booster::Int16 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    //TODO: check the range and perhaps throw an exception if the variable doesn't fit
    return static_cast<Int16::UnderlyingType>(utils::Conversion::strToInt32(text, success));
  }
};

template<>
struct ApiController::TypeInterpretation <booster::UInt16> {
  static booster::UInt16 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    //TODO: check the range and perhaps throw an exception if the variable doesn't fit
    return static_cast<UInt16::UnderlyingType>(utils::Conversion::strToUInt32(text, success));
  }
};

template<>
struct ApiController::TypeInterpretation <booster::Int32> {
  static booster::Int32 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    return utils::Conversion::strToInt32(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <booster::UInt32> {
  static booster::UInt32 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    return utils::Conversion::strToUInt32(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <booster::Int64> {
  static booster::Int64 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    return utils::Conversion::strToInt64(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <booster::UInt64> {
  static booster::UInt64 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    return utils::Conversion::strToUInt64(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <booster::Float32> {
  static booster::Float32 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    return utils::Conversion::strToFloat32(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <booster::Float64> {
  static booster::Float64 fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    return utils::Conversion::strToFloat64(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <booster::Boolean> {
  static booster::Boolean fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    (void) typeName;
    return utils::Conversion::strToBool(text, success);
  }
};

template<class T, class I>
struct ApiController::TypeInterpretation <data::type::EnumObjectWrapper<T, I>> {

  typedef data::type::EnumObjectWrapper<T, I> EnumOW;
  typedef typename I::UnderlyingTypeObjectWrapper UTOW;

  static EnumOW fromString(const booster::String& typeName, const booster::String& text, bool& success) {
    const auto& parsedValue = ApiController::TypeInterpretation<UTOW>::fromString(typeName, text, success);
    if(success) {
      data::type::EnumInterpreterError error = data::type::EnumInterpreterError::OK;
      const auto& result = I::fromInterpretation(parsedValue, false, error);
      if(error == data::type::EnumInterpreterError::OK) {
        return result.template cast<EnumOW>();
      }
      success = false;
    }
    return nullptr;
  }

};

}}}}

#endif
