

#include "Url.hpp"

#include <cstdlib>

namespace booster { namespace network {

booster::String Url::Parser::parseScheme(booster::utils::parser::Caret& caret) {
  v_buff_size pos0 = caret.getPosition();
  caret.findChar(':');
  v_buff_size size = caret.getPosition() - pos0;
  if(size > 0) {
    std::unique_ptr<v_char8[]> buff(new v_char8[static_cast<unsigned long>(size)]);
    std::memcpy(buff.get(), &caret.getData()[pos0], static_cast<size_t>(size));
    utils::String::lowerCase_ASCII(buff.get(), size);
    return booster::String(reinterpret_cast<const char*>(buff.get()), size);
  }
  return nullptr;
}

Url::Authority Url::Parser::parseAuthority(booster::utils::parser::Caret& caret) {
  
  const char* data = caret.getData();
  v_buff_size pos0 = caret.getPosition();
  v_buff_size pos = pos0;
  
  v_buff_size hostPos = pos0;
  v_buff_size atPos = -1;
  v_buff_size portPos = -1;
  
  while (pos < caret.getDataSize()) {
    v_char8 a = static_cast<v_char8>(data[pos]);
    if(a == '@') {
      atPos = pos;
      pos ++;
      hostPos = pos;
    } else if(a == ':') {
      pos ++;
      portPos = pos; // last ':' in authority proceeds port in case it goes after '@'
    } else if(a == '/' || a == '?' || a == '#') {
      if(pos == pos0) {
        return Url::Authority();
      }
      break;
    } else {
      pos ++;
    }
  }
  
  caret.setPosition(pos);
  
  Url::Authority result;
  
  if(atPos > -1) {
    result.userInfo = booster::String(&data[pos0], atPos - pos0);
  }
  
  if(portPos > hostPos) {
    result.host = booster::String(&data[hostPos], portPos - 1 - hostPos);
    char* end;
    result.port = static_cast<v_int32>(std::strtol(&data[portPos], &end, 10));
    bool success = ((reinterpret_cast<v_buff_size>(end) - reinterpret_cast<v_buff_size>(&data[portPos])) == pos - portPos);
    if(!success) {
      caret.setError("Invalid port string");
    }
  } else {
    result.host = booster::String(&data[hostPos], pos - pos0);
  }
  
  return result;
  
}

booster::String Url::Parser::parsePath(booster::utils::parser::Caret& caret) {
  auto label = caret.putLabel();
  caret.findCharFromSet("?#", 2);
  if(label.getSize() > 0) {
    return label.toString();
  }
  return nullptr;
}

void Url::Parser::parseQueryParams(Url::Parameters& params, booster::utils::parser::Caret& caret) {

  if(caret.findChar('?')) {

    do {
      caret.inc();
      auto nameLabel = caret.putLabel();
      v_buff_size charFound = caret.findCharFromSet("=&");
      if(charFound == '=') {
        nameLabel.end();
        caret.inc();
        auto valueLabel = caret.putLabel();
        caret.findChar('&');
        params.put_LockFree(StringKeyLabel(caret.getDataMemoryHandle(), nameLabel.getData(), nameLabel.getSize()),
                            StringKeyLabel(caret.getDataMemoryHandle(), valueLabel.getData(), valueLabel.getSize()));
      } else {
        params.put_LockFree(StringKeyLabel(caret.getDataMemoryHandle(), nameLabel.getData(), nameLabel.getSize()), "");
      }
    } while (caret.canContinueAtChar('&'));

  }
  
}

void Url::Parser::parseQueryParams(Url::Parameters& params, const booster::String& str) {
  booster::utils::parser::Caret caret(str.getPtr());
  parseQueryParams(params, caret);
}

Url::Parameters Url::Parser::parseQueryParams(booster::utils::parser::Caret& caret) {
  Url::Parameters params;
  parseQueryParams(params, caret);
  return params;
}

Url::Parameters Url::Parser::parseQueryParams(const booster::String& str) {
  Url::Parameters params;
  parseQueryParams(params, str);
  return params;
}

Url Url::Parser::parseUrl(booster::utils::parser::Caret& caret) {

  Url result;

  if(caret.findChar(':')) {
    if(caret.canContinueAtChar(':', 1) && !caret.isAtText("//", 2, true)) {
      caret.setPosition(0);
    } else {
      caret.setPosition(0);
      result.scheme = parseScheme(caret);
      caret.canContinueAtChar(':', 1);
    }
  } else {
    caret.setPosition(0);
  }

  caret.isAtText("//", 2, true);

  if(!caret.isAtChar('/')) {
    result.authority = parseAuthority(caret);
  }

  result.path = parsePath(caret);
  result.queryParams = parseQueryParams(caret);

  return result;
}

Url Url::Parser::parseUrl(const booster::String& str) {
  booster::utils::parser::Caret caret(str);
  return parseUrl(caret);
}
  
}}
