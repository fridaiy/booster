

#ifndef booster_utils_parser_ParsingError_hpp
#define booster_utils_parser_ParsingError_hpp

#include "booster/Types.hpp"

namespace booster { namespace utils { namespace parser {


class ParsingError : public std::runtime_error {
private:
  booster::String m_message;
  v_int64 m_code;
  v_buff_size m_position;
public:


  ParsingError(const booster::String &message, v_int64 code, v_buff_size position);


  booster::String getMessage() const;


  v_int64 getCode() const;


  v_buff_size getPosition() const;

};

}}}

#endif //booster_utils_parser_ParsingError_hpp
