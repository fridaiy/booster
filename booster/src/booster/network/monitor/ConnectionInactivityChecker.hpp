

#ifndef booster_network_monitor_ConnectionInactivityChecker_hpp
#define booster_network_monitor_ConnectionInactivityChecker_hpp

#include "MetricsChecker.hpp"
#include "booster/base/Compiler.hpp"

namespace booster { namespace network { namespace monitor {


class ConnectionInactivityChecker : public MetricsChecker {
private:
  std::chrono::duration<v_int64, std::micro> m_lastReadTimeout;
  std::chrono::duration<v_int64, std::micro> m_lastWriteTimeout;
public:


  ConnectionInactivityChecker(const std::chrono::duration<v_int64, std::micro>& lastReadTimeout,
                              const std::chrono::duration<v_int64, std::micro>& lastWriteTimeout);

  std::vector<booster::String> getMetricsList() override;

  std::shared_ptr<StatCollector> createStatCollector(const booster::String& metricName) override GPP_ATTRIBUTE(noreturn);

  bool check(const ConnectionStats& stats, v_int64 currMicroTime) override;

};

}}}

#endif //booster_network_monitor_ConnectionInactivityChecker_hpp
