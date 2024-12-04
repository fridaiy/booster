

#ifndef booster_web_mime_multipart_Part_hpp
#define booster_web_mime_multipart_Part_hpp

#include "booster/data/share/LazyStringMap.hpp"
#include "booster/data/resource/Resource.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {


class Part {
public:

  typedef booster::data::share::LazyStringMultimap<booster::data::share::StringKeyLabelCI> Headers;
private:
  booster::String m_name;
  booster::String m_filename;
  Headers m_headers;
  std::shared_ptr<data::resource::Resource> m_payload;
private:
  const char* m_tagName;
  std::shared_ptr<booster::base::Countable> m_tagObject;
public:


  Part() = default;


  Part(const Headers& headers, const std::shared_ptr<data::resource::Resource>& payload = nullptr);


  void setPayload(const std::shared_ptr<data::resource::Resource>& payload);


  std::shared_ptr<data::resource::Resource> getPayload();


  booster::String getName() const;


  booster::String getFilename() const;


  const Headers& getHeaders() const;


  booster::String getHeader(const booster::data::share::StringKeyLabelCI& headerName) const;


  void putHeader(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value);


  bool putHeaderIfNotExists(const booster::data::share::StringKeyLabelCI& key, const booster::data::share::StringKeyLabel& value);


  void setTag(const char* tagName, const std::shared_ptr<booster::base::Countable>& tagObject);


  const char* getTagName();


  std::shared_ptr<booster::base::Countable> getTagObject();


  void clearTag();

};

}}}}

#endif // booster_web_mime_multipart_Part_hpp
