#ifndef booster_async_Processor_hpp
#define booster_async_Processor_hpp

#include "./Coroutine.hpp"
#include "./CoroutineWaitList.hpp"
#include "booster/async/utils/FastQueue.hpp"
#include "booster/concurrency/SpinLock.hpp"

#include <thread>
#include <condition_variable>
#include <list>
#include <mutex>
#include <set>
#include <vector>

namespace booster { namespace async {


class Processor {
    friend class CoroutineWaitList;
private:

  class TaskSubmission {
  public:
    virtual ~TaskSubmission() = default;
    virtual CoroutineHandle* createCoroutine(Processor* processor) = 0;
  };

  template<int ...> struct IndexSequence {};
  template<int N, int ...S> struct SequenceGenerator : SequenceGenerator <N - 1, N - 1, S...> {};
  template<int ...S>
  struct SequenceGenerator<0, S...> {
    typedef IndexSequence<S...> type;
  };

  template<typename CoroutineType, typename ... Args>
  class SubmissionTemplate : public TaskSubmission {
  private:
    std::tuple<Args...> m_params;
  public:

    SubmissionTemplate(Args... params)
      : m_params(std::make_tuple(params...))
    {}

    virtual CoroutineHandle* createCoroutine(Processor* processor) override {
      return creator(processor, typename SequenceGenerator<sizeof...(Args)>::type());
    }

    template<int ...S>
    CoroutineHandle* creator(Processor* processor, IndexSequence<S...>) {
      return new CoroutineHandle(processor, new CoroutineType(std::get<S>(m_params) ...));
    }

  };

private:

  std::vector<std::shared_ptr<worker::Worker>> m_ioWorkers;
  std::vector<std::shared_ptr<worker::Worker>> m_timerWorkers;

  std::vector<utils::FastQueue<CoroutineHandle>> m_ioPopQueues;
  std::vector<utils::FastQueue<CoroutineHandle>> m_timerPopQueues;

  v_uint32 m_ioBalancer = 0;
  v_uint32 m_timerBalancer = 0;

private:

  std::unordered_set<CoroutineHandle*> m_sleepNoTimeSet;
  std::unordered_set<CoroutineHandle*> m_sleepTimeSet;
  std::mutex m_sleepMutex;
  std::condition_variable m_sleepCV;

  std::thread m_sleepSetTask{&Processor::checkCoroutinesSleep, this};

private:

  booster::concurrency::SpinLock m_taskLock;
  std::condition_variable_any m_taskCondition;
  std::list<std::shared_ptr<TaskSubmission>> m_taskList;
  utils::FastQueue<CoroutineHandle> m_pushList;

private:

  utils::FastQueue<CoroutineHandle> m_queue;

private:
  std::atomic_bool m_running{true};
  std::atomic<v_int32> m_tasksCounter{0};
private:

  void popIOTask(CoroutineHandle* coroutine);
  void popTimerTask(CoroutineHandle* coroutine);

  void consumeAllTasks();
  void addCoroutine(CoroutineHandle* coroutine);
  void popTasks();
  void pushQueues();

  void putCoroutineToSleep(CoroutineHandle* ch);
  void wakeCoroutine(CoroutineHandle* ch);
  void checkCoroutinesSleep();

public:

  Processor() = default;
  void addWorker(const std::shared_ptr<worker::Worker>& worker);
  void pushOneTask(CoroutineHandle* coroutine);
  void pushTasks(utils::FastQueue<CoroutineHandle>& tasks);
  template<typename CoroutineType, typename ... Args>
  void execute(Args... params) {
    auto submission = std::make_shared<SubmissionTemplate<CoroutineType, Args...>>(params...);
    ++ m_tasksCounter;
    {
      std::lock_guard<booster::concurrency::SpinLock> lock(m_taskLock);
      m_taskList.push_back(submission);
    }
    m_taskCondition.notify_one();
  }

  void waitForTasks();
  bool iterate(v_int32 numIterations);
  void stop();
  v_int32 getTasksCount();
};
  
}}

#endif
