

#include "String.hpp"
#include <cstring>

namespace booster { namespace utils {

v_buff_size String::compare(const void* data1, v_buff_size size1, const void* data2, v_buff_size size2) {

  if(data1 == data2) return 0;
  if(data1 == nullptr) return -1;
  if(data2 == nullptr) return 1;

  if(size1 < size2) {
    auto res = std::memcmp(data1, data2, static_cast<size_t>(size1));
    if(res == 0) return -1;
    return res;
  }

  if(size1 > size2) {
    auto res = std::memcmp(data1, data2, static_cast<size_t>(size2));
    if(res == 0) return 1;
    return res;
  }

  return std::memcmp(data1, data2, static_cast<size_t>(size1));

}

v_buff_size String::compareCI_ASCII(const void* data1, v_buff_size size1, const void* data2, v_buff_size size2) {

  if(data1 == data2) return 0;
  if(data1 == nullptr) return -1;
  if(data2 == nullptr) return 1;

  auto d1 = reinterpret_cast<const char*>(data1);
  auto d2 = reinterpret_cast<const char*>(data2);

  v_buff_size size = size1;
  if(size2 < size1) size = size2;

  for(v_buff_size i = 0; i < size; i ++) {

    v_char8 a = static_cast<v_char8>(d1[i]);
    v_char8 b = static_cast<v_char8>(d2[i]);

    if(a >= 'A' && a <= 'Z') a |= 32;
    if(b >= 'A' && b <= 'Z') b |= 32;

    if(a != b) {
      return static_cast<v_buff_size>(a) - static_cast<v_buff_size>(b);
    }

  }

  if(size1 < size2) return -1;
  if(size1 > size2) return  1;

  return 0;

}

void String::lowerCase_ASCII(void* data, v_buff_size size) {
  for(v_buff_size i = 0; i < size; i++) {
    v_char8 a = (reinterpret_cast<p_char8>(data))[i];
    if(a >= 'A' && a <= 'Z') (reinterpret_cast<p_char8>(data))[i] = a | 32;
  }
}

void String::upperCase_ASCII(void* data, v_buff_size size) {
  for(v_buff_size i = 0; i < size; i++) {
    v_char8 a = (reinterpret_cast<p_char8>(data))[i];
    if(a >= 'a' && a <= 'z') (reinterpret_cast<p_char8>(data))[i] = a & 223;
  }
}

}}
