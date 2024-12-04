

#ifndef booster_data_buffer_Processor_hpp
#define booster_data_buffer_Processor_hpp

#include "booster/IODefinitions.hpp"
#include "booster/base/ObjectHandle.hpp"
#include <vector>

namespace booster { namespace data { namespace buffer {


struct InlineReadData {


  void* currBufferPtr;


  v_buff_size bytesLeft;


  InlineReadData();


  InlineReadData(void* data, v_buff_size size);


  void set(void* data, v_buff_size size);


  void inc(v_buff_size amount);


  void setEof();

};


struct InlineWriteData {


  const void* currBufferPtr;


  v_buff_size bytesLeft;


  InlineWriteData();


  InlineWriteData(const void* data, v_buff_size size);


  void set(const void* data, v_buff_size size);


  void inc(v_buff_size amount);


  void setEof();

};


class Processor {
public:


  enum Error : v_int32 {


    OK = 0,


    PROVIDE_DATA_IN = 1,


    FLUSH_DATA_OUT = 2,


    FINISHED = 3

    //
    // Other values are processor-specific errors. //
    //
  };

public:


  virtual ~Processor() = default;


  virtual v_io_size suggestInputStreamReadSize() = 0;


  virtual v_int32 iterate(data::buffer::InlineReadData& dataIn,
                          data::buffer::InlineReadData& dataOut) = 0;

};


class ProcessingPipeline : public Processor {
private:
  std::vector<base::ObjectHandle<Processor>> m_processors;
  std::vector<data::buffer::InlineReadData> m_intermediateData;
public:


  ProcessingPipeline(const std::vector<base::ObjectHandle<Processor>>& m_processors);


  v_io_size suggestInputStreamReadSize() override;


  v_int32 iterate(data::buffer::InlineReadData& dataIn,
                  data::buffer::InlineReadData& dataOut) override;

};

}}}

#endif // booster_data_buffer_Processor_hpp
