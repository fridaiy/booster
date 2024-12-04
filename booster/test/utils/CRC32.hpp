

#ifndef booster_utils_CRC32_hpp
#define booster_utils_CRC32_hpp

#include "booster/Environment.hpp"

#include "booster/encoding/Hex.hpp"

namespace booster { namespace utils {


class CRC32 {
public:


  static const p_uint32 TABLE_04C11DB7;
public:

  static v_uint32 bitReverse(v_uint32 poly);
  

  static p_uint32 generateTable(v_uint32 poly);


  static v_uint32 calc(const void *buffer, v_buff_size size, v_uint32 crc = 0, v_uint32 initValue = 0xFFFFFFFF, v_uint32 xorOut = 0xFFFFFFFF, p_uint32 table = TABLE_04C11DB7);
  
};
    
}}

#endif
