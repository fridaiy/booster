#ifndef booster_async_Coroutine_hpp
#define booster_async_Coroutine_hpp

#include "./Error.hpp"

#include "booster/async/utils/FastQueue.hpp"

#include "booster/IODefinitions.hpp"
#include "booster/Environment.hpp"

#include "booster/Types.hpp"

#include <chrono>
#include <exception>

namespace booster { namespace async {

class CoroutineHandle;
class AbstractCoroutine;
class Processor;
class CoroutineStarter;
class CoroutineWaitList;

namespace worker {
  class Worker;
}


class Action {
  friend Processor;
  friend CoroutineHandle;
  friend AbstractCoroutine;
  friend CoroutineStarter;
  friend worker::Worker;
public:
  typedef Action (AbstractCoroutine::*FunctionPtr)();
public:

  static const std::chrono::system_clock::time_point TIME_ZERO;

  //无效的动作
  static constexpr const v_int32 TYPE_NONE = 0;

  //启动协程
  static constexpr const v_int32 TYPE_COROUTINE = 1;

  //替换协程函数
  static constexpr const v_int32 TYPE_YIELD_TO = 2;

  //重复执行
  static constexpr const v_int32 TYPE_REPEAT = 3;

  //定时任务
  static constexpr const v_int32 TYPE_WAIT_REPEAT = 4;

  //等待IO ok
  static constexpr const v_int32 TYPE_IO_WAIT = 5;

  //重复刚才的io操作
  static constexpr const v_int32 TYPE_IO_REPEAT = 6;

  //当前协程执行完毕 将控制权交给 启动它的协程
  static constexpr const v_int32 TYPE_FINISH = 7;

  //出错
  static constexpr const v_int32 TYPE_ERROR = 8;

  //需要加入等待队列
  static constexpr const v_int32 TYPE_WAIT_LIST = 9;

  //需要加入带有超时时间的等待列表
  static constexpr const v_int32 TYPE_WAIT_LIST_WITH_TIMEOUT = 10;

public:


  enum IOEventType : v_int32 {
    IO_EVENT_READ = 256,
    IO_EVENT_WRITE = 512
  };


  static constexpr const v_int32 CODE_IO_WAIT_READ = TYPE_IO_WAIT | IOEventType::IO_EVENT_READ;

  static constexpr const v_int32 CODE_IO_WAIT_WRITE = TYPE_IO_WAIT | IOEventType::IO_EVENT_WRITE;

  static constexpr const v_int32 CODE_IO_WAIT_RESCHEDULE = TYPE_IO_WAIT | IOEventType::IO_EVENT_READ | IOEventType::IO_EVENT_WRITE;

  static constexpr const v_int32 CODE_IO_REPEAT_READ = TYPE_IO_REPEAT | IOEventType::IO_EVENT_READ;

  static constexpr const v_int32 CODE_IO_REPEAT_WRITE = TYPE_IO_REPEAT | IOEventType::IO_EVENT_WRITE;

  static constexpr const v_int32 CODE_IO_REPEAT_RESCHEDULE = TYPE_IO_REPEAT | IOEventType::IO_EVENT_READ | IOEventType::IO_EVENT_WRITE;
private:

  struct IOData {
    booster::v_io_handle ioHandle;
    IOEventType ioEventType;
  };

