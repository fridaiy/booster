

#include "Url.hpp"

#include "booster/data/stream/BufferStream.hpp"

namespace booster { namespace encoding {

Url::Config::Config() {

  disallowCharRange(0, 255);

  allowCharRange('0', '9');
  allowCharRange('a', 'z');
  allowCharRange('A', 'Z');

  allowChar('-');
  allowChar('.');
  allowChar('_');
  allowChar('~');

}

void Url::Config::allowChar(v_char8 c) {
  allowedChars[c] = true;
}

void Url::Config::allowCharRange(v_char8 from, v_char8 to) {
  for(v_int32 i = from; i <= to; i++) {
    allowedChars[i] = true;
  }
}

void Url::Config::disallowChar(v_char8 c) {
  allowedChars[c] = false;
}

void Url::Config::disallowCharRange(v_char8 from, v_char8 to) {
  for(v_int32 i = from; i <= to; i++) {
    allowedChars[i] = false;
  }
}

void Url::encode(data::stream::ConsistentOutputStream *stream, const void *data, v_buff_size size, const Config& config) {

  auto pdata = reinterpret_cast<const char*>(data);

  for(v_buff_size i = 0; i < size; i++) {
    v_char8 c = static_cast<v_char8>(pdata[i]);
    if(config.allowedChars[c]) {
      stream->writeCharSimple(c);
    } else if(c == ' ' && config.spaceToPlus) {
      stream->writeCharSimple('+');
    } else {
        stream->writeCharSimple('%');
        Hex::encode(stream, pdata + i, 1, config.hexAlphabet);
    }
  }

}

void Url::decode(data::stream::ConsistentOutputStream* stream, const void* data, v_buff_size size) {

  auto pdata = reinterpret_cast<const char*>(data);
  v_buff_size i = 0;

  while (i < size) {

    v_char8 c = static_cast<v_char8>(pdata[i]);
    if(c == '%') {
      if(size - i > 1) {
        Hex::decode(stream, pdata + i + 1, 2);
        i += 3;
      } else {
        break;
      }
    } else if (c == '+') {
      stream->writeCharSimple(' ');
      i ++;
    } else {
        stream->writeCharSimple(c);
        i ++;
    }

  }

}

booster::String Url::encode(const booster::String& data, const Config& config) {
  if(!data) return nullptr;
  data::stream::BufferOutputStream stream(static_cast<v_buff_size>(data->size() * 3));
  encode(&stream, data->data(), static_cast<v_buff_size>(data->size()), config);
  return stream.toString();
}

booster::String Url::decode(const booster::String& data) {
  if(!data) return nullptr;
  data::stream::BufferOutputStream stream(static_cast<v_buff_size>(data->size()));
  decode(&stream, data->data(), static_cast<v_buff_size>(data->size()));
  return stream.toString();
}

}}
