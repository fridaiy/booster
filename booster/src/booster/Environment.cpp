

#include "Environment.hpp"

#include "booster/base/Log.hpp"

#include <iomanip>
#include <chrono>
#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdarg>

#if defined(WIN32) || defined(_WIN32)
	#include <winsock2.h>

  struct tm* localtime_r(time_t *_clock, struct tm *_result) {
      localtime_s(_result, _clock);
      return _result;
  }
#endif

namespace booster {

v_atomicCounter Environment::m_objectsCount(0);
v_atomicCounter Environment::m_objectsCreated(0);

#ifndef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
thread_local v_counter Environment::m_threadLocalObjectsCount = 0;
thread_local v_counter Environment::m_threadLocalObjectsCreated = 0;
#endif

std::mutex& Environment::getComponentsMutex() {
  static std::mutex componentsMutex;
  return componentsMutex;
}

std::unordered_map<std::string, std::unordered_map<std::string, void*>>& Environment::getComponents() {
  static std::unordered_map<std::string, std::unordered_map<std::string, void*>> components;
  return components;
}

std::shared_ptr<Logger> Environment::m_logger;

DefaultLogger::DefaultLogger(const Config& config)
  : m_config(config)
{}

void DefaultLogger::log(v_uint32 priority, const std::string& tag, const std::string& message) {

  bool indent = false;
  auto time = std::chrono::system_clock::now().time_since_epoch();

  std::lock_guard<std::mutex> lock(m_lock);

  switch (priority) {
    case PRIORITY_V:
      std::cout << "\033[0m V \033[0m|";
      break;

    case PRIORITY_D:
      std::cout << "\033[34m D \033[0m|";
      break;

    case PRIORITY_I:
      std::cout << "\033[32m I \033[0m|";
      break;

    case PRIORITY_W:
      std::cout << "\033[45m W \033[0m|";
      break;

    case PRIORITY_E:
      std::cout << "\033[41m E \033[0m|";
      break;

    default:
      std::cout << " " << priority << " |";
  }

  if (m_config.timeFormat) {
	time_t seconds = std::chrono::duration_cast<std::chrono::seconds>(time).count();
    tm now;
    localtime_r(&seconds, &now);
#ifdef BOOSTER_DISABLE_STD_PUT_TIME
	  char timeBuffer[50];
      strftime(timeBuffer, sizeof(timeBuffer), m_config.timeFormat, &now);
      std::cout << timeBuffer;
#else
      std::cout << std::put_time(&now, m_config.timeFormat);
#endif
    indent = true;
  }

  if (m_config.printTicks) {
    auto ticks = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
    if(indent) {
      std::cout << " ";
    }
    std::cout << ticks;
    indent = true;
  }

  if (indent) {
    std::cout << "|";
  }

  if (message.empty()) {
    std::cout << " " << tag << std::endl;
  } else {
    std::cout << " " << tag << ":" << message << std::endl;
  }

}

void DefaultLogger::enablePriority(v_uint32 priority) {
  if (priority > PRIORITY_E) {
    return;
  }
  m_config.logMask |= (1U << priority);
}

void DefaultLogger::disablePriority(v_uint32 priority) {
  if (priority > PRIORITY_E) {
    return;
  }
  m_config.logMask &= ~(1U << priority);
}

bool DefaultLogger::isLogPriorityEnabled(v_uint32 priority) {
  if (priority > PRIORITY_E) {
    return true;
  }
  return m_config.logMask & (1U << priority);
}

void LogCategory::enablePriority(v_uint32 priority) {
  if (priority > Logger::PRIORITY_E) {
    return;
  }
  enabledPriorities |= (1U << priority);
}

void LogCategory::disablePriority(v_uint32 priority) {
  if (priority > Logger::PRIORITY_E) {
    return;
  }
  enabledPriorities &= ~(1U << priority);
}

bool LogCategory::isLogPriorityEnabled(v_uint32 priority) {
  if (priority > Logger::PRIORITY_E) {
    return true;
  }
  return enabledPriorities & (1U << priority);
}

void Environment::init() {
  init(std::make_shared<DefaultLogger>());
}

void Environment::init(const std::shared_ptr<Logger>& logger) {

  m_logger = logger;

#if defined(WIN32) || defined(_WIN32)
    // Initialize Winsock
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {

        throw std::runtime_error("[booster::Environment::init()]: Error. WSAStartup failed");
    }
#endif

  checkTypes();

  m_objectsCount = 0;
  m_objectsCreated = 0;

#ifndef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
  m_threadLocalObjectsCount = 0;
  m_threadLocalObjectsCreated = 0;
#endif

  {
    std::lock_guard<std::mutex> lock(getComponentsMutex());
    if (getComponents().size() > 0) {
      throw std::runtime_error("[booster::Environment::init()]: Error. "
                               "Invalid state. Components were created before call to Environment::init()");
    }
  }

}

void Environment::destroy(){
  {
    std::lock_guard<std::mutex> lock(getComponentsMutex());
    if(getComponents().size() > 0) {
      throw std::runtime_error("[booster::Environment::destroy()]: Error. "
                               "Invalid state. Leaking components");
    }
  }
  m_logger.reset();

#if defined(WIN32) || defined(_WIN32)
    WSACleanup();
#endif
}

void Environment::checkTypes(){

  static_assert(sizeof(v_char8) == 1, "");
  static_assert(sizeof(v_int16) == 2, "");
  static_assert(sizeof(v_uint16) == 2, "");
  static_assert(sizeof(v_int32) == 4, "");
  static_assert(sizeof(v_int64) == 8, "");
  static_assert(sizeof(v_uint32) == 4, "");
  static_assert(sizeof(v_uint64) == 8, "");
  static_assert(sizeof(v_float64) == 8, "");

  v_int32 vInt32 = ~(1);
  v_int64 vInt64 = ~v_int64(1);
  v_uint32 vUInt32 = ~v_uint32(1);
  v_uint64 vUInt64 = ~v_uint64(1);

  BOOSTER_ASSERT(vInt32 < 0)
  BOOSTER_ASSERT(vInt64 < 0)
  BOOSTER_ASSERT(vUInt32 > 0)
  BOOSTER_ASSERT(vUInt64 > 0)

}

void Environment::incObjects(){

  m_objectsCount ++;
  m_objectsCreated ++;

#ifndef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
  m_threadLocalObjectsCount ++;
  m_threadLocalObjectsCreated ++;
#endif

}

void Environment::decObjects(){

  m_objectsCount --;

#ifndef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
  m_threadLocalObjectsCount --;
#endif

}

v_counter Environment::getObjectsCount(){
  return m_objectsCount;
}

v_counter Environment::getObjectsCreated(){
  return m_objectsCreated;
}

v_counter Environment::getThreadLocalObjectsCount(){
#ifndef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
  return m_threadLocalObjectsCount;
#else
  return 0;
#endif
}

v_counter Environment::getThreadLocalObjectsCreated(){
#ifndef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
  return m_threadLocalObjectsCreated;
#else
  return 0;
#endif
}

void Environment::setLogger(const std::shared_ptr<Logger>& logger){
  m_logger = logger;
}

std::shared_ptr<Logger> Environment::getLogger() {
  return m_logger;
}

void Environment::printCompilationConfig() {

  BOOSTER_LOGd("booster-version", BOOSTER_VERSION)

#ifdef BOOSTER_DISABLE_ENV_OBJECT_COUNTERS
  BOOSTER_LOGd("booster/Config", "BOOSTER_DISABLE_ENV_OBJECT_COUNTERS")
#endif

#ifdef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
  BOOSTER_LOGd("booster/Config", "BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL")
#endif

#ifdef BOOSTER_THREAD_HARDWARE_CONCURRENCY
  BOOSTER_LOGd("booster/Config", "BOOSTER_THREAD_HARDWARE_CONCURRENCY={}", BOOSTER_THREAD_HARDWARE_CONCURRENCY)
#endif

}

void Environment::log(v_uint32 priority, const std::string& tag, const std::string& message) {
  if(m_logger != nullptr) {
    m_logger->log(priority, tag, message);
  }
}

void Environment::registerComponent(const std::string& typeName, const std::string& componentName, void* component) {
  std::lock_guard<std::mutex> lock(getComponentsMutex());
  auto& bucket = getComponents()[typeName];
  auto it = bucket.find(componentName);
  if(it != bucket.end()){
    throw std::runtime_error("[booster::Environment::registerComponent()]: Error. Component with given name already exists: name='" + componentName + "'");
  }
  bucket[componentName] = component;
}

void Environment::unregisterComponent(const std::string& typeName, const std::string& componentName) {
  std::lock_guard<std::mutex> lock(getComponentsMutex());
  auto& components = getComponents();
  auto bucketIt = getComponents().find(typeName);
  if(bucketIt == components.end() || bucketIt->second.size() == 0) {
    throw std::runtime_error("[booster::Environment::unregisterComponent()]: Error. Component of given type doesn't exist: type='" + typeName + "'");
  }
  auto& bucket = bucketIt->second;
  auto componentIt = bucket.find(componentName);
  if(componentIt == bucket.end()) {
    throw std::runtime_error("[booster::Environment::unregisterComponent()]: Error. Component with given name doesn't exist: name='" + componentName + "'");
  }
  bucket.erase(componentIt);
  if(bucket.size() == 0) {
    components.erase(bucketIt);
  }
}

void* Environment::getComponent(const std::string& typeName) {
  std::lock_guard<std::mutex> lock(getComponentsMutex());
  auto& components = getComponents();
  auto bucketIt = components.find(typeName);
  if(bucketIt == components.end() || bucketIt->second.size() == 0) {
    throw std::runtime_error("[booster::Environment::getComponent()]: Error. Component of given type doesn't exist: type='" + typeName + "'");
  }
  auto bucket = bucketIt->second;
  if(bucket.size() > 1){
    throw std::runtime_error("[booster::Environment::getComponent()]: Error. Ambiguous component reference. Multiple components exist for a given type: type='" + typeName + "'");
  }
  return bucket.begin()->second;
}

void* Environment::getComponent(const std::string& typeName, const std::string& componentName) {
  std::lock_guard<std::mutex> lock(getComponentsMutex());
  auto& components = getComponents();
  auto bucketIt = components.find(typeName);
  if(bucketIt == components.end() || bucketIt->second.size() == 0) {
    throw std::runtime_error("[booster::Environment::getComponent()]: Error. Component of given type doesn't exist: type='" + typeName + "'");
  }
  auto bucket = bucketIt->second;
  auto componentIt = bucket.find(componentName);
  if(componentIt == bucket.end()) {
    throw std::runtime_error("[booster::Environment::getComponent()]: Error. Component with given name doesn't exist: name='" + componentName + "'");
  }
  return componentIt->second;
}

v_int64 Environment::getMicroTickCount(){
  std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>
  (std::chrono::system_clock::now().time_since_epoch());
  return ms.count();
}

}
