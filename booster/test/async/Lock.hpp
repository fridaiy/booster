

#ifndef booster_async_Mutex_hpp
#define booster_async_Mutex_hpp

#include "./CoroutineWaitList.hpp"

namespace booster { namespace async {

class Lock : private CoroutineWaitList::Listener {
private:
  std::atomic<v_int32> m_counter;
  std::mutex m_mutex;
  CoroutineWaitList m_list;
private:
  void onNewItem(CoroutineWaitList& list) override;
public:
  Lock();
  Action waitAsync();
  void lock();
  void unlock();
  bool try_lock();
};

class LockGuard {
public:
  typedef booster::async::CoroutineStarter CoroutineStarter;
private:
  bool m_ownsLock;
  Lock* m_lock;
public:
  LockGuard(const LockGuard&) = delete;
  LockGuard& operator = (const LockGuard&) = delete;
public:
  LockGuard();
  LockGuard(Lock* lock);
  ~LockGuard();
  void setLockObject(Lock* lock);
  CoroutineStarter lockAsync();
  CoroutineStarter lockAsync(Lock* lock);
  Action lockAsyncInline(booster::async::Action&& nextAction);
  bool owns_lock() const;
  void unlock();

};

CoroutineStarter synchronize(booster::async::Lock *lock, CoroutineStarter&& starter);

}}

#endif // booster_async_Mutex_hpp
