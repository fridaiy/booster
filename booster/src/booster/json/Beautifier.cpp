

#include "Beautifier.hpp"

#include "booster/data/stream/BufferStream.hpp"

namespace booster { namespace json {

Beautifier::Beautifier(ConsistentOutputStream* outputStream, const booster::String& indent, const booster::String& newLine)
  : m_outputStream(outputStream)
  , m_indent(indent)
  , m_newLine(newLine)
  , m_level(0)
  , m_wantIndent(false)
  , m_isCharEscaped(false)
  , m_isInString(false)
{}

void Beautifier::writeIndent(ConsistentOutputStream* outputStream) {
  outputStream->writeSimple(m_newLine->data(), static_cast<v_buff_size>(m_newLine->size()));
  for(v_int32 i = 0; i < m_level; i ++ ) {
    outputStream->writeSimple(m_indent->data(), static_cast<v_buff_size>(m_indent->size()));
  }
}

v_io_size Beautifier::write(const void *data, v_buff_size count, async::Action& action) {

  (void) action;

  booster::data::stream::BufferOutputStream buffer(count);

  for(v_buff_size i = 0; i < count; i ++) {

    v_char8 c = static_cast<v_char8>((reinterpret_cast<const char*>(data)) [i]);

    if(m_isCharEscaped) {
      m_isCharEscaped = false;
      buffer.writeCharSimple(c);
      continue;
    }

    if(m_isInString) {
      if(c == '\\') {
        m_isCharEscaped = true;
      } else if(c == '"') {
        m_isInString = false;
      }
      buffer.writeCharSimple(c);
      continue;
    }

    switch(c) {

      case '{': {
        if(m_wantIndent) {
          writeIndent(&buffer);
        }
        m_level ++;
        m_wantIndent = true;
        buffer.writeCharSimple('{');
        break;
      }

      case '}': {
        m_level --;
        if(!m_wantIndent) {
          writeIndent(&buffer);
        }
        buffer.writeCharSimple('}');
        m_wantIndent = false;
        break;
      }

      case '[': {
        if(m_wantIndent) {
          writeIndent(&buffer);
        }
        m_level ++;
        m_wantIndent = true;
        buffer.writeCharSimple('[');
        break;
      }

      case ']': {
        m_level --;
        if(!m_wantIndent) {
          writeIndent(&buffer);
        }
        buffer.writeCharSimple(']');
        m_wantIndent = false;
        break;
      }

      case ',': {
        m_wantIndent = true;
        buffer.writeCharSimple(',');
        break;
      }

      case '"': {
        if(m_wantIndent) {
          writeIndent(&buffer);
        }
        buffer.writeCharSimple('"');
        m_wantIndent = false;
        m_isInString = true;
        break;
      }

      case ':': {
        buffer.writeSimple(": ", 2);
        break;
      }

      default:
        if(m_wantIndent) {
          writeIndent(&buffer);
          m_wantIndent = false;
        }
        buffer.writeCharSimple(c);

    }

  }

  return m_outputStream->writeSimple(buffer.getData(), buffer.getCurrentPosition());

}


void Beautifier::setOutputStreamIOMode(IOMode ioMode) {
  return m_outputStream->setOutputStreamIOMode(ioMode);
}


Beautifier::IOMode Beautifier::getOutputStreamIOMode() {
  return m_outputStream->getOutputStreamIOMode();
}

Beautifier::Context& Beautifier::getOutputStreamContext() {
  return m_outputStream->getOutputStreamContext();
}

}}
