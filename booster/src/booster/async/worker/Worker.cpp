

#include "Worker.hpp"

namespace booster { namespace async { namespace worker {

Worker::Worker(Type type)
  : m_type(type)
{}

void Worker::setCoroutineScheduledAction(CoroutineHandle* coroutine, Action &&action) {
  coroutine->_SCH_A = std::forward<Action>(action);
}

Action& Worker::getCoroutineScheduledAction(CoroutineHandle* coroutine) {
  return coroutine->_SCH_A;
}

Processor* Worker::getCoroutineProcessor(CoroutineHandle* coroutine) {
  return coroutine->_PP;
}

void Worker::dismissAction(Action& action) {
  action.m_type = Action::TYPE_NONE;
}

CoroutineHandle* Worker::nextCoroutine(CoroutineHandle* coroutine) {
  return coroutine->_ref;
}

Worker::Type Worker::getType() {
  return m_type;
}

}}}
