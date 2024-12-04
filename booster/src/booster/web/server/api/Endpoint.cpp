

#include "Endpoint.hpp"

namespace booster { namespace web { namespace server { namespace api {

Endpoint::Info::Param::Param()
  : name(nullptr)
  , type(nullptr)
{}

Endpoint::Info::Param::Param(const booster::String& pName,
                             booster::data::type::Type* pType)
  : name(pName)
  , type(pType)
{}

const std::list<booster::String>& Endpoint::Info::Params::getOrder() const {
  return m_order;
}

Endpoint::Info::Param& Endpoint::Info::Params::add(const booster::String& aname, booster::data::type::Type* type) {
  m_order.push_back(aname);
  Endpoint::Info::Param& param = operator [](aname);
  param.name = aname;
  param.type = type;
  return param;
}

Endpoint::Info::Param& Endpoint::Info::Params::operator [](const booster::String& aname) {
  return m_params[aname];
}

Endpoint::Info::Info() : hide(false)
{}

std::shared_ptr<Endpoint::Info> Endpoint::Info::createShared(){
  return std::make_shared<Info>();
}

booster::String Endpoint::Info::toString() {
  booster::data::stream::BufferOutputStream stream;
  
  stream << "\nEndpoint\n";
  
  if(name) {
    stream << "name: '" << name << "'\n";
  }
  
  if(path){
    stream << "path: '" << path << "'\n";
  }
  
  if(method){
    stream << "method: '" << method << "'\n";
  }
  
  if(body.name != nullptr){
    stream << "body: '" << body.name << "', type: '" << body.type->classId.name << "'\n";
  }
  
  auto headerIt = headers.getOrder().begin();
  while (headerIt != headers.getOrder().end()) {
    auto header = headers[*headerIt++];
    stream << "header: '" << header.name << "', type: '" << header.type->classId.name << "'\n";
  }
  
  auto pathIt = pathParams.getOrder().begin();
  while (pathIt != pathParams.getOrder().end()) {
    auto param = pathParams[*pathIt++];
    stream << "pathParam: '" << param.name << "', type: '" << param.type->classId.name << "'\n";
  }
  
  return stream.toString();
}

Endpoint::Endpoint(const std::shared_ptr<RequestHandler>& pHandler,
                   const std::function<std::shared_ptr<Endpoint::Info>()>& infoBuilder)
  : handler(pHandler)
  , m_infoBuilder(infoBuilder)
{}

std::shared_ptr<Endpoint> Endpoint::createShared(const std::shared_ptr<RequestHandler>& handler,
                                                 const std::function<std::shared_ptr<Endpoint::Info>()>& infoBuilder){
  return std::make_shared<Endpoint>(handler, infoBuilder);
}

std::shared_ptr<Endpoint::Info> Endpoint::info() {
  if (m_info == nullptr) {
    m_info = m_infoBuilder();
  }
  return m_info;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Endpoints::append(const std::list<std::shared_ptr<Endpoint>>& endpoints) {
  list.insert(list.end(), endpoints.begin(), endpoints.end());
}

void Endpoints::append(const Endpoints& endpoints) {
  append(endpoints.list);
}

void Endpoints::append(const std::shared_ptr<Endpoint>& endpoint) {
  list.push_back(endpoint);
}

}}}}
