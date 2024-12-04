

#ifndef booster_network_monitor_MetricsChecker_hpp
#define booster_network_monitor_MetricsChecker_hpp

#include "StatCollector.hpp"

namespace booster { namespace network { namespace monitor {


class MetricsChecker : public booster::base::Countable  {
public:


  virtual ~MetricsChecker() override = default;


  virtual std::vector<booster::String> getMetricsList() = 0;


  virtual std::shared_ptr<StatCollector> createStatCollector(const booster::String& metricName) = 0;


  virtual bool check(const ConnectionStats& stats, v_int64 currMicroTime) = 0;

};

}}}

#endif //booster_network_monitor_MetricsChecker_hpp
