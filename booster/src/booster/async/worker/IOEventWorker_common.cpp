#include "IOEventWorker.hpp"
#include <unistd.h>


namespace booster { namespace async { namespace worker {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IOEventWorker

IOEventWorker::IOEventWorker(IOEventWorkerForeman* foreman, Action::IOEventType specialization)
  : Worker(Type::IO)
  , m_foreman(foreman)
  , m_specialization(specialization)
  , m_running(true)
  , m_eventQueueHandle(INVALID_IO_HANDLE)
  , m_wakeupTrigger(INVALID_IO_HANDLE)
  , m_inEvents(nullptr)
  , m_inEventsCount(0)
  , m_inEventsCapacity(0)
  , m_outEvents(nullptr)
{
  m_thread = std::thread(&IOEventWorker::run, this);
}


IOEventWorker::~IOEventWorker() {

  if(m_eventQueueHandle >=0) {
    ::close(m_eventQueueHandle);
  }

  if(m_wakeupTrigger >= 0) {
    ::close(m_wakeupTrigger);
  }

}


void IOEventWorker::pushTasks(utils::FastQueue<CoroutineHandle> &tasks) {
  if (tasks.first != nullptr) {
    {
      std::lock_guard<booster::concurrency::SpinLock> guard(m_backlogLock);
      utils::FastQueue<CoroutineHandle>::moveAll(tasks, m_backlog);
    }
    triggerWakeup();
  }
}

void IOEventWorker::pushOneTask(CoroutineHandle *task) {
  {
    std::lock_guard<booster::concurrency::SpinLock> guard(m_backlogLock);
    m_backlog.pushBack(task);
  }
  triggerWakeup();
}

void IOEventWorker::run() {

  initEventQueue();

  while (m_running) {
    consumeBacklog();
    waitEvents();
  }

}

void IOEventWorker::stop() {
  {
    std::lock_guard<booster::concurrency::SpinLock> lock(m_backlogLock);
    m_running = false;
  }
  triggerWakeup();
}

void IOEventWorker::join() {
  m_thread.join();
}

void IOEventWorker::detach() {
  m_thread.detach();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IOEventWorkerForeman

IOEventWorkerForeman::IOEventWorkerForeman()
  : Worker(Type::IO)
  , m_reader(this, Action::IOEventType::IO_EVENT_READ)
  , m_writer(this, Action::IOEventType::IO_EVENT_WRITE)
{}

IOEventWorkerForeman::~IOEventWorkerForeman() {
}

void IOEventWorkerForeman::pushTasks(utils::FastQueue<CoroutineHandle>& tasks) {

  utils::FastQueue<CoroutineHandle> readerTasks;
  utils::FastQueue<CoroutineHandle> writerTasks;

  while(tasks.first != nullptr) {

    CoroutineHandle* coroutine = tasks.popFront();
    auto& action = getCoroutineScheduledAction(coroutine);

    switch(action.getIOEventType()) {

      case Action::IOEventType::IO_EVENT_READ:
        readerTasks.pushBack(coroutine);
        break;

      case Action::IOEventType::IO_EVENT_WRITE:
        writerTasks.pushBack(coroutine);
        break;

      default:
        throw std::runtime_error("[booster::async::worker::IOEventWorkerForeman::pushTasks()]: Error. Unknown Action Event Type.");

    }

  }

  if(readerTasks.first != nullptr) {
    m_reader.pushTasks(readerTasks);
  }

  if(writerTasks.first != nullptr) {
    m_writer.pushTasks(writerTasks);
  }

}

void IOEventWorkerForeman::pushOneTask(CoroutineHandle* task) {

  auto& action = getCoroutineScheduledAction(task);

  switch(action.getIOEventType()) {

    case Action::IOEventType::IO_EVENT_READ:
      m_reader.pushOneTask(task);
      break;

    case Action::IOEventType::IO_EVENT_WRITE:
      m_writer.pushOneTask(task);
      break;

    default:
      throw std::runtime_error("[booster::async::worker::IOEventWorkerForeman::pushTasks()]: Error. Unknown Action Event Type.");

  }

}

void IOEventWorkerForeman::stop() {
  m_writer.stop();
  m_reader.stop();
}

void IOEventWorkerForeman::join() {
  m_reader.join();
  m_writer.join();
}

void IOEventWorkerForeman::detach() {
  m_reader.detach();
  m_writer.detach();
}

}}}
