

#ifndef booster_web_mime_ContentMappers_hpp
#define booster_web_mime_ContentMappers_hpp

#include "booster/web/protocol/http/Http.hpp"
#include "booster/data/mapping/ObjectMapper.hpp"

#include <shared_mutex>

namespace booster::web::mime {

class ContentMappers {
private:

  struct MatchedMapper {

    std::shared_ptr<data::mapping::ObjectMapper> mapper;
    v_float64 quality;

    bool operator < (const MatchedMapper& other) const {
      return quality > other.quality;
    }

  };

private:
  typedef std::unordered_map<data::share::StringKeyLabelCI, std::shared_ptr<data::mapping::ObjectMapper>> MappersBySubtypes;
private:
  std::pair<booster::String, booster::String> typeAndSubtype(const data::share::StringKeyLabelCI& contentType) const;
  std::shared_ptr<data::mapping::ObjectMapper> selectMapper(const protocol::http::HeaderValueData& values) const;
private:
  std::unordered_map<data::share::StringKeyLabelCI, MappersBySubtypes> m_index;
  std::unordered_map<data::share::StringKeyLabelCI, std::shared_ptr<data::mapping::ObjectMapper>> m_mappers;
  std::shared_ptr<data::mapping::ObjectMapper> m_defaultMapper;
  mutable std::shared_mutex m_mutex;
public:

  ContentMappers() = default;
  virtual ~ContentMappers() = default;

  void putMapper(const std::shared_ptr<data::mapping::ObjectMapper>& mapper);

  void setDefaultMapper(const booster::String& contentType);
  void setDefaultMapper(const std::shared_ptr<data::mapping::ObjectMapper>& mapper);

  std::shared_ptr<data::mapping::ObjectMapper> getMapper(const booster::String& contentType) const;
  std::shared_ptr<data::mapping::ObjectMapper> getDefaultMapper() const;

  std::shared_ptr<data::mapping::ObjectMapper> selectMapperForContent(const booster::String& contentTypeHeader) const;

  std::shared_ptr<data::mapping::ObjectMapper> selectMapper(const booster::String& acceptHeader) const;
  std::shared_ptr<data::mapping::ObjectMapper> selectMapper(const std::vector<booster::String>& acceptableContentTypes) const;

  void clear();

};

}

#endif //booster_web_mime_ContentMappers_hpp
