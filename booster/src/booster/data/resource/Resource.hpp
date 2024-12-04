

#ifndef booster_data_resource_Resource_hpp
#define booster_data_resource_Resource_hpp

#include "booster/data/stream/Stream.hpp"

namespace booster { namespace data { namespace resource {


class Resource : public booster::base::Countable {
public:


  virtual ~Resource() override = default;


  virtual std::shared_ptr<data::stream::OutputStream> openOutputStream() = 0;


  virtual std::shared_ptr<data::stream::InputStream> openInputStream() = 0;


  virtual booster::String getInMemoryData() = 0;


  virtual v_int64 getKnownSize() = 0;


  virtual booster::String getLocation() = 0;

};

}}}

#endif //booster_data_resource_Resource_hpp
