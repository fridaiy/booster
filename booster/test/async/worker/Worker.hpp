#ifndef booster_async_worker_Worker_hpp
#define booster_async_worker_Worker_hpp

#include "booster/async/Coroutine.hpp"
#include <thread>

namespace booster { namespace async { namespace worker {


class Worker {
public:
  enum Type : v_int32 {
    PROCESSOR = 0,
    TIMER = 1,
    IO = 2,
    TYPES_COUNT = 3
  };

private:
  Type m_type;
protected:
  static void setCoroutineScheduledAction(CoroutineHandle* coroutine, Action&& action);
  static Action& getCoroutineScheduledAction(CoroutineHandle* coroutine);
  static Processor* getCoroutineProcessor(CoroutineHandle* coroutine);
  static void dismissAction(Action& action);
  static CoroutineHandle* nextCoroutine(CoroutineHandle* coroutine);
public:
  Worker(Type type);
  virtual ~Worker() = default;
  virtual void pushTasks(utils::FastQueue<CoroutineHandle>& tasks) = 0;
  virtual void pushOneTask(CoroutineHandle* task) = 0;
  virtual void stop() = 0;
  virtual void join() = 0;
  virtual void detach() = 0;
  Type getType();
};

}}}

#endif
