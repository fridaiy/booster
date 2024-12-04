

#include "HttpRouter.hpp"

namespace booster { namespace web { namespace server {

std::shared_ptr<HttpRouter> HttpRouter::createShared() {
  return std::make_shared<HttpRouter>();
}

void HttpRouter::route(const std::shared_ptr<server::api::Endpoint>& endpoint) {
  route(endpoint->info()->method, endpoint->info()->path, endpoint->handler);
}

void HttpRouter::route(const server::api::Endpoints& endpoints) {
  for(auto& e : endpoints.list) {
    route(e);
  }
}

std::shared_ptr<server::api::ApiController> HttpRouter::addController(const std::shared_ptr<server::api::ApiController>& controller) {
  m_controllers.push_back(controller);
  route(controller->getEndpoints());
  return controller;
}

}}}
