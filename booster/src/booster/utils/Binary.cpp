

#include "Binary.hpp"

namespace booster { namespace utils {

v_int64 Binary::nextP2(v_int64 v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  v++;
  return v;
}

}}
