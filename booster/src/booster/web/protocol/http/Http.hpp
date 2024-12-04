

#ifndef booster_web_protocol_http_Http_hpp
#define booster_web_protocol_http_Http_hpp

#include "booster/network/tcp/Connection.hpp"

#include "booster/web/protocol/CommunicationError.hpp"

#include "booster/utils/parser/Caret.hpp"
#include "booster/data/share/LazyStringMap.hpp"
#include "booster/Types.hpp"

#include <unordered_map>
#include <unordered_set>

namespace booster { namespace web { namespace protocol { namespace http {


typedef booster::data::share::LazyStringMultimap<booster::data::share::StringKeyLabelCI> Headers;


typedef booster::data::share::LazyStringMultimap<booster::data::share::StringKeyLabel> QueryParams;


class Status{
public:


  static const Status CODE_100;// Continue


  static const Status CODE_101;// Switching


  static const Status CODE_102;// Processing


  static const Status CODE_200;// OK


  static const Status CODE_201;// Created


  static const Status CODE_202;// Accepted


  static const Status CODE_203;// Non-Authoritative Information


  static const Status CODE_204;// No Content


  static const Status CODE_205;// Reset Content


  static const Status CODE_206;// Partial Content


  static const Status CODE_207;// Multi-Status


  static const Status CODE_226;// IM Used


  static const Status CODE_300;// Multiple Choices


  static const Status CODE_301;// Moved Permanently


  static const Status CODE_302;// Moved Temporarily


  static const Status CODE_303;// See Other


  static const Status CODE_304;// Not Modified


  static const Status CODE_305;// Use Proxy


  static const Status CODE_306;// Reserved


  static const Status CODE_307;// Temporary Redirect


  static const Status CODE_400;// Bad Request


  static const Status CODE_401;// Unauthorized


  static const Status CODE_402;// Payment Required


  static const Status CODE_403;// Forbidden


  static const Status CODE_404;// Not Found


  static const Status CODE_405;// Method Not Allowed


  static const Status CODE_406;// Not Acceptable


  static const Status CODE_407;// Proxy Authentication Required


  static const Status CODE_408;// Request Timeout


  static const Status CODE_409;// Conflict


  static const Status CODE_410;// Gone


  static const Status CODE_411;// Length Required


  static const Status CODE_412;// Precondition Failed


  static const Status CODE_413;// Request Entity Too Large


  static const Status CODE_414;// Request-URI Too Large


  static const Status CODE_415;// Unsupported Media Type


  static const Status CODE_416;// Requested Range Not Satisfiable


  static const Status CODE_417;// Expectation Failed


  static const Status CODE_418;// I'm a teapot


  static const Status CODE_422;// Unprocessable Entity


  static const Status CODE_423;// Locked


  static const Status CODE_424;// Failed Dependency


  static const Status CODE_425;// Unordered Collection


  static const Status CODE_426;// Upgrade Required


  static const Status CODE_428;// Precondition Required


  static const Status CODE_429;// Too Many Requests


  static const Status CODE_431;// Request Header Fields Too Large


  static const Status CODE_434;// Requested host unavailable


  static const Status CODE_444;// Close connection withot sending headers


  static const Status CODE_449;// Retry With


  static const Status CODE_451;// Unavailable For Legal Reasons


  static const Status CODE_500;// Internal Server Error


  static const Status CODE_501;// Not Implemented


  static const Status CODE_502;// Bad Gateway


  static const Status CODE_503;// Service Unavailable


  static const Status CODE_504;// Gateway Timeout


  static const Status CODE_505;// HTTP Version Not Supported


  static const Status CODE_506;// Variant Also Negotiates


  static const Status CODE_507;// Insufficient Storage


  static const Status CODE_508;// Loop Detected


  static const Status CODE_509;// Bandwidth Limit Exceeded


  static const Status CODE_510;// Not Extended


  static const Status CODE_511;// Network Authentication Required


  Status()
    : code(0)
    , description(nullptr)
  {}


  Status(v_int32 pCode, const char* pDesc)
    : code(pCode)
    , description(pDesc)
  {}


  v_int32 code;


  const char* description;
  
  bool operator==(const Status& other) const {
    return this->code == other.code;
  }
  
  bool operator!=(const Status& other) const {
    return this->code != other.code;
  }
  
};


class HttpError : public protocol::ProtocolError<Status> {
private:
  Headers m_headers;
public:


  HttpError(const Info& info,
            const booster::String& message,
            const Headers& headers = {})
    : protocol::ProtocolError<Status>(info, message)
    , m_headers(headers)
  {}


  HttpError(const Status& status,
            const booster::String& message,
            const Headers& headers = {})
    : protocol::ProtocolError<Status>(Info(0, status), message)
    , m_headers(headers)
  {}


  const Headers& getHeaders() const {
    return m_headers;
  }
  
};


#define BOOSTER_ASSERT_HTTP(COND, STATUS, MESSAGE) \
if(!(COND)) { throw booster::web::protocol::http::HttpError(STATUS, MESSAGE, {}); }


class Header {
public:


  class Value {
  public:
    static const char* const CONNECTION_CLOSE;
    static const char* const CONNECTION_KEEP_ALIVE;
    static const char* const CONNECTION_UPGRADE;
    
