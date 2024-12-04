

#ifndef booster_web_mime_multipart_Multipart_hpp
#define booster_web_mime_multipart_Multipart_hpp

#include "Part.hpp"
#include <list>

namespace booster { namespace web { namespace mime { namespace multipart {


typedef booster::data::share::LazyStringMultimap<booster::data::share::StringKeyLabelCI> Headers;


class Multipart {
private:
  booster::String m_boundary;
public:


  Multipart(const booster::String& boundary);


  virtual ~Multipart() = default;


  booster::String getBoundary();


  virtual std::shared_ptr<Part> readNextPart(async::Action& action) = 0;


  virtual void writeNextPart(const std::shared_ptr<Part>& part, async::Action& action) = 0;


  std::shared_ptr<Part> readNextPartSimple();


  void writeNextPartSimple(const std::shared_ptr<Part>& part);

public:


  static booster::String generateRandomBoundary(v_int32 boundarySize = 15);


  static booster::String parseBoundaryFromHeaders(const Headers& requestHeaders);

};



}}}}


#endif // booster_web_mime_multipart_Multipart_hpp
