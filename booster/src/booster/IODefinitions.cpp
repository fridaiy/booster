

#include "IODefinitions.hpp"

#if defined(WIN32) || defined(_WIN32)
  #include <winsock2.h>
#endif

namespace booster {

bool isValidIOHandle(v_io_handle handle) {

#if defined(WIN32) || defined(_WIN32)
  return handle != INVALID_SOCKET;
#else
  return handle >= 0;
#endif

}

}
