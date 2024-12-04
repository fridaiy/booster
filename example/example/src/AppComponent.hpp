#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "booster/web/server/AsyncHttpConnectionHandler.hpp"
#include "booster/web/server/HttpRouter.hpp"
#include "booster/network/tcp/server/ConnectionProvider.hpp"

#include "booster/web/mime/ContentMappers.hpp"
#include "booster/json/ObjectMapper.hpp"

#include "booster/macro/component.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:

  /**
   * Create Async Executor
   */
  BOOSTER_CREATE_COMPONENT(std::shared_ptr<booster::async::Executor>, executor)([] {
    return std::make_shared<booster::async::Executor>(
      4 /* Data-Processing threads */,
      1 /* I/O threads */,
      1 /* Timer threads */
    );
  }());
  
  /**
   *  Create ConnectionProvider component which listens on the port
   */
  BOOSTER_CREATE_COMPONENT(std::shared_ptr<booster::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    return booster::network::tcp::server::ConnectionProvider::createShared({"localhost", 8000});
  }());
  
  /**
   *  Create Router component
   */
  BOOSTER_CREATE_COMPONENT(std::shared_ptr<booster::web::server::HttpRouter>, httpRouter)([] {
    return booster::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  BOOSTER_CREATE_COMPONENT(std::shared_ptr<booster::network::ConnectionHandler>, serverConnectionHandler)([] {
    BOOSTER_COMPONENT(std::shared_ptr<booster::web::server::HttpRouter>, router); // get Router component
    BOOSTER_COMPONENT(std::shared_ptr<booster::async::Executor>, executor); // get Async executor component
    return booster::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
  }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  BOOSTER_CREATE_COMPONENT(std::shared_ptr<booster::web::mime::ContentMappers>, apiContentMappers)([] {

    auto json = std::make_shared<booster::json::ObjectMapper>();
    json->serializerConfig().json.useBeautifier = true;

    auto mappers = std::make_shared<booster::web::mime::ContentMappers>();
    mappers->putMapper(json);

    return mappers;

  }());

};

#endif /* AppComponent_hpp */
