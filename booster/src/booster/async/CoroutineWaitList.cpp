

#include "CoroutineWaitList.hpp"

#include "./Processor.hpp"
#include <algorithm>
#include <set>

namespace booster { namespace async {


CoroutineWaitList::CoroutineWaitList(CoroutineWaitList&& other) {
  {
    std::lock_guard<std::mutex> lock{other.m_lock};
    m_coroutines = std::move(other.m_coroutines);
  }
}

CoroutineWaitList::~CoroutineWaitList() {
  notifyAll();
}

void CoroutineWaitList::setListener(Listener* listener) {
  m_listener = listener;
}

void CoroutineWaitList::add(CoroutineHandle* coroutine) {
  {
    std::lock_guard<std::mutex> lock(m_lock);
    m_coroutines.insert(coroutine);
  }
  if(m_listener != nullptr) {
    m_listener->onNewItem(*this);
  }
}

void CoroutineWaitList::notifyFirst() {
  std::lock_guard<std::mutex> lock(m_lock);
  if(!m_coroutines.empty()) {
    removeCoroutine(*m_coroutines.begin());
  }
}

void CoroutineWaitList::notifyAll() {
  std::lock_guard<std::mutex> lock(m_lock);
  while (!m_coroutines.empty()) {
    removeCoroutine(*m_coroutines.begin());
  }
}

void CoroutineWaitList::removeCoroutine(CoroutineHandle* coroutine) {
  m_coroutines.erase(coroutine);
  coroutine->_PP->wakeCoroutine(coroutine);
}

void CoroutineWaitList::forgetCoroutine(CoroutineHandle *coroutine) {
  std::lock_guard<std::mutex> lock(m_lock);
  m_coroutines.erase(coroutine);
}

CoroutineWaitList& CoroutineWaitList::operator=(CoroutineWaitList&& other) {

  if (this == std::addressof(other)) return *this;
      
  notifyAll();

  {
    std::lock_guard<std::mutex> otherLock(other.m_lock);
    std::lock_guard<std::mutex> myLock(m_lock);
    m_coroutines = std::move(other.m_coroutines);
  }

  return *this;

}

}}
