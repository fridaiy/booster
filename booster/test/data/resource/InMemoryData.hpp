

#ifndef booster_data_resource_InMemoryData_hpp
#define booster_data_resource_InMemoryData_hpp

#include "./Resource.hpp"
#include "booster/data/stream/BufferStream.hpp"

namespace booster { namespace data { namespace resource {

class InMemoryData : public Resource {
private:

  struct DataHandle {

    booster::String data;

    DataHandle(const booster::String& pData)
      : data(pData)
    {}

  };

  struct OutputDataHandle {

    std::shared_ptr<DataHandle> dataHandle;
    booster::String data;
    data::stream::BufferOutputStream* stream;

    ~OutputDataHandle();

  };

private:
  std::shared_ptr<DataHandle> m_handle;
public:


  InMemoryData() = default;


  InMemoryData(const booster::String& data);


  std::shared_ptr<data::stream::OutputStream> openOutputStream() override;


  std::shared_ptr<data::stream::InputStream> openInputStream() override;


  booster::String getInMemoryData() override;


  v_int64 getKnownSize() override;


  booster::String getLocation() override;

};

}}}

#endif //booster_data_resource_InMemoryData_hpp
