

#ifndef booster_network_monitor_ConnectionMonitor_hpp
#define booster_network_monitor_ConnectionMonitor_hpp

#include "MetricsChecker.hpp"

#include "booster/network/ConnectionProvider.hpp"
#include "booster/data/stream/Stream.hpp"

#include <unordered_set>
#include <condition_variable>

namespace booster { namespace network { namespace monitor {


class ConnectionMonitor : public ClientConnectionProvider, public ServerConnectionProvider {
private:

  class ConnectionInvalidator : public provider::Invalidator<data::stream::IOStream> {
  public:

    void invalidate(const std::shared_ptr<data::stream::IOStream>& connection) override;

  };

private:

  class Monitor; // FWD

  class ConnectionProxy : public data::stream::IOStream {
    friend Monitor;
  private:
    std::shared_ptr<Monitor> m_monitor;
    provider::ResourceHandle<data::stream::IOStream> m_connectionHandle;
    std::mutex m_statsMutex;
    ConnectionStats m_stats;
  public:

    ConnectionProxy(const std::shared_ptr<Monitor>& monitor,
                    const provider::ResourceHandle<data::stream::IOStream>& connectionHandle);

    ~ConnectionProxy() override;

    v_io_size read(void *buffer, v_buff_size count, async::Action& action) override;
    v_io_size write(const void *data, v_buff_size count, async::Action& action) override;

    void setInputStreamIOMode(data::stream::IOMode ioMode) override;
    data::stream::IOMode getInputStreamIOMode() override;
    data::stream::Context& getInputStreamContext() override;

    void setOutputStreamIOMode(data::stream::IOMode ioMode) override;
    data::stream::IOMode getOutputStreamIOMode() override;
    data::stream::Context& getOutputStreamContext() override;

    void invalidate();

  };

private:

  class Monitor : public booster::base::Countable {
  private:

    std::mutex m_runMutex;
    std::condition_variable m_runCondition;
    std::atomic<bool> m_running {true};
    bool m_stopped {false};

    std::mutex m_connectionsMutex;
    std::unordered_set<v_uint64> m_connections;

    std::mutex m_checkMutex;
    std::vector<std::shared_ptr<MetricsChecker>> m_metricsCheckers;
    std::unordered_map<booster::String, std::shared_ptr<StatCollector>> m_statCollectors;

  private:
    static void monitorTask(std::shared_ptr<Monitor> monitor);
  private:
    static void* createOrGetMetricData(ConnectionStats& stats, const std::shared_ptr<StatCollector>& collector);
  public:

    static std::shared_ptr<Monitor> createShared();

    void addConnection(ConnectionProxy* connection);
    void freeConnectionStats(ConnectionStats& stats);
    void removeConnection(v_uint64 id);

    void invalidateAll();

    void addStatCollector(const std::shared_ptr<StatCollector>& collector);
    void removeStatCollector(const booster::String& metricName);

    void addMetricsChecker(const std::shared_ptr<MetricsChecker>& checker);

    void onConnectionRead(ConnectionStats& stats, v_io_size readResult);
    void onConnectionWrite(ConnectionStats& stats, v_io_size writeResult);

    void stop();

  };

private:
  std::shared_ptr<ConnectionInvalidator> m_invalidator;
  std::shared_ptr<Monitor> m_monitor;
  std::shared_ptr<ConnectionProvider> m_connectionProvider;
public:


  ConnectionMonitor(const std::shared_ptr<ConnectionProvider>& connectionProvider);

  provider::ResourceHandle<data::stream::IOStream> get() override;

  async::CoroutineStarterForResult<const provider::ResourceHandle<data::stream::IOStream>&> getAsync() override;

  void addStatCollector(const std::shared_ptr<StatCollector>& collector);


  void addMetricsChecker(const std::shared_ptr<MetricsChecker>& checker);


  void invalidateAll();

  void stop() override;

};

}}}

#endif //booster_network_monitor_ConnectionMonitor_hpp
