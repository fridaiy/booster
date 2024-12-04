

#ifndef booster_web_url_mapping_Router_hpp
#define booster_web_url_mapping_Router_hpp

#include "./Pattern.hpp"

#include "booster/Types.hpp"

#include <utility>
#include <list>

namespace booster { namespace web { namespace url { namespace mapping {


template<typename Endpoint>
class Router : public base::Countable {
private:


  typedef std::pair<std::shared_ptr<Pattern>, Endpoint> Pair;


  typedef booster::data::share::StringKeyLabel StringKeyLabel;
public:


  class Route {
  private:
    bool m_valid;
    Endpoint m_endpoint;
    Pattern::MatchMap m_matchMap;
  public:


    Route()
      : m_valid(false)
    {}


    Route(const Endpoint& endpoint, Pattern::MatchMap&& matchMap)
      : m_valid(true)
      , m_endpoint(endpoint)
      , m_matchMap(matchMap)
    {}


    const Endpoint& getEndpoint() {
      return m_endpoint;
    }


    const Pattern::MatchMap& getMatchMap() {
      return m_matchMap;
    }


    bool isValid() {
      return m_valid;
    }
    
    explicit operator bool() const {
      return m_valid;
    }
    
  };
  
private:
  std::list<Pair> m_endpointsByPattern;
public:
  
  static std::shared_ptr<Router> createShared(){
    return std::make_shared<Router>();
  }


  void route(const booster::String& pathPattern, const Endpoint& endpoint) {
    auto pattern = Pattern::parse(pathPattern);
    m_endpointsByPattern.push_back({pattern, endpoint});
  }


  Route getRoute(const StringKeyLabel& path){

    for(auto& pair : m_endpointsByPattern) {
      Pattern::MatchMap matchMap;
      if(pair.first->match(path, matchMap)) {
        return Route(pair.second, std::move(matchMap));
      }
    }

    return Route();
  }
  
  void logRouterMappings(const booster::data::share::StringKeyLabel &branch) {

    for(auto& pair : m_endpointsByPattern) {
      auto mapping = pair.first->toString();
      BOOSTER_LOGd("Router", "url '{} {}' -> mapped", reinterpret_cast<const char*>(branch.getData()), mapping)
    }

  }
  
};
  
}}}}

#endif
