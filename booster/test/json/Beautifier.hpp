

#ifndef booster_json_Beautifier_hpp
#define booster_json_Beautifier_hpp

#include "booster/data/stream/Stream.hpp"

namespace booster { namespace json {


class Beautifier : public booster::data::stream::ConsistentOutputStream {
public:
  typedef booster::data::stream::ConsistentOutputStream ConsistentOutputStream;
  typedef booster::data::stream::IOMode IOMode;
  typedef booster::data::stream::Context Context;
private:
  ConsistentOutputStream* m_outputStream;
  booster::String m_indent;
  booster::String m_newLine;
private:
  v_int32 m_level;
  bool m_wantIndent;
  bool m_isCharEscaped;
  bool m_isInString;
private:
  void writeIndent(ConsistentOutputStream* outputStream);
public:
  

  Beautifier(ConsistentOutputStream* outputStream, const booster::String& indent, const booster::String& newLine);


  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;


  void setOutputStreamIOMode(IOMode ioMode) override;


  IOMode getOutputStreamIOMode() override;


  Context& getOutputStreamContext() override;

};

}}

#endif // booster_json_Beautifier_hpp
