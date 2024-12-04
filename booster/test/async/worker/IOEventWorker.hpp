#ifndef booster_async_worker_IOEventWorker_hpp
#define booster_async_worker_IOEventWorker_hpp

#include "./Worker.hpp"
#include "booster/concurrency/SpinLock.hpp"

#include <thread>
#include <mutex>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(BOOSTER_IO_EVENT_INTERFACE)

    #define BOOSTER_IO_EVENT_INTERFACE "epoll"
    #define BOOSTER_IO_EVENT_INTERFACE_EPOLL

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace booster { namespace async { namespace worker {

class IOEventWorkerForeman;


class IOEventWorker : public Worker {
private:
  static constexpr const v_int32 MAX_EVENTS = 10000;
private:
  IOEventWorkerForeman* m_foreman;
  Action::IOEventType m_specialization;
  std::atomic<bool> m_running;
  utils::FastQueue<CoroutineHandle> m_backlog;
  booster::concurrency::SpinLock m_backlogLock;
private:
  booster::v_io_handle m_eventQueueHandle;
  booster::v_io_handle m_wakeupTrigger;
  std::unique_ptr<v_char8[]> m_inEvents;
  v_int32 m_inEventsCount;
  v_int32 m_inEventsCapacity;
  std::unique_ptr<v_char8[]> m_outEvents;
private:
  std::thread m_thread;
private:
  void consumeBacklog();
  void waitEvents();
private:
  void initEventQueue();
  void triggerWakeup();
  void setTriggerEvent(p_char8 eventPtr);
  void setCoroutineEvent(CoroutineHandle* coroutine, int operation, p_char8 eventPtr);
public:
  IOEventWorker(IOEventWorkerForeman* foreman, Action::IOEventType specialization);
  ~IOEventWorker() override;
  void pushTasks(utils::FastQueue<CoroutineHandle>& tasks) override;
  void pushOneTask(CoroutineHandle* task) override;


  void run();
  void stop() override;
  void join() override;
  void detach() override;
};


class IOEventWorkerForeman : public Worker {
private:
  IOEventWorker m_reader;
  IOEventWorker m_writer;
public:
  IOEventWorkerForeman();
  ~IOEventWorkerForeman() override;
  void pushTasks(utils::FastQueue<CoroutineHandle>& tasks) override;
  void pushOneTask(CoroutineHandle* task) override;
  void stop() override;
  void join() override;
  void detach() override;
};

}}}

#endif
