

#include "ConnectionProviderSwitch.hpp"
#include "booster/base/Log.hpp"

namespace booster { namespace network {

ConnectionProviderSwitch::ConnectionProviderSwitch(const std::shared_ptr<ConnectionProvider>& provider)
  : m_provider(provider)
{}

void ConnectionProviderSwitch::resetProvider(const std::shared_ptr<ConnectionProvider>& provider) {
  std::lock_guard<std::mutex> lock(m_mutex);
  m_provider = provider;
  m_properties = provider->getProperties();
}

std::shared_ptr<ConnectionProvider> ConnectionProviderSwitch::getCurrentProvider() {

  std::shared_ptr<ConnectionProvider> provider;

  {
    std::lock_guard<std::mutex> lock(m_mutex);
    provider = m_provider;
  }

  if(!provider) {
    const char* const TAG = "[booster::network::ConnectionProviderSwitch::getCurrentProvider()]";
    const char* const msg = "Error. Can't provide connection. There is no provider set.";
    BOOSTER_LOGe(TAG, msg)
    throw std::runtime_error(std::string(TAG) + ": " + msg);
  }

  return provider;

}

provider::ResourceHandle<data::stream::IOStream> ConnectionProviderSwitch::get() {
  return getCurrentProvider()->get();
}


booster::async::CoroutineStarterForResult<const provider::ResourceHandle<data::stream::IOStream>&> ConnectionProviderSwitch::getAsync() {
  return getCurrentProvider()->getAsync();
}


void ConnectionProviderSwitch::stop() {
  return getCurrentProvider()->stop();
}

}}
