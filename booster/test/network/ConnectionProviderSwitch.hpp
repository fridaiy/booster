

#ifndef booster_network_ConnectionProviderSwitch_hpp
#define booster_network_ConnectionProviderSwitch_hpp

#include "ConnectionProvider.hpp"
#include <mutex>

namespace booster { namespace network {


class ConnectionProviderSwitch : public ServerConnectionProvider, public ClientConnectionProvider {
private:
  std::shared_ptr<ConnectionProvider> getCurrentProvider();
private:
  std::shared_ptr<ConnectionProvider> m_provider;
  std::mutex m_mutex;
public:


  ConnectionProviderSwitch() = default;


  ConnectionProviderSwitch(const std::shared_ptr<ConnectionProvider>& provider);


  void resetProvider(const std::shared_ptr<ConnectionProvider>& provider);


  provider::ResourceHandle<data::stream::IOStream> get() override;


  booster::async::CoroutineStarterForResult<const provider::ResourceHandle<data::stream::IOStream>&> getAsync() override;


  void stop() override;

};

}}

#endif //booster_network_ConnectionProviderSwitch_hpp
