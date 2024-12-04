

#ifndef booster_encoding_Base64_hpp
#define booster_encoding_Base64_hpp

#include "booster/Types.hpp"

namespace booster { namespace encoding {


class Base64 {
public:


  class DecodingError : public std::runtime_error {
  public:


    DecodingError(const char* message)
      :std::runtime_error(message)
    {}
    
  };
  
private:
  
  static v_char8 getAlphabetCharIndex(v_char8 a, const char* auxiliaryChars);
  
public:

  static const char* const ALPHABET_BASE64;


  static const char* const ALPHABET_BASE64_URL;


  static const char* const ALPHABET_BASE64_URL_SAFE;
  

  static const char* const ALPHABET_BASE64_AUXILIARY_CHARS;


  static const char* const ALPHABET_BASE64_URL_AUXILIARY_CHARS;


  static const char* const ALPHABET_BASE64_URL_SAFE_AUXILIARY_CHARS;


  static v_buff_size calcEncodedStringSize(v_buff_size size);


  static v_buff_size calcDecodedStringSize(const char* data, v_buff_size size, v_buff_size& base64StrLength, const char* auxiliaryChars = ALPHABET_BASE64_AUXILIARY_CHARS);


  static bool isBase64String(const char* data, v_buff_size size, const char* auxiliaryChars = ALPHABET_BASE64_AUXILIARY_CHARS);


  static booster::String encode(const void* data, v_buff_size size, const char* alphabet = ALPHABET_BASE64);


  static booster::String encode(const booster::String& data, const char* alphabet = ALPHABET_BASE64);


  static booster::String decode(const char* data, v_buff_size size, const char* auxiliaryChars = ALPHABET_BASE64_AUXILIARY_CHARS);
  

  static booster::String decode(const booster::String& data, const char* auxiliaryChars = ALPHABET_BASE64_AUXILIARY_CHARS);
  
};
  
}}

#endif
