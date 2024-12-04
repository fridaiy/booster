

#ifndef booster_IODefinitions_hpp
#define booster_IODefinitions_hpp

#include "booster/async/Error.hpp"
#include "booster/Types.hpp"

#if !defined(WIN32) && !defined(_WIN32)
#include <sys/socket.h>
#endif

namespace booster {


#if defined(WIN32) || defined(_WIN32)
  #if defined(_WIN64)
    typedef unsigned long long v_io_handle;
  #else
    typedef unsigned long v_io_handle;
  #endif
    typedef int v_sock_size;
    constexpr const v_io_handle INVALID_IO_HANDLE = v_io_handle (-1);
#else
  typedef int v_io_handle;
  typedef socklen_t v_sock_size;
  constexpr const v_io_handle INVALID_IO_HANDLE = (-1);
#endif


bool isValidIOHandle(v_io_handle handle);


typedef v_int64 v_io_size;


enum IOError : v_io_size {


  ZERO_VALUE = 0,


  BROKEN_PIPE = -1001,


  RETRY_READ = -1002,


  RETRY_WRITE = -1003,

};


class AsyncIOError : public booster::async::Error {
private:
  v_io_size m_code;
public:


  AsyncIOError(const char* what, v_io_size code)
    : booster::async::Error(what)
    , m_code(code)
  {}


  AsyncIOError(v_io_size code)
    : booster::async::Error("AsyncIOError")
    , m_code(code)
  {}


  v_io_size getCode() const {
    return m_code;
  }

};

}

#endif // booster_IODefinitions_hpp
