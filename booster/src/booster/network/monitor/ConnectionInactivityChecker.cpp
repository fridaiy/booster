

#include "ConnectionInactivityChecker.hpp"

namespace booster { namespace network { namespace monitor {

ConnectionInactivityChecker::ConnectionInactivityChecker(const std::chrono::duration<v_int64, std::micro>& lastReadTimeout,
                                                         const std::chrono::duration<v_int64, std::micro>& lastWriteTimeout)
  : m_lastReadTimeout(lastReadTimeout)
  , m_lastWriteTimeout(lastWriteTimeout)
{}

std::vector<booster::String> ConnectionInactivityChecker::getMetricsList() {
  return {};
}

std::shared_ptr<StatCollector> ConnectionInactivityChecker::createStatCollector(const booster::String& metricName) {
  throw std::runtime_error("[booster::network::monitor::ConnectionInactivityChecker::createStatCollector()]: "
                           "Error. ConnectionInactivityChecker doesn't use any stat collectors.");
}

bool ConnectionInactivityChecker::check(const ConnectionStats& stats, v_int64 currMicroTime) {

  bool goodRead;
  if(stats.timestampLastRead == 0) {
    goodRead = currMicroTime - stats.timestampCreated < m_lastReadTimeout.count();
  } else {
    goodRead = currMicroTime - stats.timestampLastRead < m_lastReadTimeout.count();
  }

  bool goodWrite;
  if(stats.timestampLastWrite == 0) {
    goodWrite = currMicroTime - stats.timestampCreated < m_lastWriteTimeout.count();
  } else {
    goodWrite = currMicroTime - stats.timestampLastWrite < m_lastWriteTimeout.count();
  }

  return  goodRead && goodWrite;
}

}}}
