

#ifndef booster_async_Executor_hpp
#define booster_async_Executor_hpp

#include "./Processor.hpp"
#include "booster/async/worker/Worker.hpp"
#include "booster/base/Compiler.hpp"

#include "booster/concurrency/SpinLock.hpp"

#include <tuple>
#include <mutex>
#include <condition_variable>

namespace booster { namespace async {

class Executor {
private:
  
  class SubmissionProcessor : public worker::Worker {
  private:
    booster::async::Processor m_processor;
  private:
    std::atomic<bool> m_isRunning;
  private:
    std::thread m_thread;
  public:
    SubmissionProcessor();
  public:

    template<typename CoroutineType, typename ... Args>
    void execute(Args... params) {
      m_processor.execute<CoroutineType, Args...>(params...);
    }

    booster::async::Processor& getProcessor();

    void pushTasks(utils::FastQueue<CoroutineHandle>& tasks) override GPP_ATTRIBUTE(noreturn);

    void pushOneTask(CoroutineHandle* task) override GPP_ATTRIBUTE(noreturn);

    void run();

    void stop() override;

    void join() override;

    void detach() override;
    
  };

public:
  static constexpr const v_int32 VALUE_SUGGESTED = -1000;
public:
  static constexpr const v_int32 IO_WORKER_TYPE_NAIVE = 0;
  static constexpr const v_int32 IO_WORKER_TYPE_EVENT = 1;
private:
  std::atomic<v_uint32> m_balancer;
private:
  std::vector<std::shared_ptr<SubmissionProcessor>> m_processorWorkers;
  std::vector<std::shared_ptr<worker::Worker>> m_allWorkers;
private:
  static v_int32 chooseProcessorWorkersCount(v_int32 processorWorkersCount);
  static v_int32 chooseIOWorkersCount(v_int32 processorWorkersCount, v_int32 ioWorkersCount);
  static v_int32 chooseTimerWorkersCount(v_int32 timerWorkersCount);
  static v_int32 chooseIOWorkerType(v_int32 ioWorkerType);
  void linkWorkers(const std::vector<std::shared_ptr<worker::Worker>>& workers);
public:
  Executor(v_int32 processorWorkersCount = VALUE_SUGGESTED,
           v_int32 ioWorkersCount = VALUE_SUGGESTED,
           v_int32 timerWorkersCount = VALUE_SUGGESTED,
           v_int32 ioWorkerType = VALUE_SUGGESTED);


  ~Executor() = default;
  void join();
  void detach();
  void stop();
  template<typename CoroutineType, typename ... Args>
  void execute(Args... params) {
    auto& processor = m_processorWorkers[(++ m_balancer) % m_processorWorkers.size()];
    processor->execute<CoroutineType, Args...>(params...);
  }

  v_int32 getTasksCount();
  void waitTasksFinished(const std::chrono::duration<v_int64, std::micro>& timeout = std::chrono::minutes(1));
};
  
}}

#endif