    static const char* const SERVER;
    static const char* const USER_AGENT;
    
    static const char* const TRANSFER_ENCODING_CHUNKED;
    static const char* const CONTENT_TYPE_APPLICATION_JSON;

    static const char* const EXPECT_100_CONTINUE;
  };
public:
  static const char* const ACCEPT;              // "Accept"
  static const char* const AUTHORIZATION;       // "Authorization"
  static const char* const WWW_AUTHENTICATE;    // "WWW-Authenticate"
  static const char* const CONNECTION;          // "Connection"
  static const char* const TRANSFER_ENCODING;   // "Transfer-Encoding"
  static const char* const CONTENT_ENCODING;    // "Content-Encoding"
  static const char* const CONTENT_LENGTH;      // "Content-Length"
  static const char* const CONTENT_TYPE;        // "Content-Type"
  static const char* const CONTENT_RANGE;       // "Content-Range"
  static const char* const RANGE;               // "Range"
  static const char* const HOST;                // "Host"
  static const char* const USER_AGENT;          // "User-Agent"
  static const char* const SERVER;              // "Server"
  static const char* const UPGRADE;             // "Upgrade"
  static const char* const CORS_ORIGIN;         // Access-Control-Allow-Origin
  static const char* const CORS_METHODS;        // Access-Control-Allow-Methods
  static const char* const CORS_HEADERS;        // Access-Control-Allow-Headers
  static const char* const CORS_MAX_AGE;        // Access-Control-Max-Age
  static const char* const ACCEPT_ENCODING;     // Accept-Encoding
  static const char* const EXPECT;              // Expect
};
  
class Range {
public:
  static const char* const UNIT_BYTES;
private:
  Range()
    : units(nullptr)
  {}
public:
  
  Range(const booster::String& pUnits,
        v_int64 pStart,
        v_int64 pEnd)
    : units(pUnits)
    , start(pStart)
    , end(pEnd)
  {}
  
  booster::String units;
  v_int64 start;
  v_int64 end;
  
  booster::String toString() const;
  
  bool isValid() const {
    return units.get() != nullptr;
  }
  
  static Range parse(booster::utils::parser::Caret& caret);
  static Range parse(const booster::String& str);
  
};
  
class ContentRange {
public:
  static const char* const UNIT_BYTES;
private:
  ContentRange()
    : units(nullptr)
  {}
public:
  
  ContentRange(const booster::String& pUnits,
               v_int64 pStart,
               v_int64 pEnd,
               v_int64 pSize,
               bool pIsSizeKnown)
    : units(pUnits)
    , start(pStart)
    , end(pEnd)
    , size(pSize)
    , isSizeKnown(pIsSizeKnown)
  {}
  
  booster::String units;
  v_int64 start;
  v_int64 end;
  v_int64 size;
  bool isSizeKnown;
  
  booster::String toString() const;
  
  bool isValid() const {
    return units.get() != nullptr;
  }
  
  static ContentRange parse(booster::utils::parser::Caret& caret);
  static ContentRange parse(const booster::String& str);
  
};


struct RequestStartingLine {

  booster::data::share::StringKeyLabel method; // GET, POST ...


  booster::data::share::StringKeyLabel path;


  booster::data::share::StringKeyLabel protocol;
};


struct ResponseStartingLine {

  booster::data::share::StringKeyLabel protocol;


  v_int32 statusCode;


  booster::data::share::StringKeyLabel description;
};


struct HeaderValueData {


  std::unordered_set<data::share::StringKeyLabelCI> tokens;


  std::unordered_map<data::share::StringKeyLabelCI, data::share::StringKeyLabel> titleParams;


  booster::String getTitleParamValue(const data::share::StringKeyLabelCI& key) const;

};


class Parser {
private:
  static booster::data::share::StringKeyLabelCI parseHeaderNameLabel(const std::shared_ptr<std::string>& headersText,
                                                                   booster::utils::parser::Caret& caret);
public:


  static void parseRequestStartingLine(RequestStartingLine& line,
                                       const std::shared_ptr<std::string>& headersText,
                                       booster::utils::parser::Caret& caret,
                                       Status& error);


  static void parseResponseStartingLine(ResponseStartingLine& line,
                                        const std::shared_ptr<std::string>& headersText,
                                        booster::utils::parser::Caret& caret,
                                        Status& error);


  static void parseOneHeader(Headers& headers,
                             const std::shared_ptr<std::string>& headersText,
                             booster::utils::parser::Caret& caret,
                             Status& error);


  static void parseHeaders(Headers& headers,
                           const std::shared_ptr<std::string>& headersText,
                           booster::utils::parser::Caret& caret,
                           Status& error);


  static void parseHeaderValueData(HeaderValueData& data, const booster::data::share::StringKeyLabel& headerValue, char separator);

};


class Utils {
public:


  static void writeHeaders(const Headers& headers, data::stream::ConsistentOutputStream* stream);

};
  
}}}}

namespace std {
  
  template<>
  struct hash<booster::web::protocol::http::Status> {
    
    typedef booster::web::protocol::http::Status argument_type;
    typedef v_uint64 result_type;
    
    result_type operator()(booster::web::protocol::http::Status const& s) const noexcept {
      return static_cast<result_type>(s.code);
    }
    
  };
}

#endif
