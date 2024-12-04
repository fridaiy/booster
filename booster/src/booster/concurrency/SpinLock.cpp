

#include "SpinLock.hpp"

#include <thread>

namespace booster { namespace concurrency {

SpinLock::SpinLock()
  : m_atom(false)
{}
  
void SpinLock::lock() {
  while (std::atomic_exchange_explicit(&m_atom, true, std::memory_order_acquire)) {
    std::this_thread::yield();
  }
}

void SpinLock::unlock() {
  std::atomic_store_explicit(&m_atom, false, std::memory_order_release);
}

bool SpinLock::try_lock() {
  return !std::atomic_exchange_explicit(&m_atom, true, std::memory_order_acquire);
}
  
}}
