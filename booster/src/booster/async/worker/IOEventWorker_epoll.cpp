#include "IOEventWorker.hpp"

#ifdef BOOSTER_IO_EVENT_INTERFACE_EPOLL

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// epoll based implementation

#include "booster/async/Processor.hpp"
#include "booster/base/Log.hpp"

#include <unistd.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

namespace booster { namespace async { namespace worker {

void IOEventWorker::initEventQueue() {

#if !defined __ANDROID_API__ || __ANDROID_API__ >= 21
  m_eventQueueHandle = ::epoll_create1(0);
#else
  m_eventQueueHandle = ::epoll_create(0);
#endif

  if(m_eventQueueHandle == -1) {
    BOOSTER_LOGe("[booster::async::worker::IOEventWorker::initEventQueue()]", "Error. Call to ::epoll_create1() failed. errno={}", errno)
    throw std::runtime_error("[booster::async::worker::IOEventWorker::initEventQueue()]: Error. Call to ::epoll_create1() failed.");
  }

  m_outEvents = std::unique_ptr<v_char8[]>(new (std::nothrow) v_char8[MAX_EVENTS * sizeof(epoll_event)]);

  if(!m_outEvents) {
    BOOSTER_LOGe("[booster::async::worker::IOEventWorker::initEventQueue()]",
               "Error. Unable to allocate {} bytes for events.", MAX_EVENTS * sizeof(epoll_event))
    throw std::runtime_error("[booster::async::worker::IOEventWorker::initEventQueue()]: Error. Unable to allocate memory for events.");
  }

  m_wakeupTrigger = ::eventfd(0, EFD_NONBLOCK);

  if(m_wakeupTrigger == -1) {
    BOOSTER_LOGe("[booster::async::worker::IOEventWorker::initEventQueue()]", "Error. Call to ::eventfd() failed. errno={}", errno)
    throw std::runtime_error("[booster::async::worker::IOEventWorker::initEventQueue()]: Error. Call to ::eventfd() failed.");
  }

  epoll_event event;
  std::memset(&event, 0, sizeof(epoll_event));

  event.data.ptr = this;

#ifdef EPOLLEXCLUSIVE
  event.events = EPOLLIN | EPOLLET | EPOLLEXCLUSIVE;
#else
  event.events = EPOLLIN | EPOLLET;
#endif

  auto res = ::epoll_ctl(m_eventQueueHandle, EPOLL_CTL_ADD, m_wakeupTrigger, &event);
  if(res == -1) {
    BOOSTER_LOGe("[booster::async::worker::IOEventWorker::initEventQueue()]", "Error. Call to ::epoll_ctl failed. errno={}", errno)
    throw std::runtime_error("[booster::async::worker::IOEventWorker::initEventQueue()]: Error. Call to ::epoll_ctl() failed.");
  }

}

void IOEventWorker::triggerWakeup() {
  eventfd_write(m_wakeupTrigger, 1);
}

void IOEventWorker::setTriggerEvent(p_char8 eventPtr) {
  (void) eventPtr;
}

void IOEventWorker::setCoroutineEvent(CoroutineHandle* coroutine, int operation, p_char8 eventPtr) {
  (void) eventPtr;

  auto& action = getCoroutineScheduledAction(coroutine);

  switch(action.getType()) {

    case Action::TYPE_IO_WAIT: break;
    case Action::TYPE_IO_REPEAT: break;

    default:
      BOOSTER_LOGe("[booster::async::worker::IOEventWorker::pushCoroutineToQueue()]", "Error. Unknown Action. action.getType()=={}", action.getType())
      throw std::runtime_error("[booster::async::worker::IOEventWorker::pushCoroutineToQueue()]: Error. Unknown Action.");

  }

  epoll_event event;
  std::memset(&event, 0, sizeof(epoll_event));

  event.data.ptr = coroutine;

  switch(action.getIOEventType()) {

    case Action::IOEventType::IO_EVENT_READ:
      event.events = EPOLLIN  | EPOLLET | EPOLLONESHOT;
      break;

    case Action::IOEventType::IO_EVENT_WRITE:
      event.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
      break;

    default:
      throw std::runtime_error("[booster::async::worker::IOEventWorker::pushCoroutineToQueue()]: Error. Unknown Action Event Type.");

  }

  auto res = epoll_ctl(m_eventQueueHandle, operation, action.getIOHandle(), &event);
  if(res == -1) {
    BOOSTER_LOGe("[booster::async::worker::IOEventWorker::setEpollEvent()]", "Error. Call to epoll_ctl failed. operation={}, errno={}", operation, errno)
    throw std::runtime_error("[booster::async::worker::IOEventWorker::setEpollEvent()]: Error. Call to epoll_ctl failed.");
  }

}

void IOEventWorker::consumeBacklog() {

  std::lock_guard<booster::concurrency::SpinLock> lock(m_backlogLock);

  auto curr = m_backlog.first;
  while(curr != nullptr) {
    setCoroutineEvent(curr, EPOLL_CTL_ADD, nullptr);
    curr = nextCoroutine(curr);
  }

  m_backlog.first = nullptr;
  m_backlog.last = nullptr;
  m_backlog.count = 0;

}

void IOEventWorker::waitEvents() {

  epoll_event* outEvents = reinterpret_cast<epoll_event*>(m_outEvents.get());
  auto eventsCount = epoll_wait(m_eventQueueHandle, outEvents, MAX_EVENTS, -1);

  if((eventsCount < 0) && (errno != EINTR)) {
    BOOSTER_LOGe("[booster::async::worker::IOEventWorker::waitEvents()]", "Error:\n"
               "errno={}\n"
               "in-events={}\n"
               "foreman={}\n"
               "this={}\n"
               "specialization={}",
               errno, m_inEventsCount,
               reinterpret_cast<v_buff_usize>(m_foreman), reinterpret_cast<v_buff_usize>(this),
               static_cast<v_int32>(m_specialization))
    throw std::runtime_error("[booster::async::worker::IOEventWorker::waitEvents()]: Error. Event loop failed.");
  }

  utils::FastQueue<CoroutineHandle> popQueue;

  for(v_int32 i = 0; i < eventsCount; i ++) {

    void* dataPtr = outEvents[i].data.ptr;

    if(dataPtr != nullptr) {

      if(dataPtr == this) {

        eventfd_t value;
        eventfd_read(m_wakeupTrigger, &value);

      } else {

        auto coroutine = reinterpret_cast<CoroutineHandle*>(dataPtr);

        Action action = coroutine->iterate();

        int res;

        switch(action.getIOEventCode() | m_specialization) {

          case Action::CODE_IO_WAIT_READ:
            setCoroutineScheduledAction(coroutine, std::move(action));
            setCoroutineEvent(coroutine, EPOLL_CTL_MOD, nullptr);
            break;

          case Action::CODE_IO_WAIT_WRITE:
            setCoroutineScheduledAction(coroutine, std::move(action));
            setCoroutineEvent(coroutine, EPOLL_CTL_MOD, nullptr);
            break;

          case Action::CODE_IO_REPEAT_READ:
            setCoroutineScheduledAction(coroutine, std::move(action));
            setCoroutineEvent(coroutine, EPOLL_CTL_MOD, nullptr);
            break;

          case Action::CODE_IO_REPEAT_WRITE:
            setCoroutineScheduledAction(coroutine, std::move(action));
            setCoroutineEvent(coroutine, EPOLL_CTL_MOD, nullptr);
            break;

          case Action::CODE_IO_WAIT_RESCHEDULE:

            res = epoll_ctl(m_eventQueueHandle, EPOLL_CTL_DEL, action.getIOHandle(), nullptr);
            if(res == -1) {
              BOOSTER_LOGe(
                "[booster::async::worker::IOEventWorker::waitEvents()]",
                "Error. Call to epoll_ctl failed. operation={}, errno={}. action_code={}, worker_specialization={}",
                EPOLL_CTL_DEL, errno, action.getIOEventCode(), static_cast<v_int32>(m_specialization)
              )
              throw std::runtime_error("[booster::async::worker::IOEventWorker::waitEvents()]: Error. Call to epoll_ctl failed.");
            }

            setCoroutineScheduledAction(coroutine, std::move(action));
            popQueue.pushBack(coroutine);

            break;

          case Action::CODE_IO_REPEAT_RESCHEDULE:

            res = epoll_ctl(m_eventQueueHandle, EPOLL_CTL_DEL, action.getIOHandle(), nullptr);
            if(res == -1) {
              BOOSTER_LOGe(
                "[booster::async::worker::IOEventWorker::waitEvents()]",
                "Error. Call to epoll_ctl failed. operation={}, errno={}. action_code={}, worker_specialization={}",
                EPOLL_CTL_DEL, errno, action.getIOEventCode(), static_cast<v_int32>(m_specialization)
              )
              throw std::runtime_error("[booster::async::worker::IOEventWorker::waitEvents()]: Error. Call to epoll_ctl failed.");
            }

            setCoroutineScheduledAction(coroutine, std::move(action));
            popQueue.pushBack(coroutine);

            break;


          default:

            auto& prevAction = getCoroutineScheduledAction(coroutine);

            res = epoll_ctl(m_eventQueueHandle, EPOLL_CTL_DEL, prevAction.getIOHandle(), nullptr);
            if(res == -1) {
              BOOSTER_LOGe("[booster::async::worker::IOEventWorker::waitEvents()]", "Error. Call to epoll_ctl failed. operation={}, errno={}", EPOLL_CTL_DEL, errno)
              throw std::runtime_error("[booster::async::worker::IOEventWorker::waitEvents()]: Error. Call to epoll_ctl failed.");
            }

            setCoroutineScheduledAction(coroutine, std::move(action));
            getCoroutineProcessor(coroutine)->pushOneTask(coroutine);

        }

      }

    }

  }

  if(popQueue.count > 0) {
    m_foreman->pushTasks(popQueue);
  }

}

}}}

#endif
