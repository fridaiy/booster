

#include "ParsingError.hpp"

namespace booster { namespace utils { namespace parser {

ParsingError::ParsingError(const booster::String &message, v_int64 code, v_buff_size position)
  :std::runtime_error(*message)
  , m_message(message)
  , m_code(code)
  , m_position(position)
{}

booster::String ParsingError::getMessage() const {
  return m_message;
}

v_int64 ParsingError::getCode() const {
  return m_code;
}

v_buff_size ParsingError::getPosition() const {
  return m_position;
}

}}}
