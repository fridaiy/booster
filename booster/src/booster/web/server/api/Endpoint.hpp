

#ifndef booster_web_server_api_Endpoint_hpp
#define booster_web_server_api_Endpoint_hpp

#include "booster/web/server/HttpRequestHandler.hpp"

#include <list>
#include <unordered_map>
#include <functional>

namespace booster { namespace web { namespace server { namespace api {


class Endpoint : public booster::base::Countable {
public:


  typedef booster::web::server::HttpRequestHandler RequestHandler;
public:


  class Info : public booster::base::Countable {
  public:


    struct Param {
      
      Param();
      Param(const booster::String& pName, booster::data::type::Type* pType);

      booster::String name;
      booster::data::type::Type* type;

      booster::String description;
      booster::Boolean required = true;
      booster::Boolean deprecated = false;
      booster::Boolean allowEmptyValue;
      std::list<std::pair<booster::String, booster::Any>> examples;

      Param& addExample(const booster::String& title, const booster::Any& example) {
        examples.push_back({title, example});
        return *this;
      }

    };


    class Params {
    private:
      std::list<booster::String> m_order;
      std::unordered_map<booster::String, Param> m_params;
    public:

      const std::list<booster::String>& getOrder() const;


      Param& add(const booster::String& name, booster::data::type::Type* type);


      template<class T>
      Param& add(const booster::String& aname) {
        return add(aname, T::Class::getType());
      }


      Param& operator [](const booster::String& name);

    };


    struct ContentHints {
      booster::String contentType;
      booster::data::type::Type* schema;
      booster::String description;
      std::list<std::pair<booster::String, booster::Any>> examples;

      ContentHints& addExample(const booster::String& title, const booster::Any& example) {
        examples.push_back({title, example});
        return *this;
      }
    };
    
  public:


    Info();


    static std::shared_ptr<Info> createShared();


    booster::String name;


    booster::String summary;


    booster::String description;


    booster::String path;


    booster::String method;


    booster::String authorization;


    booster::Boolean hide;


    std::list<booster::String> tags;


    Param body;


    booster::String bodyContentType;


    std::list<ContentHints> consumes;


    std::unordered_map<booster::String, std::shared_ptr<std::list<booster::String>>> securityRequirements;


    Params headers;


    Params pathParams;


    Params queryParams;
    

    std::unordered_map<booster::web::protocol::http::Status, ContentHints> responses;
    
    booster::String toString();


    template<class Wrapper>
    ContentHints& addConsumes(const booster::String& contentType, const booster::String& desc = booster::String()) {
      consumes.push_back({contentType, Wrapper::Class::getType(), desc});
      return consumes.back();
    }


    template<class Wrapper>
    ContentHints& addResponse(const booster::web::protocol::http::Status& status, const booster::String& contentType, const booster::String& responseDescription = booster::String()) {
      auto& hint = responses[status];
      hint.contentType = contentType;
      hint.description = responseDescription.get() == nullptr ? status.description : responseDescription;
      hint.schema = Wrapper::Class::getType();
      return hint;
    }


    ContentHints& addResponse(const booster::web::protocol::http::Status& status, const booster::String& responseDescription = booster::String()) {
      auto& hint = responses[status];
      hint.description = responseDescription.get() == nullptr ? status.description : responseDescription;
      return hint;
    }


    void addSecurityRequirement(const booster::String &requirement, const std::shared_ptr<std::list<booster::String>> &scopes = nullptr) {
      securityRequirements[requirement] = scopes;
    }


    void addTag(const booster::String& tag) {
      tags.push_back(tag);
    }

  };
public:

  Endpoint(const std::shared_ptr<RequestHandler>& pHandler, const std::function<std::shared_ptr<Endpoint::Info>()>& infoBuilder);
  
  static std::shared_ptr<Endpoint> createShared(const std::shared_ptr<RequestHandler>& handler,
                                                const std::function<std::shared_ptr<Endpoint::Info>()>& infoBuilder);
  
  const std::shared_ptr<RequestHandler> handler;

  std::shared_ptr<Info> info();

private:
  std::shared_ptr<Info> m_info;
  std::function<std::shared_ptr<Endpoint::Info>()> m_infoBuilder;
  
};


struct Endpoints {

  std::list<std::shared_ptr<Endpoint>> list;

  void append(const std::list<std::shared_ptr<Endpoint>>& endpoints);
  void append(const Endpoints& endpoints);
  void append(const std::shared_ptr<Endpoint>& endpoint);

};
  
}}}}

#endif
