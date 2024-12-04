

#ifndef booster_utils_Conversion_hpp
#define booster_utils_Conversion_hpp

#include "booster/data/type/Primitive.hpp"
#include "booster/Types.hpp"


#include "booster/base/Countable.hpp"
#include "booster/Environment.hpp"

#include <string>

namespace booster { namespace utils {


class Conversion {
public:


  static v_int32 strToInt32(const char *str);


  static v_int32 strToInt32(const booster::String &str, bool &success);


  static v_uint32 strToUInt32(const char *str);


  static v_uint32 strToUInt32(const booster::String &str, bool &success);


  static v_int64 strToInt64(const char *str);


  static v_int64 strToInt64(const booster::String &str, bool &success);


  static v_uint64 strToUInt64(const char *str);


  static v_uint64 strToUInt64(const booster::String &str, bool &success);


  static v_buff_size int32ToCharSequence(v_int32 value, p_char8 data, v_buff_size n);


  static v_buff_size uint32ToCharSequence(v_uint32 value, p_char8 data, v_buff_size n);


  static v_buff_size int64ToCharSequence(v_int64 value, p_char8 data, v_buff_size n);


  static v_buff_size uint64ToCharSequence(v_uint64 value, p_char8 data, v_buff_size n);


  static booster::String int32ToStr(v_int32 value);


  static booster::String uint32ToStr(v_uint32 value);


  static booster::String int64ToStr(v_int64 value);


  static booster::String uint64ToStr(v_uint64 value);


  static std::string int32ToStdStr(v_int32 value);


  static std::string uint32ToStdStr(v_uint32 value);


  static std::string int64ToStdStr(v_int64 value);


  static std::string uint64ToStdStr(v_uint64 value);


  template<typename T>
  static v_buff_size primitiveToCharSequence(T value, p_char8 data, v_buff_size n, const char *pattern) {
    return snprintf(reinterpret_cast<char *>(data), static_cast<size_t>(n), pattern, value);
  }


  template<typename T>
  static booster::String primitiveToStr(T value, const char *pattern) {
    v_char8 buff[100];
    auto size = primitiveToCharSequence(value, &buff[0], 100, pattern);
    if (size > 0) {
      return booster::String(reinterpret_cast<const char *>(&buff[0]), size);
    }
    return nullptr;
  }


  static v_float32 strToFloat32(const char *str);


  static v_float32 strToFloat32(const booster::String &str, bool &success);


  static v_float64 strToFloat64(const char *str);


  static v_float64 strToFloat64(const booster::String &str, bool &success);


  static v_buff_size float32ToCharSequence(v_float32 value, p_char8 data, v_buff_size n, const char *format = BOOSTER_FLOAT_STRING_FORMAT);


  static v_buff_size float64ToCharSequence(v_float64 value, p_char8 data, v_buff_size n, const char *format = BOOSTER_FLOAT_STRING_FORMAT);


  static booster::String float32ToStr(v_float32 value, const char *format = BOOSTER_FLOAT_STRING_FORMAT);


  static booster::String float64ToStr(v_float64 value, const char *format = BOOSTER_FLOAT_STRING_FORMAT);


  static booster::String boolToStr(bool value);


  static bool strToBool(const booster::String &str, bool &success);

};

}}

#endif
