

#ifndef booster_web_mime_multipart_StatefulParser_hpp
#define booster_web_mime_multipart_StatefulParser_hpp

#include "booster/data/stream/BufferStream.hpp"
#include "booster/data/share/LazyStringMap.hpp"
#include "booster/Types.hpp"

#include <unordered_map>

namespace booster { namespace web { namespace mime { namespace multipart {


class StatefulParser {
private:
  static constexpr v_int32 STATE_BOUNDARY = 0;
  static constexpr v_int32 STATE_AFTER_BOUNDARY = 1;
  static constexpr v_int32 STATE_HEADERS = 2;
  static constexpr v_int32 STATE_DATA = 3;
  static constexpr v_int32 STATE_DONE = 4;
private:
  static constexpr v_uint32 HEADERS_SECTION_END = ('\r' << 24) | ('\n' << 16) | ('\r' << 8) | ('\n');
private:

  typedef booster::data::share::LazyStringMultimap<booster::data::share::StringKeyLabelCI> Headers;
public:


  class Listener {
  public:

    typedef booster::data::share::LazyStringMultimap<booster::data::share::StringKeyLabelCI> Headers;
  public:


    virtual ~Listener() = default;


    virtual void onPartHeaders(const Headers& partHeaders) = 0;


    virtual void onPartData(const char* data, v_buff_size size) = 0;

  };

public:


  class AsyncListener {
  public:

    typedef booster::data::share::LazyStringMultimap<booster::data::share::StringKeyLabelCI> Headers;
  public:


    virtual ~AsyncListener() = default;


    virtual async::CoroutineStarter onPartHeadersAsync(const Headers& partHeaders) = 0;


    virtual async::CoroutineStarter onPartDataAsync(const char* data, v_buff_size size) = 0;

  };

private:

  class ListenerCall {
  public:

    static constexpr v_int32 CALL_NONE = 0;
    static constexpr v_int32 CALL_ON_HEADERS = 1;
    static constexpr v_int32 CALL_ON_DATA = 2;

  public:

    ListenerCall()
      : callType(CALL_NONE)
      , data(nullptr)
      , size(0)
    {}

    v_int32 callType;
    const char* data;
    v_io_size size;

    void setOnHeadersCall();
    void setOnDataCall(const char* pData, v_buff_size pSize);

    void call(StatefulParser* parser);
    async::CoroutineStarter callAsync(StatefulParser* parser);

    explicit operator bool() const;

  };

private:

  v_int32 m_state;
  v_int64 m_currPartIndex;
  v_buff_size m_currBoundaryCharIndex;
  bool m_checkForBoundary;
  bool m_finishingBoundary;
  bool m_readingBody;

  v_uint32 m_headerSectionEndAccumulator;

  booster::String m_firstBoundarySample;
  booster::String m_nextBoundarySample;


  data::stream::BufferOutputStream m_headersBuffer;


  v_buff_size m_maxPartHeadersSize;

  std::shared_ptr<Listener> m_listener;
  std::shared_ptr<AsyncListener> m_asyncListener;

private:

  void parseHeaders(Headers& headers);

private:

  ListenerCall parseNext_Boundary(data::buffer::InlineWriteData& inlineData);
  void         parseNext_AfterBoundary(data::buffer::InlineWriteData& inlineData);
  ListenerCall parseNext_Headers(data::buffer::InlineWriteData& inlineData);
  ListenerCall parseNext_Data(data::buffer::InlineWriteData& inlineData);

public:


  StatefulParser(const booster::String& boundary,
                 const std::shared_ptr<Listener>& listener,
                 const std::shared_ptr<AsyncListener>& asyncListener);


  void parseNext(data::buffer::InlineWriteData& inlineData, async::Action& action);


  bool finished();

};

}}}}

#endif // booster_web_mime_multipart_StatefulParser_hpp
