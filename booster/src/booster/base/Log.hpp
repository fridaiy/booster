

#ifndef booster_base_Log_hpp
#define booster_base_Log_hpp

#include "booster/data/stream/BufferStream.hpp"
#include "booster/macro/basic.hpp"

namespace booster { namespace base {

class LogMessage {
private:

  struct Parameter {
    v_buff_size startPos;
    v_buff_size endPos;
  };

private:
  bool writeNextChunk();
private:
  booster::String m_msg;
  mutable data::stream::BufferOutputStream m_stream;
  v_uint64 m_currParam;
  std::vector<Parameter> m_params;
public:

  explicit LogMessage(const booster::String& msg);

  std::string toStdString() const;

  LogMessage& operator << (const char* str);
  LogMessage& operator << (bool value);

  LogMessage& operator << (char value);
  LogMessage& operator << (unsigned char value);
  LogMessage& operator << (short value);
  LogMessage& operator << (unsigned short value);
  LogMessage& operator << (int value);
  LogMessage& operator << (unsigned value);
  LogMessage& operator << (long value);
  LogMessage& operator << (unsigned long value);
  LogMessage& operator << (long long value);
  LogMessage& operator << (unsigned long long value);
  LogMessage& operator << (float value);
  LogMessage& operator << (double value);
  LogMessage& operator << (long double value);

  LogMessage& operator << (const booster::String& str);
  LogMessage& operator << (const Boolean& value);
  LogMessage& operator << (const Int8& value);
  LogMessage& operator << (const UInt8& value);
  LogMessage& operator << (const Int16& value);
  LogMessage& operator << (const UInt16& value);
  LogMessage& operator << (const Int32& value);
  LogMessage& operator << (const UInt32& value);
  LogMessage& operator << (const Int64& value);
  LogMessage& operator << (const UInt64& value);
  LogMessage& operator << (const Float32& value);
  LogMessage& operator << (const Float64& value);

};

struct Log {

  static void ignore(std::initializer_list<void*> list) {
    (void) list;
  }

  template<typename ... Types>
  static void stream(v_uint32 priority, const std::string& tag, const booster::String& message, Types... args) {
    booster::base::LogMessage msg(message);
    ignore({std::addressof(msg << args)...});
    log(priority, tag, msg);
  }

  template<typename ... Types>
  static void stream(v_uint32 priority, const LogCategory& category, const booster::String& message, Types... args) {
    booster::base::LogMessage msg(message);
    ignore({std::addressof(msg << args)...});
    log(priority, category, msg);
  }

  static void log(v_uint32 priority, const std::string& tag, const LogMessage& message);
  static void log(v_uint32 priority, const LogCategory& category, const LogMessage& message);

};

}}

////////////////////////////
////////////////////////////
////////////////////////////

#ifndef BOOSTER_DISABLE_LOGV


#define BOOSTER_LOGv(TAG, ...) \
  booster::base::Log::stream(booster::Logger::PRIORITY_V, TAG, __VA_ARGS__);

#else
  #define BOOSTER_LOGv(TAG, ...)
#endif

#ifndef BOOSTER_DISABLE_LOGD


#define BOOSTER_LOGd(TAG, ...) \
  booster::base::Log::stream(booster::Logger::PRIORITY_D, TAG, __VA_ARGS__);

#else
  #define BOOSTER_LOGd(TAG, ...)
#endif

#ifndef BOOSTER_DISABLE_LOGI


#define BOOSTER_LOGi(TAG, ...) \
  booster::base::Log::stream(booster::Logger::PRIORITY_I, TAG, __VA_ARGS__);

#else
  #define BOOSTER_LOGi(TAG, ...)
#endif

#ifndef BOOSTER_DISABLE_LOGW


#define BOOSTER_LOGw(TAG, ...) \
  booster::base::Log::stream(booster::Logger::PRIORITY_W, TAG, __VA_ARGS__);

#else
  #define BOOSTER_LOGw(TAG, ...)
#endif

#ifndef BOOSTER_DISABLE_LOGE


#define BOOSTER_LOGe(TAG, ...) \
  booster::base::Log::stream(booster::Logger::PRIORITY_E, TAG, __VA_ARGS__);

#else
  #define BOOSTER_LOGe(TAG, ...)
#endif

//////////////////////
//////////////////////
//////////////////////



#define BOOSTER_DECLARE_LOG_CATEGORY(NAME) \
  static booster::LogCategory NAME;


#define BOOSTER_LOG_CATEGORY(NAME, TAG, ENABLED) \
  booster::LogCategory NAME = booster::LogCategory(TAG, ENABLED);


//////////////////////
//////////////////////
//////////////////////


#define BOOSTER_ASSERT_FMT(FMT, EXP) \
if(!(EXP)) { \
  BOOSTER_LOGe("\033[1mASSERT\033[0m[\033[1;31mFAILED\033[0m]", FMT, #EXP) \
  exit(EXIT_FAILURE); \
}


#define BOOSTER_ASSERT(EXP) \
if(!(EXP)) { \
  BOOSTER_LOGe("\033[1mASSERT\033[0m[\033[1;31mFAILED\033[0m]", #EXP) \
  exit(EXIT_FAILURE); \
}

#endif 
