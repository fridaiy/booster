#include "IOWorker.hpp"
#include "booster/async/Processor.hpp"

#include <chrono>

namespace booster { namespace async { namespace worker {

IOWorker::IOWorker()
  : Worker(Type::IO)
  , m_running(true)
{
  m_thread = std::thread(&IOWorker::run, this);
}

void IOWorker::pushTasks(utils::FastQueue<CoroutineHandle>& tasks) {
  {
    std::lock_guard<booster::concurrency::SpinLock> guard(m_backlogLock);
    utils::FastQueue<CoroutineHandle>::moveAll(tasks, m_backlog);
  }
  m_backlogCondition.notify_one();
}

void IOWorker::pushOneTask(CoroutineHandle* task) {
  {
    std::lock_guard<booster::concurrency::SpinLock> guard(m_backlogLock);
    m_backlog.pushBack(task);
  }
  m_backlogCondition.notify_one();
}

void IOWorker::consumeBacklog(bool blockToConsume) {

  if(blockToConsume) {

    std::unique_lock<booster::concurrency::SpinLock> lock(m_backlogLock);
    while (m_backlog.first == nullptr && m_running) {
      m_backlogCondition.wait(lock);
    }
    utils::FastQueue<CoroutineHandle>::moveAll(m_backlog, m_queue);
  } else {

    std::unique_lock<booster::concurrency::SpinLock> lock(m_backlogLock, std::try_to_lock);
    if (lock.owns_lock()) {
      utils::FastQueue<CoroutineHandle>::moveAll(m_backlog, m_queue);
    }

  }

}

void IOWorker::run() {

  v_int32 consumeIteration = 0;
  v_int32 roundIteration = 0;

  v_int64 tick =std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  while(m_running) {

    auto CP = m_queue.first;
    if(CP != nullptr) {

      Action action = CP->iterate();
      auto& schA = getCoroutineScheduledAction(CP);

      switch(action.getType()) {

        case Action::TYPE_IO_REPEAT:

          dismissAction(schA);

          ++ roundIteration;
          if(roundIteration == 10) {
            roundIteration = 0;
            m_queue.round();
          }
          break;
        case Action::TYPE_IO_WAIT:
          roundIteration = 0;
          if(schA.getType() == Action::TYPE_WAIT_REPEAT) {
            if(schA.getTimePointMicroseconds() < tick) {
              m_queue.popFront();
              setCoroutineScheduledAction(CP, booster::async::Action::createWaitRepeatAction(0));
              getCoroutineProcessor(CP)->pushOneTask(CP);
            } else {
              m_queue.round();
            }
          } else {
            setCoroutineScheduledAction(CP, booster::async::Action::createWaitRepeatAction(tick + 1000000));
            m_queue.round();
          }
          break;

        default:
          roundIteration = 0;
          m_queue.popFront();
          setCoroutineScheduledAction(CP, std::move(action));
          getCoroutineProcessor(CP)->pushOneTask(CP);
          break;

      }

      ++ consumeIteration;
      if(consumeIteration == 100) {
        consumeIteration = 0;
        consumeBacklog(false);
        std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
        tick = ms.count();
      }

    } else {
      consumeBacklog(true);
      std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
      tick = ms.count();
    }

  }

}

void IOWorker::stop() {
  {
    std::lock_guard<booster::concurrency::SpinLock> lock(m_backlogLock);
    m_running = false;
  }
  m_backlogCondition.notify_one();
}

void IOWorker::join() {
  m_thread.join();
}

void IOWorker::detach() {
  m_thread.detach();
}

}}}
