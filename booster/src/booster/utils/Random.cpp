

#include "Random.hpp"

namespace booster { namespace utils {

#ifndef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
  thread_local std::mt19937 Random::RANDOM_GENERATOR(std::random_device{}());
#else
  std::mt19937 Random::RANDOM_GENERATOR (std::random_device{}());
  booster::concurrency::SpinLock Random::RANDOM_LOCK;
#endif

void Random::randomBytes(p_char8 buffer, v_buff_size bufferSize) {

#if defined(BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL)
  std::lock_guard<booster::concurrency::SpinLock> randomLock(RANDOM_LOCK);
#endif

  std::uniform_int_distribution<size_t> distribution(0, 255);

  for(v_buff_size i = 0; i < bufferSize; i ++) {
    buffer[i] = static_cast<v_char8>(distribution(RANDOM_GENERATOR));
  }

}

}}
