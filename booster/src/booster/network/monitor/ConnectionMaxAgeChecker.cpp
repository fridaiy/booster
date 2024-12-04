

#include "ConnectionMaxAgeChecker.hpp"

namespace booster { namespace network { namespace monitor {

ConnectionMaxAgeChecker::ConnectionMaxAgeChecker(const std::chrono::duration<v_int64, std::micro>& maxAge)
  : m_maxAge(maxAge)
{}

std::vector<booster::String> ConnectionMaxAgeChecker::getMetricsList() {
  return {};
}

std::shared_ptr<StatCollector> ConnectionMaxAgeChecker::createStatCollector(const booster::String& metricName) {
  throw std::runtime_error("[booster::network::monitor::ConnectionMaxAgeChecker::createStatCollector()]: "
                           "Error. ConnectionMaxAgeChecker doesn't use any stat collectors.");
}

bool ConnectionMaxAgeChecker::check(const ConnectionStats& stats, v_int64 currMicroTime) {
  return currMicroTime - stats.timestampCreated < m_maxAge.count();
}

}}}
