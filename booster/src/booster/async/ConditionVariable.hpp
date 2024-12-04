#ifndef booster_async_ConditionVariable_hpp
#define booster_async_ConditionVariable_hpp

#include "./Lock.hpp"

#include <functional>

namespace booster { namespace async {

class ConditionVariable : private CoroutineWaitList::Listener {
public:

  class Condition {
  public:
    virtual bool check() = 0;
  };

public:

  template<typename F>
  class ConditionTemplate : public Condition {
  private:
    F lambda;
  public:
    ConditionTemplate(const F& f) : lambda(f) {}
    bool check() override {
      return lambda();
    }
  };

private:
  std::atomic_bool m_notified;
  CoroutineWaitList m_list;
private:
  void onNewItem(CoroutineWaitList& list) override;
public:

  ConditionVariable();

  CoroutineStarter wait(LockGuard& lockGuard, std::function<bool()> condition);
  CoroutineStarter waitUntil(LockGuard& lockGuard, std::function<bool()> condition, const std::chrono::system_clock::time_point& timeoutTime);
  CoroutineStarter waitFor(LockGuard& lockGuard, std::function<bool()>, const std::chrono::duration<v_int64, std::micro>& timeout);

  void notifyFirst();
  void notifyAll();

};

}}

#endif
