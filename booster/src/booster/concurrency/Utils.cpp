

#include "Utils.hpp"
#include "booster/base/Log.hpp"

namespace booster { namespace concurrency {

v_int32 Utils::setThreadAffinityToOneCpu(std::thread::native_handle_type nativeHandle, v_int32 cpuIndex) {
  return setThreadAffinityToCpuRange(nativeHandle, cpuIndex, cpuIndex);
}

v_int32 Utils::setThreadAffinityToCpuRange(std::thread::native_handle_type nativeHandle, v_int32 firstCpuIndex, v_int32 lastCpuIndex) {
#if defined(_GNU_SOURCE)

  // NOTE:

  // The below line doesn't compile on Android.
  //result = pthread_setaffinity_np(nativeHandle, sizeof(cpu_set_t), &cpuset);

  // The below line compiles on Android but has not been tested.
  //result = sched_setaffinity(nativeHandle, sizeof(cpu_set_t), &cpuset);

  #if !defined(__ANDROID__) && !defined(BOOSTER_COMPAT_BUILD_NO_SET_AFFINITY)

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    for(v_int32 i = firstCpuIndex; i <= lastCpuIndex; i++) {
      CPU_SET(static_cast<size_t>(i), &cpuset);
    }

    v_int32 result = pthread_setaffinity_np(nativeHandle, sizeof(cpu_set_t), &cpuset);

    if (result != 0) {
      BOOSTER_LOGd("[booster::concurrency::Thread::assignThreadToCpu(...)]", "error code - {}", result)
    }

    return result;

  #else
    return -1;
  #endif

#else
  (void)nativeHandle;
  (void)firstCpuIndex;
  (void)lastCpuIndex;
  return -1;
#endif
}

v_int32 Utils::calcHardwareConcurrency() {
#if !defined(BOOSTER_THREAD_HARDWARE_CONCURRENCY)
  v_int32 concurrency = static_cast<v_int32>(std::thread::hardware_concurrency());
  if(concurrency == 0) {
    BOOSTER_LOGd("[booster::concurrency:Thread::calcHardwareConcurrency()]", "Warning - failed to get hardware_concurrency. Setting hardware_concurrency=1")
    concurrency = 1;
  }
  return concurrency;
#else
  return BOOSTER_THREAD_HARDWARE_CONCURRENCY;
#endif
}

v_int32 Utils::getHardwareConcurrency() {
  static v_int32 concurrency = calcHardwareConcurrency();
  return concurrency;
}

}}