  struct WaitListData {
    CoroutineWaitList* waitList;
    v_int64 timePointMicroseconds;
  };

private:
  union Data {
    FunctionPtr fptr;
    AbstractCoroutine* coroutine;
    Error* error;
    IOData ioData;
    v_int64 timePointMicroseconds;
    WaitListData waitListData;
  };
private:
  mutable v_int32 m_type;
  Data m_data;
private:
  void free();
protected:
  Action(v_int32 type);
public:
  Action();
  static Action clone(const Action& action);
  static Action createActionByType(v_int32 type);
  static Action createIOWaitAction(v_io_handle ioHandle, IOEventType ioEventType);
  static Action createIORepeatAction(v_io_handle ioHandle, IOEventType ioEventType);
  static Action createWaitRepeatAction(v_int64 timePointMicroseconds);
  static Action createWaitListAction(CoroutineWaitList* waitList, const std::chrono::system_clock::time_point& timeoutTime = TIME_ZERO);
  Action(AbstractCoroutine* coroutine);
  Action(const FunctionPtr& functionPtr);
  Action(Error* error);
  Action(const Action&) = delete;
  Action(Action&& other);
  ~Action();
  Action& operator=(const Action&) = delete;
  Action& operator=(Action&& other);
  bool isError() const;
  bool isNone() const;
  v_int32 getType() const;
  v_int64 getTimePointMicroseconds() const;
  booster::v_io_handle getIOHandle() const;
  IOEventType getIOEventType() const;
  v_int32 getIOEventCode() const;
};


class CoroutineStarter {
private:
  AbstractCoroutine* m_first;
  AbstractCoroutine* m_last;

private:

  void freeCoroutines();

public:
  CoroutineStarter(AbstractCoroutine* coroutine);
  CoroutineStarter(const CoroutineStarter&) = delete;
  CoroutineStarter(CoroutineStarter&& other);
  ~CoroutineStarter();
  CoroutineStarter& operator=(const CoroutineStarter&) = delete;
  CoroutineStarter& operator=(CoroutineStarter&& other);
  Action next(Action&& action);
  CoroutineStarter& next(CoroutineStarter&& starter);

};


class CoroutineHandle : public booster::base::Countable {
  friend utils::FastQueue<CoroutineHandle>;
  friend Processor;
  friend worker::Worker;
  friend CoroutineWaitList;
public:
  typedef booster::async::Action Action;
  typedef booster::async::Error Error;
  typedef Action (AbstractCoroutine::*FunctionPtr)();
private:
  Processor* _PP;
  AbstractCoroutine* _CP;
  FunctionPtr _FP;
  booster::async::Action _SCH_A;
  CoroutineHandle* _ref;
public:

  CoroutineHandle(Processor* processor, AbstractCoroutine* rootCoroutine);
  ~CoroutineHandle() override;

  Action takeAction(Action&& action);
  Action iterate();
  Action iterateAndTakeAction();

  bool finished() const;

};


class AbstractCoroutine : public booster::base::Countable {
  friend CoroutineStarter;
  friend CoroutineHandle;
public:
  typedef booster::async::Action Action;
  typedef booster::async::Error Error;
  typedef Action (AbstractCoroutine::*FunctionPtr)();
public:

  template<typename ...Args>
  class AbstractMemberCaller {
  public:
    virtual ~AbstractMemberCaller() = default;
    virtual Action call(AbstractCoroutine* coroutine, const Args&... args) = 0;
  };

  template<typename T, typename ...Args>
  class MemberCaller : public AbstractMemberCaller<Args...> {
  public:
    typedef Action (T::*Func)(const Args&...);
  private:
    Func m_func;
  public:

    MemberCaller(Func func)
      : m_func(func)
    {}

    Action call(AbstractCoroutine* coroutine, const Args&... args) override {
      T* _this = static_cast<T*>(coroutine);
      return (_this->*m_func)(args...);
    }

  };

  template<typename T, typename ...Args>
  static std::unique_ptr<AbstractMemberCaller<Args...>> createMemberCaller(Action (T::*func)(Args...)) {
    return std::unique_ptr<AbstractMemberCaller<Args...>>(new MemberCaller<T, Args...>(func));
  }

private:
  AbstractCoroutine* m_parent;
protected:
  Action m_parentReturnAction;
  FunctionPtr m_parentReturnFP;
public:
  AbstractCoroutine();
  virtual ~AbstractCoroutine() override = default;
  virtual Action act() = 0;
  virtual Action call(const FunctionPtr& ptr) = 0;
  virtual Action handleError(Error* error);
  AbstractCoroutine* getParent() const;
  static Action repeat();
  static Action waitRepeat(const std::chrono::duration<v_int64, std::micro>& timeout);
  CoroutineStarter waitFor(const std::chrono::duration<v_int64, std::micro>& timeout);
  static Action ioWait(v_io_handle ioHandle, Action::IOEventType ioEventType);
  static Action ioRepeat(v_io_handle ioHandle, Action::IOEventType ioEventType);
  static Action error(Error* error);
  template<class E, typename ... Args>
  Action error(Args... args) {
    return error(new E(args...));
  }
  
};

template<class T>
class Coroutine : public AbstractCoroutine {
public:
  typedef Action (T::*Function)();
public:

