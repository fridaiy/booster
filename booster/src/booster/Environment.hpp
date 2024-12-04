

#ifndef booster_Environment_hpp
#define booster_Environment_hpp

#include "booster/base/Compiler.hpp"
#include "booster/base/Config.hpp"

#include <cstdarg>
#include <cstdio>
#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <cstdlib>

#define BOOSTER_VERSION "1.4.0"

typedef unsigned char v_char8;
typedef v_char8 *p_char8;

typedef int8_t v_int8;
typedef v_int8* p_int8;
typedef uint8_t v_uint8;
typedef v_uint8* p_uint8;

typedef int16_t v_int16;
typedef v_int16* p_int16;
typedef uint16_t v_uint16;
typedef v_uint16* p_uint16;

typedef int32_t v_int32;
typedef v_int32* p_int32;
typedef uint32_t v_uint32;
typedef v_uint32* p_uint32;

typedef int64_t v_int64;
typedef v_int64* p_int64;
typedef uint64_t v_uint64;
typedef v_uint64* p_uint64;

typedef double v_float64;
typedef v_float64 * p_float64;

typedef float v_float32;
typedef v_float32* p_float32;

typedef std::atomic_int_fast64_t v_atomicCounter;
typedef v_int64 v_counter;


typedef intptr_t v_buff_size;
typedef v_buff_size* p_buff_size;

typedef uintptr_t v_buff_usize;
typedef v_buff_usize* p_buff_usize;

namespace booster {


class Logger {
public:

  static constexpr v_uint32 PRIORITY_V = 0;


  static constexpr v_uint32 PRIORITY_D = 1;


  static constexpr v_uint32 PRIORITY_I = 2;


  static constexpr v_uint32 PRIORITY_W = 3;


  static constexpr v_uint32 PRIORITY_E = 4;

public:

  virtual ~Logger() = default;


  virtual void log(v_uint32 priority, const std::string& tag, const std::string& message) = 0;


  virtual bool isLogPriorityEnabled(v_uint32 ) {
    return true;
  }


  virtual v_buff_size getMaxFormattingBufferSize() {
    return 4096;
  }
};


class LogCategory {
 public:

  LogCategory(std::string pTag, bool pCategoryEnabled, v_uint32 pEnabledPriorities = ((1<<Logger::PRIORITY_V) | (1<<Logger::PRIORITY_D) | (1<<Logger::PRIORITY_I) | (1<<Logger::PRIORITY_W) | (1<<Logger::PRIORITY_E)))
    : tag(std::move(pTag))
    , categoryEnabled(pCategoryEnabled)
    , enabledPriorities(pEnabledPriorities)
  {}


  const std::string tag;


  bool categoryEnabled;


  v_uint32 enabledPriorities;


  void enablePriority(v_uint32 priority);


  void disablePriority(v_uint32 priority);


  bool isLogPriorityEnabled(v_uint32 priority);
};


class DefaultLogger : public Logger {
public:

  struct Config {


    Config(const char* tfmt, bool printMicroTicks, v_uint32 initialLogMask)
      : timeFormat(tfmt)
      , printTicks(printMicroTicks)
      , logMask(initialLogMask)
    {}


    const char* timeFormat;


    bool printTicks;


    v_uint32 logMask;
  };
private:
  Config m_config;
  std::mutex m_lock;
public:


  DefaultLogger(const Config& config = Config(
          "%Y-%m-%d %H:%M:%S",
          true,
          (1 << PRIORITY_V) | (1 << PRIORITY_D) | (1 << PRIORITY_I) | (1 << PRIORITY_W) | (1 << PRIORITY_E)
          ));


  void log(v_uint32 priority, const std::string& tag, const std::string& message) override;


  void enablePriority(v_uint32 priority);


  void disablePriority(v_uint32 priority);


  bool isLogPriorityEnabled(v_uint32 priority) override;
};


class Environment{
private:

  static v_atomicCounter m_objectsCount;
  static v_atomicCounter m_objectsCreated;

#ifndef BOOSTER_COMPAT_BUILD_NO_THREAD_LOCAL
  static thread_local v_counter m_threadLocalObjectsCount;
  static thread_local v_counter m_threadLocalObjectsCreated;
#endif
private:

  static std::mutex& getComponentsMutex();
  static std::unordered_map<std::string, std::unordered_map<std::string, void*>>& getComponents();

private:
  static std::shared_ptr<Logger> m_logger;
  static void checkTypes();
public:


  template <typename T>
  class Component {
  private:
    std::string m_type;
    std::string m_name;
    T m_object;
  public:


    Component(const std::string& name, const T& object)
      : m_type(typeid(T).name())
      , m_name(name)
      , m_object(object)
    {
      Environment::registerComponent(m_type, m_name, &m_object);
    }


    Component(const T& object)
      : Component("NoName", object)
    {}


    ~Component() {
      Environment::unregisterComponent(m_type, m_name);
    }


    T getObject() {
      return m_object;
    }
    
  };
  
private:
  static void registerComponent(const std::string& typeName, const std::string& componentName, void* component);
  static void unregisterComponent(const std::string& typeName, const std::string& componentName);
public:


  static void init();


  static void init(const std::shared_ptr<Logger>& logger);


  static void destroy();


  static void incObjects();


  static void decObjects();


  static v_counter getObjectsCount();


  static v_counter getObjectsCreated();


  static v_counter getThreadLocalObjectsCount();


  static v_counter getThreadLocalObjectsCreated();


  static void setLogger(const std::shared_ptr<Logger>& logger);


  static std::shared_ptr<Logger> getLogger();


  static void printCompilationConfig();


  static void log(v_uint32 priority, const std::string& tag, const std::string& message);


  static void* getComponent(const std::string& typeName);


  static void* getComponent(const std::string& typeName, const std::string& componentName);


  static v_int64 getMicroTickCount();
  
};
  
}


#endif
