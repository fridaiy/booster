

#include "Conversion.hpp"

#include <cstdlib>

namespace booster { namespace utils {
  
v_int32 Conversion::strToInt32(const char* str){
  char* end;
  return static_cast<v_int32>(std::strtol(str, &end, 10));
}

v_int32 Conversion::strToInt32(const booster::String& str, bool& success){
  if(str == nullptr || str->empty()) {
    success = false;
    return 0;
  }
  char* end;
  v_int32 result = static_cast<v_int32>(std::strtol(str->data(), &end, 10));
  success = ((reinterpret_cast<v_buff_size>(end) - reinterpret_cast<v_buff_size>(str->data())) == static_cast<v_buff_size>(str->size()));
  return result;
}

v_uint32 Conversion::strToUInt32(const char* str){
  char* end;
  return static_cast<v_uint32>(std::strtoul(str, &end, 10));
}

v_uint32 Conversion::strToUInt32(const booster::String& str, bool& success){
  if(str == nullptr || str->empty()) {
    success = false;
    return 0;
  }
  char* end;
  v_uint32 result = static_cast<v_uint32>(std::strtoul(str->data(), &end, 10));
  success = ((reinterpret_cast<v_buff_size>(end) - reinterpret_cast<v_buff_size>(str->data())) == static_cast<v_buff_size>(str->size()));
  return result;
}

v_int64 Conversion::strToInt64(const char* str){
  char* end;
  return std::strtoll(str, &end, 10);
}

v_int64 Conversion::strToInt64(const booster::String& str, bool& success){
  if(str == nullptr || str->empty()) {
    success = false;
    return 0;
  }
  char* end;
  v_int64 result = std::strtoll(str->data(), &end, 10);
  success = ((reinterpret_cast<v_buff_size>(end) - reinterpret_cast<v_buff_size>(str->data())) == static_cast<v_buff_size>(str->size()));
  return result;
}

v_uint64 Conversion::strToUInt64(const char* str){
  char* end;
  return std::strtoull(str, &end, 10);
}

v_uint64 Conversion::strToUInt64(const booster::String& str, bool& success){
  if(str == nullptr || str->empty()) {
    success = false;
    return 0;
  }
  char* end;
  v_uint64 result = std::strtoull(str->data(), &end, 10);
  success = ((reinterpret_cast<v_buff_size>(end) - reinterpret_cast<v_buff_size>(str->data())) == static_cast<v_buff_size>(str->size()));
  return result;
}

v_buff_size Conversion::int32ToCharSequence(v_int32 value, p_char8 data, v_buff_size n) {
  return snprintf(reinterpret_cast<char*>(data), static_cast<size_t>(n), "%ld", static_cast<long>(value));
}

v_buff_size Conversion::uint32ToCharSequence(v_uint32 value, p_char8 data, v_buff_size n) {
  return snprintf(reinterpret_cast<char*>(data), static_cast<size_t>(n), "%lu", static_cast<unsigned long>(value));
}

v_buff_size Conversion::int64ToCharSequence(v_int64 value, p_char8 data, v_buff_size n) {
  return snprintf(reinterpret_cast<char*>(data), static_cast<size_t>(n), "%lld", static_cast<long long int>(value));
}

v_buff_size Conversion::uint64ToCharSequence(v_uint64 value, p_char8 data, v_buff_size n) {
  return snprintf(reinterpret_cast<char*>(data), static_cast<size_t>(n), "%llu", static_cast<long long unsigned int>(value));
}

booster::String Conversion::int32ToStr(v_int32 value){
  v_char8 buff [16]; // Max 10 digits with 1 sign. 16 is plenty enough.
  auto size = int32ToCharSequence(value, &buff[0], 16);
  if(size > 0){
    return booster::String(reinterpret_cast<const char*>(&buff[0]), size);
  }
  return nullptr;
}

booster::String Conversion::uint32ToStr(v_uint32 value){
  v_char8 buff [16]; // Max 10 digits. 16 is plenty enough.
  auto size = uint32ToCharSequence(value, &buff[0], 16);
  if(size > 0){
    return booster::String(reinterpret_cast<const char*>(&buff[0]), size);
  }
  return nullptr;
}

booster::String Conversion::int64ToStr(v_int64 value){
  v_char8 buff [32]; // Max 20 digits unsigned, 19 digits +1 sign signed.
  auto size = int64ToCharSequence(value, &buff[0], 32);
  if(size > 0){
    return booster::String(reinterpret_cast<const char*>(&buff[0]), size);
  }
  return nullptr;
}

booster::String Conversion::uint64ToStr(v_uint64 value){
  v_char8 buff [32]; // Max 20 digits.
  auto size = uint64ToCharSequence(value, &buff[0], 32);
  if(size > 0){
    return booster::String(reinterpret_cast<const char*>(&buff[0]), size);
  }
  return nullptr;
}

std::string Conversion::int32ToStdStr(v_int32 value){
  v_char8 buff [16];
  auto size = int32ToCharSequence(value, &buff[0], 16);
  if(size > 0){
    return std::string(reinterpret_cast<const char*>(buff), static_cast<size_t>(size));
  }
  return nullptr;
}

std::string Conversion::uint32ToStdStr(v_uint32 value){
  v_char8 buff [16];
  auto size = uint32ToCharSequence(value, &buff[0], 16);
  if(size > 0){
    return std::string(reinterpret_cast<const char*>(buff), static_cast<size_t>(size));
  }
  return nullptr;
}

std::string Conversion::int64ToStdStr(v_int64 value){
  v_char8 buff [32];
  v_int32 size = v_int32(int64ToCharSequence(value, &buff[0], 32));
  if(size > 0){
    return std::string(reinterpret_cast<const char*>(buff), static_cast<size_t>(size));
  }
  return nullptr;
}

std::string Conversion::uint64ToStdStr(v_uint64 value){
  v_char8 buff [32];
  auto size = uint64ToCharSequence(value, &buff[0], 32);
  if(size > 0){
    return std::string(reinterpret_cast<const char*>(buff), static_cast<size_t>(size));
  }
  return nullptr;
}

v_float32 Conversion::strToFloat32(const char* str){
  char* end;
  return std::strtof(str, &end);
}

v_float32 Conversion::strToFloat32(const booster::String& str, bool& success) {
  if(str == nullptr || str->empty()) {
    success = false;
    return 0;
  }
  char* end;
  v_float32 result = std::strtof(str->data(), &end);
  success = ((reinterpret_cast<v_buff_size>(end) - reinterpret_cast<v_buff_size>(str->data())) == static_cast<v_buff_size>(str->size()));
  return result;
}

v_float64 Conversion::strToFloat64(const char* str){
  char* end;
  return std::strtod(str, &end);
}

v_float64 Conversion::strToFloat64(const booster::String& str, bool& success) {
  if(str == nullptr || str->empty()) {
    success = false;
    return 0;
  }
  char* end;
  v_float64 result = std::strtod(str->data(), &end);
  success = ((reinterpret_cast<v_buff_size>(end) - reinterpret_cast<v_buff_size>(str->data())) == static_cast<v_buff_size>(str->size()));
  return result;
}

v_buff_size Conversion::float32ToCharSequence(v_float32 value, p_char8 data, v_buff_size n, const char* format) {
  return snprintf(reinterpret_cast<char*>(data), static_cast<size_t>(n), format, static_cast<double>(value));
}

v_buff_size Conversion::float64ToCharSequence(v_float64 value, p_char8 data, v_buff_size n, const char* format) {
  return snprintf(reinterpret_cast<char*>(data), static_cast<size_t>(n), format, value);
}

booster::String Conversion::float32ToStr(v_float32 value, const char* format) {
  v_char8 buff [100];
  auto size = float32ToCharSequence(value, &buff[0], 100, format);
  if(size > 0){
    return booster::String(reinterpret_cast<const char*>(&buff[0]), size);
  }
  return nullptr;
}

booster::String Conversion::float64ToStr(v_float64 value, const char* format) {
  v_char8 buff [100];
  auto size = float64ToCharSequence(value, &buff[0], 100, format);
  if(size > 0){
    return booster::String(reinterpret_cast<const char*>(&buff[0]), size);
  }
  return nullptr;
}

booster::String Conversion::boolToStr(bool value) {
  if(value){
    return booster::String("true", 4);
  } else {
    return booster::String("false", 5);
  }
}

bool Conversion::strToBool(const booster::String& str, bool& success) {
  if(str == "true"){
    success = true;
    return true;
  } else if(str == "false"){
    success = true;
    return false;
  }
  success = false;
  return false;
}
  
}}
