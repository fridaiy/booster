

#ifndef booster_encoding_Url_hpp
#define booster_encoding_Url_hpp

#include "Hex.hpp"
#include "booster/data/stream/Stream.hpp"

namespace booster { namespace encoding {

class Url {
public:

  struct Config {

    bool spaceToPlus = false;
    const char* hexAlphabet = Hex::ALPHABET_UPPER;
    bool allowedChars[256];

    Config();

    void allowChar(v_char8 c);
    void allowCharRange(v_char8 from, v_char8 to);

    void disallowChar(v_char8 c);
    void disallowCharRange(v_char8 from, v_char8 to);

  };

public:

  static void encode(data::stream::ConsistentOutputStream* stream, const void* data, v_buff_size size, const Config& config);
  static void decode(data::stream::ConsistentOutputStream* stream, const void* data, v_buff_size size);

  static booster::String encode(const booster::String& data, const Config& config);
  static booster::String decode(const booster::String& data);

};

}}

#endif //booster_encoding_Url_hpp
