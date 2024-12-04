

#ifndef booster_data_buffer_IOBuffer_hpp
#define booster_data_buffer_IOBuffer_hpp

#include "booster/base/Countable.hpp"

namespace booster { namespace data{ namespace buffer {


class IOBuffer : public booster::base::Countable {
public:

  static const v_buff_size BUFFER_SIZE;
private:
  p_char8 m_entry;
public:

  IOBuffer();
public:


  static std::shared_ptr<IOBuffer> createShared();


  ~IOBuffer() override;


  void* getData();


  v_buff_size getSize();
  
};
  
}}}

#endif
