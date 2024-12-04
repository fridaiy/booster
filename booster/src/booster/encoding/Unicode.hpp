

#ifndef booster_encoding_Unicode_hpp
#define booster_encoding_Unicode_hpp

#include "booster/Environment.hpp"

namespace booster { namespace encoding {


class Unicode {
public:

  static v_buff_size getUtf8CharSequenceLength(v_char8 firstByte);


  static v_buff_size getUtf8CharSequenceLengthForCode(v_uint32 code);


  static v_int32 encodeUtf8Char(const char* sequence, v_buff_size& length);


  static v_buff_size decodeUtf8Char(v_int32 code, p_char8 buffer);


  static void codeToUtf16SurrogatePair(v_int32 code, v_int16& high, v_int16& low);


  static v_int32 utf16SurrogatePairToCode(v_int16 high, v_int16 low);
};
  
}}

#endif
