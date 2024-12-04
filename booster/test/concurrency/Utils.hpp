

#ifndef booster_concurrency_Utils_hpp
#define booster_concurrency_Utils_hpp

#include "booster/Environment.hpp"
#include <thread>

namespace booster { namespace concurrency {

class Utils {
private:
  static v_int32 calcHardwareConcurrency();
public:

  
  static v_int32 setThreadAffinityToOneCpu(std::thread::native_handle_type nativeHandle, v_int32 cpuIndex);

  
  static v_int32 setThreadAffinityToCpuRange(std::thread::native_handle_type nativeHandle, v_int32 firstCpuIndex, v_int32 lastCpuIndex);

  
  static v_int32 getHardwareConcurrency();

};

}}

#endif //booster_concurrency_Utils_hpp
