

#include "CommunicationUtils.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace utils {
  
void CommunicationUtils::considerConnectionState(const std::shared_ptr<protocol::http::incoming::Request>& request,
                                                 const std::shared_ptr<protocol::http::outgoing::Response>& response,
                                                 ConnectionState& connectionState)
{

  if(connectionState != ConnectionState::ALIVE) {
    return;
  }

  auto outState = response->getHeaders().getAsMemoryLabel<booster::data::share::StringKeyLabelCI>(Header::CONNECTION);
  if(outState && outState == Header::Value::CONNECTION_UPGRADE) {
    connectionState = ConnectionState::DELEGATED;
    return;
  }
  
  if(request) {

    auto connection = request->getHeaders().getAsMemoryLabel<booster::data::share::StringKeyLabelCI>(Header::CONNECTION);
    if(connection) {
      if(connection != Header::Value::CONNECTION_KEEP_ALIVE) {
        connectionState = ConnectionState::CLOSING;
      }
      return;
    }
    



    auto& protocol = request->getStartingLine().protocol;
    if(protocol && booster::utils::String::compareCI_ASCII(protocol.getData(), protocol.getSize(), "HTTP/1.1", 8) == 0) {
      if(outState && outState != Header::Value::CONNECTION_KEEP_ALIVE) {
        connectionState = ConnectionState::CLOSING;
      }
      return;
    }
  }
  



  if(!outState || outState != Header::Value::CONNECTION_KEEP_ALIVE) {
    connectionState = ConnectionState::CLOSING;
  }

  return;
  
}

std::shared_ptr<encoding::EncoderProvider>
CommunicationUtils::selectEncoder(const std::shared_ptr<http::incoming::Request>& request,
                                  const std::shared_ptr<http::encoding::ProviderCollection>& providers)
{
  if(providers && request) {

    auto suggested = request->getHeaders().getAsMemoryLabel<booster::data::share::StringKeyLabel>(Header::ACCEPT_ENCODING);

    if(suggested) {

      http::HeaderValueData valueData;
      http::Parser::parseHeaderValueData(valueData, suggested, ',');

      return providers->get(valueData.tokens);

    }

  }

  return nullptr;

}
  
}}}}}

