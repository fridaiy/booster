#include "Lock.hpp"

namespace booster { namespace async {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Lock

Lock::Lock()
  : m_counter(0)
{
  m_list.setListener(this);
}

void Lock::onNewItem(CoroutineWaitList& list) {
  auto counter = m_counter.load();
  if(counter == 0) {
    list.notifyFirst();
  } else if(counter < 0) {
    throw std::runtime_error("[booster::async::Lock::onNewItem()]: Error. Invalid state.");
  }
}

Action Lock::waitAsync() {
  auto counter = m_counter.load();
  if(counter > 0) {
    return Action::createWaitListAction(&m_list);
  } else if(counter == 0) {
    return Action::createActionByType(Action::TYPE_REPEAT);
  }
  throw std::runtime_error("[booster::async::Lock::waitAsync()]: Error. Invalid state.");
}

void Lock::lock() {
  m_mutex.lock();
  ++ m_counter;
}

void Lock::unlock() {
  m_mutex.unlock();
  -- m_counter;
  if(m_counter < 0) {
    throw std::runtime_error("[booster::async::Lock::unlock()]: Error. Invalid state.");
  }
  m_list.notifyFirst();
}

bool Lock::try_lock() {
  bool result = m_mutex.try_lock();
  if(result) {
    ++ m_counter;
  }
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LockGuard

LockGuard::LockGuard()
  : m_ownsLock(false)
  , m_lock(nullptr)
{}

LockGuard::LockGuard(Lock* lock)
  : m_ownsLock(false)
  , m_lock(lock)
{}

LockGuard::~LockGuard() {
  if(m_ownsLock) {
    m_lock->unlock();
  }
}

void LockGuard::setLockObject(Lock* lock) {
  if(m_lock == nullptr) {
    m_lock = lock;
  } else if(m_lock != lock) {
    throw std::runtime_error("[booster::async::LockGuard::setLockObject()]: Error. Invalid state. LockGuard is NOT reusable.");
  }
}

CoroutineStarter LockGuard::lockAsync() {

  class LockCoroutine : public Coroutine<LockCoroutine> {
  private:
    LockGuard* m_guard;
  public:

    LockCoroutine(LockGuard* guard)
      : m_guard(guard)
    {}

    Action act() override {
      return m_guard->lockAsyncInline(finish());
    }

  };

  return LockCoroutine::start(this);

}

CoroutineStarter LockGuard::lockAsync(Lock* lock) {
  setLockObject(lock);
  return lockAsync();
}


Action LockGuard::lockAsyncInline(booster::async::Action&& nextAction) {

  if(!m_ownsLock) {

    m_ownsLock = m_lock->try_lock();

    if(m_ownsLock) {
      return std::forward<Action>(nextAction);
    } else {
      return m_lock->waitAsync();
    }

  } else {
    throw std::runtime_error("[booster::async::LockGuard::lockAsyncInline()]: Error. Invalid state. Double lock attempt.");
  }

}

bool LockGuard::owns_lock() const {
  return m_ownsLock;
}

void LockGuard::unlock() {

  if(m_lock) {

    if(m_ownsLock) {

      m_lock->unlock();
      m_ownsLock = false;

    } else {
      throw std::runtime_error("[booster::async::LockGuard::unlock()]: Error. Invalid state. LockGuard is NOT owning the lock.");
    }

  } else {
    throw std::runtime_error("[booster::async::LockGuard::unlock()]: Error. Invalid state. Lock object is nullptr.");
  }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Misc

CoroutineStarter synchronize(booster::async::Lock *lock, CoroutineStarter&& starter) {

  class Synchronized : public booster::async::Coroutine<Synchronized> {
  private:
    booster::async::LockGuard m_lockGuard;
    CoroutineStarter m_starter;
  public:

    Synchronized(booster::async::Lock *lock, CoroutineStarter&& starter)
      : m_lockGuard(lock)
      , m_starter(std::forward<CoroutineStarter>(starter))
    {}

    Action act() override {
      return m_lockGuard.lockAsync().next(std::move(m_starter)).next(finish());
    }

  };

  return new Synchronized(lock, std::forward<CoroutineStarter>(starter));

}

}}
