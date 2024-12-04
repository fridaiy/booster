

#ifndef booster_async_CoroutineWaitList_hpp
#define booster_async_CoroutineWaitList_hpp

#include "booster/async/Coroutine.hpp"

#include <unordered_set>
#include <mutex>

namespace booster { namespace async {

class CoroutineWaitList {
  friend Processor;
public:

  class Listener {
  public:

    virtual ~Listener() = default;

    virtual void onNewItem(CoroutineWaitList& list) = 0;
  };
private:
  std::unordered_set<CoroutineHandle*> m_coroutines;
  std::mutex m_lock;
  Listener* m_listener = nullptr;
private:
  void removeCoroutine(CoroutineHandle* coroutine); //<-- Calls Processor
  void forgetCoroutine(CoroutineHandle* coroutine); //<-- Called From Processor
protected:
  void add(CoroutineHandle* coroutine);

public:

  CoroutineWaitList(const CoroutineWaitList&) = delete;
  CoroutineWaitList& operator=(const CoroutineWaitList&) = delete;

public:

  CoroutineWaitList() = default;
  CoroutineWaitList(CoroutineWaitList&& other);
  virtual ~CoroutineWaitList();
  void setListener(Listener* listener);
  void notifyFirst();
  void notifyAll();

  CoroutineWaitList& operator=(CoroutineWaitList&& other);

};

}}

#endif
