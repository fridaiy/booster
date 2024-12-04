

#ifndef booster_web_protocol_http_encoding_ProviderCollection_hpp
#define booster_web_protocol_http_encoding_ProviderCollection_hpp

#include "EncoderProvider.hpp"
#include "booster/data/share/MemoryLabel.hpp"
#include <unordered_map>
#include <unordered_set>

namespace booster { namespace web { namespace protocol { namespace http { namespace encoding {


class ProviderCollection {
private:
  std::unordered_map<data::share::StringKeyLabelCI, std::shared_ptr<EncoderProvider>> m_providers;
public:


  void add(const std::shared_ptr<EncoderProvider>& provider);


  std::shared_ptr<EncoderProvider> get(const data::share::StringKeyLabelCI& encoding) const;


  std::shared_ptr<EncoderProvider> get(const std::unordered_set<data::share::StringKeyLabelCI>& encodings) const;

};

}}}}}

#endif // booster_web_protocol_http_encoding_ProviderCollection_hpp
