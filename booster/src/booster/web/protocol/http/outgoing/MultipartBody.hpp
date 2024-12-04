

#ifndef booster_web_protocol_http_outgoing_MultipartBody_hpp
#define booster_web_protocol_http_outgoing_MultipartBody_hpp

#include "./Body.hpp"

#include "booster/web/mime/multipart/Multipart.hpp"
#include "booster/data/stream/BufferStream.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace outgoing {

class MultipartBody : public booster::base::Countable, public Body {
public:


  typedef booster::web::mime::multipart::Multipart Multipart;


  typedef booster::web::mime::multipart::Part Part;
private:


  typedef booster::data::stream::ReadCallback ReadCallback;

private:

  static constexpr v_int32 STATE_BOUNDARY = 0;
  static constexpr v_int32 STATE_HEADERS = 1;
  static constexpr v_int32 STATE_BODY = 2;
  static constexpr v_int32 STATE_INC_PART = 3;
  static constexpr v_int32 STATE_ROUND = 4; // number of possible states. used to round the state.
  static constexpr v_int32 STATE_FINISHED = 5;

private:

  class PartIterator {
  private:
    std::shared_ptr<Multipart> m_multipart;
    std::shared_ptr<Part> m_part;
    std::shared_ptr<data::stream::InputStream> m_partInputStream;
    bool m_isFirst;
    bool m_initialized;
  public:

    PartIterator(const std::shared_ptr<Multipart>& multipart)
      : m_multipart(multipart)
      , m_part(nullptr)
      , m_partInputStream(nullptr)
      , m_isFirst(true)
      , m_initialized(false)
    {}

    void init(async::Action& action) {
      if(!m_initialized) {
        m_part = m_multipart->readNextPart(action);
        m_partInputStream = nullptr;
        if(m_part) {
          auto payload = m_part->getPayload();
          if (payload) {
            m_partInputStream = payload->openInputStream();
          }
        }
        m_initialized = true;
      }
    }

    void inc(async::Action& action) {
      m_part = m_multipart->readNextPart(action);
      m_partInputStream = nullptr;
      if(m_part) {
        auto payload = m_part->getPayload();
        if (payload) {
          m_partInputStream = payload->openInputStream();
        }
      }
      m_isFirst = false;
    }

    bool finished() {
      return m_part == nullptr;
    }

    bool isFirst() {
      return m_isFirst;
    }

    std::shared_ptr<Part> get() {
      return m_part;
    }

    std::shared_ptr<data::stream::InputStream> getPartInputStream() {
      return m_partInputStream;
    }

  };

private:

  static v_io_size readBoundary(const std::shared_ptr<Multipart>& multipart,
                                PartIterator& iterator,
                                data::stream::BufferInputStream& readStream,
                                void *buffer,
                                v_buff_size count);

  static v_io_size readHeaders(const std::shared_ptr<Multipart>& multipart,
                               PartIterator& iterator,
                               data::stream::BufferInputStream& readStream,
                               void *buffer,
                               v_buff_size count);

private:
  std::shared_ptr<Multipart> m_multipart;
  booster::String m_contentType;
private:
  PartIterator m_iterator;
  v_int32 m_state;
  booster::data::stream::BufferInputStream m_readStream;
  bool m_flushParts;
private:
  v_io_size readBody(void *buffer, v_buff_size count, async::Action& action);
  v_io_size incPart(async::Action& action);
public:


  MultipartBody(const std::shared_ptr<Multipart>& multipart,
                const booster::String& contentType = "multipart/form-data",
                bool flushParts = false);


  v_io_size read(void *buffer, v_buff_size count, async::Action& action) override;


  void declareHeaders(Headers& headers) override;


  p_char8 getKnownData() override;


  v_int64 getKnownSize() override;

};

}}}}}

#endif // booster_web_protocol_http_outgoing_MultipartBody_hpp
