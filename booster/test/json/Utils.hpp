

#ifndef booster_json_Utils_hpp
#define booster_json_Utils_hpp

#include "booster/utils/parser/Caret.hpp"
#include "booster/Types.hpp"

#include <string>

namespace booster { namespace json {


class Utils {
public:

  static constexpr v_uint32 FLAG_ESCAPE_SOLIDUS = 1;
  static constexpr v_uint32 FLAG_ESCAPE_UTF8CHAR = 2;

  static constexpr v_uint32 FLAG_ESCAPE_ALL = FLAG_ESCAPE_SOLIDUS | FLAG_ESCAPE_UTF8CHAR;

public:


  static constexpr v_int64 ERROR_CODE_INVALID_ESCAPED_CHAR = 1;


  static constexpr v_int64 ERROR_CODE_INVALID_SURROGATE_PAIR = 2;


  static constexpr v_int64 ERROR_CODE_PARSER_QUOTE_EXPECTED = 3;


  static constexpr v_char8 JSON_DECIMAL_SEPARATOR = '.';

public:
  typedef booster::String String;
  typedef booster::utils::parser::Caret ParsingCaret;
private:
  static v_buff_size escapeUtf8Char(const char* sequence, p_char8 buffer);
  static v_buff_size calcEscapedStringSize(const char* data, v_buff_size size, v_buff_size& safeSize, v_uint32 flags);
  static v_buff_size calcUnescapedStringSize(const char* data, v_buff_size size, v_int64& errorCode, v_buff_size& errorPosition);
  static void unescapeStringToBuffer(const char* data, v_buff_size size, p_char8 resultData);
  static const char* preparseString(ParsingCaret& caret, v_buff_size& size);
public:


  static String escapeString(const char* data, v_buff_size size, v_uint32 flags = FLAG_ESCAPE_ALL);


  static String unescapeString(const char* data, v_buff_size size, v_int64& errorCode, v_buff_size& errorPosition);


  static std::string unescapeStringToStdString(const char* data, v_buff_size size, v_int64& errorCode, v_buff_size& errorPosition);


  static String parseString(ParsingCaret& caret);


  static std::string parseStringToStdString(ParsingCaret& caret);


  static bool findDecimalSeparatorInCurrentNumber(ParsingCaret& caret);
  
};
  
}}

#endif
