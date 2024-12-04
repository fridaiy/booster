

#ifndef booster_network_monitor_StatCollector_hpp
#define booster_network_monitor_StatCollector_hpp

#include "booster/Types.hpp"
#include "booster/IODefinitions.hpp"

namespace booster { namespace network { namespace monitor {


struct ConnectionStats {


  v_int64 timestampCreated = 0;


  v_io_size totalRead = 0;


  v_io_size totalWrite = 0;


  v_int64 timestampLastRead = 0;


  v_int64 timestampLastWrite = 0;


  v_io_size lastReadSize = 0;


  v_io_size lastWriteSize = 0;


  std::unordered_map<booster::String, void*> metricsData;

};


class StatCollector : public booster::base::Countable {
public:


  virtual ~StatCollector() override = default;


  virtual booster::String metricName() = 0;


  virtual void* createMetricData() = 0;


  virtual void deleteMetricData(void* metricData) = 0;


  virtual void onRead(void* metricData, v_io_size readResult, v_int64 timestamp) = 0;


  virtual void onWrite(void* metricData, v_io_size writeResult, v_int64 timestamp) = 0;
};

}}}

#endif //booster_network_monitor_StatCollector_hpp
