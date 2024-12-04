

#ifndef booster_web_mime_multipart_Reader_hpp
#define booster_web_mime_multipart_Reader_hpp

#include "PartReader.hpp"
#include "Multipart.hpp"
#include "StatefulParser.hpp"

#include <unordered_map>

namespace booster { namespace web { namespace mime { namespace multipart {

class Reader; // FWD


typedef std::unordered_map<booster::String, std::shared_ptr<PartReader>> PartReadersMap;


class PartsParser : public StatefulParser::Listener {
  friend Reader;
private:
  PartReadersMap m_readers;
  std::shared_ptr<PartReader> m_defaultReader;
  std::shared_ptr<PartReader> m_currReader;
  Multipart* m_multipart;
  std::shared_ptr<Part> m_currPart;
public:


  PartsParser(Multipart* multipart);

  PartsParser(Multipart* multipart, const PartReadersMap& readersMap);

  void onPartHeaders(const Headers& partHeaders) override;

  void onPartData(const char* data, v_buff_size size) override;

  void setPartReader(const booster::String& partName, const std::shared_ptr<PartReader>& reader);

  void setDefaultPartReader(const std::shared_ptr<PartReader>& reader);

};

class AsyncReader; // FWD


typedef std::unordered_map<booster::String, std::shared_ptr<AsyncPartReader>> AsyncPartReadersMap;


class AsyncPartsParser : public StatefulParser::AsyncListener {
  friend AsyncReader;
private:
  async::CoroutineStarter onPartDone(const std::shared_ptr<Part>& part);
private:
  AsyncPartReadersMap m_readers;
  std::shared_ptr<AsyncPartReader> m_defaultReader;
  std::shared_ptr<AsyncPartReader> m_currReader;
  Multipart* m_multipart;
  std::shared_ptr<Part> m_currPart;
public:


  AsyncPartsParser(Multipart* multipart);

  AsyncPartsParser(Multipart* multipart, const AsyncPartReadersMap& readersMap);

  async::CoroutineStarter onPartHeadersAsync(const Headers& partHeaders) override;

  async::CoroutineStarter onPartDataAsync(const char* data, v_buff_size size) override;

  void setPartReader(const booster::String& partName, const std::shared_ptr<AsyncPartReader>& reader);

  void setDefaultPartReader(const std::shared_ptr<AsyncPartReader>& reader);

};


class Reader : public booster::data::stream::WriteCallback {
private:
  std::shared_ptr<PartsParser> m_partsParser;
  StatefulParser m_parser;
public:


  Reader(Multipart* multipart);

  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;


  void setPartReader(const booster::String& partName, const std::shared_ptr<PartReader>& reader);


  void setDefaultPartReader(const std::shared_ptr<PartReader>& reader);

};


class AsyncReader : public booster::data::stream::WriteCallback {
private:
  std::shared_ptr<AsyncPartsParser> m_partsParser;
  StatefulParser m_parser;
  std::shared_ptr<Multipart> m_multipart;
public:


  AsyncReader(const std::shared_ptr<Multipart>& multipart);

  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;


  void setPartReader(const booster::String& partName, const std::shared_ptr<AsyncPartReader>& reader);


  void setDefaultPartReader(const std::shared_ptr<AsyncPartReader>& reader);

};

}}}}

#endif // booster_web_mime_multipart_Reader_hpp
