

#ifndef booster_concurrency_SpinLock_hpp
#define booster_concurrency_SpinLock_hpp

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS
#include <atomic>

namespace booster { namespace concurrency {


class SpinLock {
protected:
  std::atomic<bool> m_atom;
public:

  
  SpinLock();

  
  void lock();

  
  void unlock();

  
  bool try_lock();

};
  
}}

#endif 
