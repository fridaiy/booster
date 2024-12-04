

#ifndef booster_web_mime_multipart_PartList_hpp
#define booster_web_mime_multipart_PartList_hpp

#include "Multipart.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {


class PartList : public Multipart {
private:
  std::unordered_map<booster::String, std::list<std::shared_ptr<Part>>> m_namedParts;
  bool m_readIteratorInitialized;
  std::list<std::shared_ptr<Part>> m_parts;
  std::list<std::shared_ptr<Part>>::const_iterator m_iterator;
public:


  PartList(const booster::String& boundary);


  PartList(const Headers& requestHeaders);


  static std::shared_ptr<PartList> createSharedWithRandomBoundary(v_int32 boundarySize = 15);


  std::shared_ptr<Part> readNextPart(async::Action& action) override;


  void writeNextPart(const std::shared_ptr<Part>& part, async::Action& action) override;


  std::shared_ptr<Part> getNamedPart(const booster::String& name);


  std::list<std::shared_ptr<Part>> getNamedParts(const booster::String& name);


  const std::list<std::shared_ptr<Part>>& getAllParts();


  v_int64 count();

};

}}}}

#endif //booster_web_mime_multipart_PartList_hpp