  static void* operator new(std::size_t sz) {
    return ::operator new(sz);
  }

  static void operator delete(void* ptr, std::size_t sz) {
    (void)sz;
    ::operator delete(ptr);
  }

public:
  template<typename ...ConstructorArgs>
  static CoroutineStarter start(ConstructorArgs&&... args) {
    return new T(std::forward<ConstructorArgs>(args)...);
  }

  Action call(const FunctionPtr& ptr) override {
    Function f = static_cast<Function>(ptr);
    return (static_cast<T*>(this)->*f)();
  }
  Action yieldTo(const Function& function) const {
    return Action(static_cast<FunctionPtr>(function));
  }
  Action finish() const {
    return Action::createActionByType(Action::TYPE_FINISH);
  }
  
};

template<typename ...Args>
class AbstractCoroutineWithResult : public AbstractCoroutine {
protected:
  std::unique_ptr<AbstractMemberCaller<Args...>> m_parentMemberCaller;
public:

  class StarterForResult {
  private:
    AbstractCoroutineWithResult* m_coroutine;
  public:
    StarterForResult(AbstractCoroutineWithResult* coroutine)
      : m_coroutine(coroutine)
    {}

    StarterForResult(const StarterForResult&) = delete;

    StarterForResult(StarterForResult&& other)
      : m_coroutine(other.m_coroutine)
    {
      other.m_coroutine = nullptr;
    }

    ~StarterForResult() {
      delete m_coroutine;
    }

    StarterForResult& operator=(const StarterForResult&) = delete;

    StarterForResult& operator=(StarterForResult&& other) {
      if (this == std::addressof(other)) return *this;
        
      delete m_coroutine;
      m_coroutine = other.m_coroutine;
      other.m_coroutine = nullptr;
      return *this;
    }

    template<typename C>
    Action callbackTo(Action (C::*callback)(Args...)) {
      if(m_coroutine == nullptr) {
        throw std::runtime_error("[booster::async::AbstractCoroutineWithResult::StarterForResult::callbackTo()]: Error. Coroutine is null.");
      }
      m_coroutine->m_parentMemberCaller = createMemberCaller(callback);
      Action result = m_coroutine;
      m_coroutine = nullptr;
      return result;

    }

  };

};

template <typename ...Args>
using
CoroutineStarterForResult = typename AbstractCoroutineWithResult<Args...>::StarterForResult;

template<class T, typename ...Args>
class CoroutineWithResult : public AbstractCoroutineWithResult<Args...> {
  friend AbstractCoroutine;
public:
  typedef Action (T::*Function)();
public:

  static void* operator new(std::size_t sz) {
    return ::operator new(sz);
  }

  static void operator delete(void* ptr, std::size_t sz) {
    (void)sz;
    ::operator delete(ptr);
  }
public:


  template<typename ...ConstructorArgs>
  static CoroutineStarterForResult<Args...> startForResult(ConstructorArgs... args) {
    return new T(args...);
  }

  Action call(const AbstractCoroutine::FunctionPtr& ptr) override {
    Function f = static_cast<Function>(ptr);
    return (static_cast<T*>(this)->*f)();
  }

  Action yieldTo(const Function& function) const {
    return Action(static_cast<AbstractCoroutine::FunctionPtr>(function));
  }

  Action _return(const Args&... args) {
    this->m_parentReturnAction = this->m_parentMemberCaller->call(this->getParent(), args...);
    return Action::createActionByType(Action::TYPE_FINISH);
  }
  
};

}}

#endif
