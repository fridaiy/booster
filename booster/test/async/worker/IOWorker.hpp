#ifndef booster_async_worker_IOWorker_hpp
#define booster_async_worker_IOWorker_hpp

#include "./Worker.hpp"
#include "booster/concurrency/SpinLock.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>

namespace booster { namespace async { namespace worker {


class IOWorker : public Worker {
private:
  bool m_running;
  utils::FastQueue<CoroutineHandle> m_backlog;
  utils::FastQueue<CoroutineHandle> m_queue;
  booster::concurrency::SpinLock m_backlogLock;
  std::condition_variable_any m_backlogCondition;
private:
  std::thread m_thread;
private:
  void consumeBacklog(bool blockToConsume);
public:
  IOWorker();
  void pushTasks(utils::FastQueue<CoroutineHandle>& tasks) override;
  void pushOneTask(CoroutineHandle* task) override;
  void run();
  void stop() override;
  void join() override;
  void detach() override;
};

}}}

#endif //booster_async_worker_IOWorker_hpp
