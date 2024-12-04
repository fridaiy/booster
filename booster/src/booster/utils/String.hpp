

#ifndef booster_utils_String_hpp
#define booster_utils_String_hpp

#include "booster/Environment.hpp"

namespace booster { namespace utils {


class String {
public:


  static v_buff_size compare(const void* data1, v_buff_size size1, const void* data2, v_buff_size size2);


  static v_buff_size compareCI_ASCII(const void* data1, v_buff_size size1, const void* data2, v_buff_size size2);


  static void lowerCase_ASCII(void* data, v_buff_size size);


  static void upperCase_ASCII(void* data, v_buff_size size);

};

}}

#endif // booster_utils_String_hpp
