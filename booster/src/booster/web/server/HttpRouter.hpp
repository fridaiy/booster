

#ifndef booster_web_server_HttpRouter_hpp
#define booster_web_server_HttpRouter_hpp

#include "./HttpRequestHandler.hpp"

#include "booster/web/server/api/ApiController.hpp"
#include "booster/web/server/api/Endpoint.hpp"
#include "booster/web/url/mapping/Router.hpp"

namespace booster { namespace web { namespace server {


template<typename RouterEndpoint>
class HttpRouterTemplate : public booster::base::Countable {
private:

  typedef data::share::StringKeyLabel StringKeyLabel;
public:


  typedef web::url::mapping::Router<RouterEndpoint> BranchRouter;


  typedef std::unordered_map<StringKeyLabel, std::shared_ptr<BranchRouter>> BranchMap;
protected:
  BranchMap m_branchMap;
protected:

  const std::shared_ptr<BranchRouter>& getBranch(const StringKeyLabel& name){
    auto it = m_branchMap.find(name);
    if(it == m_branchMap.end()){
      m_branchMap[name] = BranchRouter::createShared();
    }
    return m_branchMap[name];
  }

public:


  HttpRouterTemplate() = default;


  static std::shared_ptr<HttpRouterTemplate> createShared() {
    return std::make_shared<HttpRouterTemplate>();
  }


  void route(const booster::String& method, const booster::String& pathPattern, const RouterEndpoint& endpoint) {
    getBranch(method)->route(pathPattern, endpoint);
  }


  typename BranchRouter::Route getRoute(const StringKeyLabel& method, const StringKeyLabel& path){
    auto it = m_branchMap.find(method);
    if(it != m_branchMap.end()) {
      return m_branchMap[method]->getRoute(path);
    }
    return typename BranchRouter::Route();
  }


  void logRouterMappings() {
    for(auto it : m_branchMap) {
      it.second->logRouterMappings(it.first);
    }
  }
  
};


class HttpRouter : public HttpRouterTemplate<std::shared_ptr<HttpRequestHandler>> {
private:
  std::list<std::shared_ptr<server::api::ApiController>> m_controllers;
public:


  static std::shared_ptr<HttpRouter> createShared();

  using HttpRouterTemplate::route;
  void route(const std::shared_ptr<server::api::Endpoint>& endpoint);
  void route(const server::api::Endpoints& endpoints);


  std::shared_ptr<server::api::ApiController> addController(const std::shared_ptr<server::api::ApiController>& controller);

};
  
}}}

#endif
