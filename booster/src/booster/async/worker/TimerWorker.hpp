#ifndef booster_async_worker_TimerWorker_hpp
#define booster_async_worker_TimerWorker_hpp

#include "./Worker.hpp"
#include "booster/concurrency/SpinLock.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>

namespace booster { namespace async { namespace worker {


class TimerWorker : public Worker {
private:
  std::atomic<bool> m_running;
  utils::FastQueue<CoroutineHandle> m_backlog;
  utils::FastQueue<CoroutineHandle> m_queue;
  booster::concurrency::SpinLock m_backlogLock;
  std::condition_variable_any m_backlogCondition;
private:
  std::chrono::duration<v_int64, std::micro> m_granularity;
private:
  std::thread m_thread;
private:
  void consumeBacklog();
public:
  TimerWorker(const std::chrono::duration<v_int64, std::micro>& granularity = std::chrono::milliseconds(100));
  void pushTasks(utils::FastQueue<CoroutineHandle>& tasks) override;
  void pushOneTask(CoroutineHandle* task) override;

  void run();
  void stop() override;
  void join() override;
  void detach() override;

};

}}}

#endif //booster_async_worker_TimerWorker_hpp
