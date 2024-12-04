

#include "Address.hpp"

namespace booster { namespace network {

Address::Address(const booster::String& pHost, v_uint16 pPort, Family pFamily)
  : host(pHost)
  , port(pPort)
  , family(pFamily)
{}

}}
