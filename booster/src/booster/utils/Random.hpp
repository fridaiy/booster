

#ifndef booster_utils_Random_hpp
#define booster_utils_Random_hpp

#include "booster/concurrency/SpinLock.hpp"
#include "booster/Types.hpp"
#include <random>

namespace booster { namespace utils {


class Random {
private:
#ifndef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
  static thread_local std::mt19937 RANDOM_GENERATOR;
#else
  static std::mt19937 RANDOM_GENERATOR;
  static booster::concurrency::SpinLock RANDOM_LOCK;
#endif
public:


  static void randomBytes(p_char8 buffer, v_buff_size bufferSize);

};

}}

#endif // booster_utils_Random_hpp
