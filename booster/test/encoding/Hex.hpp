

#ifndef booster_encoding_Hex_hpp
#define booster_encoding_Hex_hpp

#include "booster/data/stream/Stream.hpp"
#include "booster/Types.hpp"

namespace booster { namespace encoding {


class Hex {
public:
  static const char* ALPHABET_UPPER;
  static const char* ALPHABET_LOWER;
public:

  static constexpr v_int32 ERROR_UNKNOWN_SYMBOL = 1;
public:


  class DecodingError : public std::runtime_error {
  public:


    DecodingError(const char* message)
      :std::runtime_error(message)
    {}

  };

public:


  static void writeUInt16(v_uint16 value, p_char8 buffer);


  static void writeUInt32(v_uint32 value, p_char8 buffer);


  static v_int32 readUInt16(const char* buffer, v_uint16& value);


  static v_int32 readUInt32(const char* buffer, v_uint32& value);


  static void encode(data::stream::ConsistentOutputStream* stream,
                     const void* data, v_buff_size size,
                     const char* alphabet = ALPHABET_UPPER);


  static booster::String encode(const booster::String& data, const char* alphabet = ALPHABET_UPPER);


  static void decode(data::stream::ConsistentOutputStream* stream,
                     const void* data, v_buff_size size, bool allowSeparators = false);


  static booster::String decode(const booster::String& data, bool allowSeparators = false);
  
};
  
}}

#endif
