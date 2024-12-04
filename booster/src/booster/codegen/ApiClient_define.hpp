



#include "booster/macro/basic.hpp"
#include "booster/macro/codegen.hpp"

#define BOOSTER_MACRO_API_CLIENT_PARAM_MACRO(MACRO, TYPE, PARAM_LIST) MACRO(TYPE, PARAM_LIST)
#define BOOSTER_MACRO_API_CLIENT_PARAM_TYPE(MACRO, TYPE, PARAM_LIST) const TYPE&
#define BOOSTER_MACRO_API_CLIENT_PARAM_NAME(MACRO, TYPE, PARAM_LIST) BOOSTER_MACRO_FIRSTARG PARAM_LIST
#define BOOSTER_MACRO_API_CLIENT_PARAM_TYPE_STR(MACRO, TYPE, PARAM_LIST) #TYPE
#define BOOSTER_MACRO_API_CLIENT_PARAM_NAME_STR(MACRO, TYPE, PARAM_LIST) BOOSTER_MACRO_FIRSTARG_STR PARAM_LIST
#define BOOSTER_MACRO_API_CLIENT_PARAM(MACRO, TYPE, PARAM_LIST) (MACRO, TYPE, PARAM_LIST)

#define HEADER(TYPE, ...)               BOOSTER_MACRO_API_CLIENT_PARAM(BOOSTER_MACRO_API_CLIENT_HEADER, TYPE, (__VA_ARGS__))
#define PATH(TYPE, ...)                 BOOSTER_MACRO_API_CLIENT_PARAM(BOOSTER_MACRO_API_CLIENT_PATH, TYPE, (__VA_ARGS__))
#define QUERY(TYPE, ...)                BOOSTER_MACRO_API_CLIENT_PARAM(BOOSTER_MACRO_API_CLIENT_QUERY, TYPE, (__VA_ARGS__))
#define BODY(TYPE, ...)                 BOOSTER_MACRO_API_CLIENT_PARAM(BOOSTER_MACRO_API_CLIENT_BODY, TYPE, (__VA_ARGS__))
#define BODY_DTO(TYPE, ...)             BOOSTER_MACRO_API_CLIENT_PARAM(BOOSTER_MACRO_API_CLIENT_BODY_DTO, TYPE, (__VA_ARGS__))
#define BODY_STRING(TYPE, ...)          BOOSTER_MACRO_API_CLIENT_PARAM(BOOSTER_MACRO_API_CLIENT_BODY_STRING, TYPE, (__VA_ARGS__))
#define AUTHORIZATION(TYPE, ...)        BOOSTER_MACRO_API_CLIENT_PARAM(BOOSTER_MACRO_API_CLIENT_AUTHORIZATION, TYPE, (__VA_ARGS__))
#define AUTHORIZATION_BASIC(TYPE, ...)  BOOSTER_MACRO_API_CLIENT_PARAM(BOOSTER_MACRO_API_CLIENT_AUTHORIZATION_BASIC, TYPE, (__VA_ARGS__))

//////////////////////////////////////////////////////////////////////////

#define BOOSTER_MACRO_API_CLIENT_MACRO_SELECTOR(MACRO, TYPE, ...) \
BOOSTER_MACRO_EXPAND(BOOSTER_MACRO_MACRO_SELECTOR(MACRO, (__VA_ARGS__)) (TYPE, __VA_ARGS__))

//////////////////////////////////////////////////////////////////////////

// INIT


#define API_CLIENT_INIT(NAME) \
public: \
  NAME(const std::shared_ptr<booster::web::client::RequestExecutor>& requestExecutor, \
       const std::shared_ptr<booster::data::mapping::ObjectMapper>& objectMapper) \
    : booster::web::client::ApiClient(requestExecutor, objectMapper) \
  {} \
public: \
  static std::shared_ptr<NAME> createShared(const std::shared_ptr<booster::web::client::RequestExecutor>& requestExecutor, \
                                      const std::shared_ptr<booster::data::mapping::ObjectMapper>& objectMapper){ \
    return std::make_shared<NAME>(requestExecutor, objectMapper); \
  }

// HEADER MACRO

#define BOOSTER_MACRO_API_CLIENT_HEADER_1(TYPE, NAME) \
__headers.put_LockFree(#NAME, ApiClient::TypeInterpretation<TYPE>::toString(#TYPE, NAME));

#define BOOSTER_MACRO_API_CLIENT_HEADER_2(TYPE, NAME, QUALIFIER) \
__headers.put_LockFree(QUALIFIER, ApiClient::TypeInterpretation<TYPE>::toString(#TYPE, NAME));

#define BOOSTER_MACRO_API_CLIENT_HEADER(TYPE, PARAM_LIST) \
BOOSTER_MACRO_API_CLIENT_MACRO_SELECTOR(BOOSTER_MACRO_API_CLIENT_HEADER_, TYPE, BOOSTER_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

// PATH MACRO

#define BOOSTER_MACRO_API_CLIENT_PATH_1(TYPE, NAME) \
__pathParams.insert({#NAME, ApiClient::TypeInterpretation<TYPE>::toString(#TYPE, NAME)});

#define BOOSTER_MACRO_API_CLIENT_PATH_2(TYPE, NAME, QUALIFIER) \
__pathParams.insert({QUALIFIER, ApiClient::TypeInterpretation<TYPE>::toString(#TYPE, NAME)});

#define BOOSTER_MACRO_API_CLIENT_PATH(TYPE, PARAM_LIST) \
BOOSTER_MACRO_API_CLIENT_MACRO_SELECTOR(BOOSTER_MACRO_API_CLIENT_PATH_, TYPE, BOOSTER_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

// QUERY MACRO

#define BOOSTER_MACRO_API_CLIENT_QUERY_1(TYPE, NAME) \
__queryParams.insert({#NAME, ApiClient::TypeInterpretation<TYPE>::toString(#TYPE, NAME)});

#define BOOSTER_MACRO_API_CLIENT_QUERY_2(TYPE, NAME, QUALIFIER) \
__queryParams.insert({QUALIFIER, ApiClient::TypeInterpretation<TYPE>::toString(#TYPE, NAME)});

#define BOOSTER_MACRO_API_CLIENT_QUERY(TYPE, PARAM_LIST) \
BOOSTER_MACRO_API_CLIENT_MACRO_SELECTOR(BOOSTER_MACRO_API_CLIENT_QUERY_, TYPE, BOOSTER_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

// BODY MACRO

#define BOOSTER_MACRO_API_CLIENT_BODY(TYPE, PARAM_LIST) \
__body = BOOSTER_MACRO_FIRSTARG PARAM_LIST;

// BODY_DTO MACRO

#define BOOSTER_MACRO_API_CLIENT_BODY_DTO(TYPE, PARAM_LIST) \
__body = booster::web::protocol::http::outgoing::BufferBody::createShared( \
    m_objectMapper->writeToString(BOOSTER_MACRO_FIRSTARG PARAM_LIST), \
    m_objectMapper->getInfo().httpContentType \
  );

// BODY_STRING MACRO

#define BOOSTER_MACRO_API_CLIENT_BODY_STRING(TYPE, PARAM_LIST) \
__body = booster::web::protocol::http::outgoing::BufferBody::createShared(BOOSTER_MACRO_FIRSTARG PARAM_LIST);

// AUTHORIZATION MACRO

#define BOOSTER_MACRO_API_CLIENT_AUTHORIZATION_2(TYPE, TOKEN, SCHEME) \
__headers.put_LockFree("Authorization", String(SCHEME " ") + String(TOKEN));

#define BOOSTER_MACRO_API_CLIENT_AUTHORIZATION(TYPE, PARAM_LIST) \
BOOSTER_MACRO_API_CLIENT_MACRO_SELECTOR(BOOSTER_MACRO_API_CLIENT_AUTHORIZATION_, TYPE, BOOSTER_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

// AUTHORIZATION_BASIC MACRO

#define BOOSTER_MACRO_API_CLIENT_AUTHORIZATION_BASIC_1(TYPE, TOKEN) \
__headers.put_LockFree("Authorization", String("Basic ") + booster::encoding::Base64::encode(TOKEN));

#define BOOSTER_MACRO_API_CLIENT_AUTHORIZATION_BASIC(TYPE, PARAM_LIST) \
BOOSTER_MACRO_API_CLIENT_MACRO_SELECTOR(BOOSTER_MACRO_API_CLIENT_AUTHORIZATION_BASIC_, TYPE, BOOSTER_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

// FOR EACH

#define BOOSTER_MACRO_API_CLIENT_PARAM_DECL(INDEX, COUNT, X) \
BOOSTER_MACRO_API_CLIENT_PARAM_TYPE X BOOSTER_MACRO_API_CLIENT_PARAM_NAME X,

#define BOOSTER_MACRO_API_CLIENT_PARAM_PUT(INDEX, COUNT, X) \
BOOSTER_MACRO_API_CLIENT_PARAM_MACRO X

// API_CALL MACRO

#define BOOSTER_API_CALL_0(NAME, METHOD, PATH) \
const booster::data::share::StringTemplate Z_PATH_TEMPLATE_##NAME = parsePathTemplate(#NAME, PATH); \
\
static void Z_ADD_HEADERS_##NAME(booster::web::client::ApiClient::Headers& headers, ...) { \
  (void) headers; \
} \
\
std::shared_ptr<booster::web::protocol::http::incoming::Response> NAME( \
  const std::shared_ptr<booster::web::client::RequestExecutor::ConnectionHandle>& __connectionHandle = nullptr \
) { \
  booster::web::client::ApiClient::Headers __headers; \
  Z_ADD_HEADERS_##NAME(__headers, 1); \
  std::shared_ptr<booster::web::protocol::http::outgoing::Body> body; \
  return executeRequest(METHOD, \
                        Z_PATH_TEMPLATE_##NAME, \
                        __headers, \
                        {}, \
                        {}, \
                        body, \
                        __connectionHandle); \
}

#define BOOSTER_API_CALL_1(NAME, METHOD, PATH, ...) \
const booster::data::share::StringTemplate Z_PATH_TEMPLATE_##NAME = parsePathTemplate(#NAME, PATH); \
\
static void Z_ADD_HEADERS_##NAME(booster::web::client::ApiClient::Headers& headers, ...) { \
  (void) headers; \
} \
\
std::shared_ptr<booster::web::protocol::http::incoming::Response> NAME(\
BOOSTER_MACRO_FOREACH(BOOSTER_MACRO_API_CLIENT_PARAM_DECL, __VA_ARGS__) \
  const std::shared_ptr<booster::web::client::RequestExecutor::ConnectionHandle>& __connectionHandle = nullptr \
) { \
  booster::web::client::ApiClient::Headers __headers; \
  Z_ADD_HEADERS_##NAME(__headers, 1); \
  std::unordered_map<booster::String, booster::String> __pathParams; \
  std::unordered_map<booster::String, booster::String> __queryParams; \
  std::shared_ptr<booster::web::protocol::http::outgoing::Body> __body; \
  BOOSTER_MACRO_FOREACH(BOOSTER_MACRO_API_CLIENT_PARAM_PUT, __VA_ARGS__) \
  return executeRequest(METHOD, \
                        Z_PATH_TEMPLATE_##NAME, \
                        __headers, \
                        __pathParams, \
                        __queryParams, \
                        __body, \
                        __connectionHandle); \
}

// Chooser

#define BOOSTER_API_CALL_MACRO_0(METHOD, PATH, NAME) \
BOOSTER_API_CALL_0(NAME, METHOD, PATH)

#define BOOSTER_API_CALL_MACRO_1(METHOD, PATH, NAME, ...) \
BOOSTER_API_CALL_1(NAME, METHOD, PATH, __VA_ARGS__)


#define API_CALL(METHOD, PATH, ...) \
BOOSTER_MACRO_EXPAND(BOOSTER_MACRO_MACRO_BINARY_SELECTOR(BOOSTER_API_CALL_MACRO_, (__VA_ARGS__)) (METHOD, PATH, __VA_ARGS__))

// API_CALL_ASYNC MACRO

#define BOOSTER_API_CALL_ASYNC_0(NAME, METHOD, PATH) \
const booster::data::share::StringTemplate Z_PATH_TEMPLATE_##NAME = parsePathTemplate(#NAME, PATH); \
\
static void Z_ADD_HEADERS_##NAME(booster::web::client::ApiClient::Headers& headers, ...) { \
  (void) headers; \
} \
\
booster::async::CoroutineStarterForResult<const std::shared_ptr<booster::web::protocol::http::incoming::Response>&> NAME( \
  const std::shared_ptr<booster::web::client::RequestExecutor::ConnectionHandle>& __connectionHandle = nullptr \
) { \
  booster::web::client::ApiClient::Headers __headers; \
  Z_ADD_HEADERS_##NAME(__headers, 1); \
  std::shared_ptr<booster::web::protocol::http::outgoing::Body> body; \
  return executeRequestAsync(METHOD, \
                             Z_PATH_TEMPLATE_##NAME, \
                             __headers, \
                             {}, \
                             {}, \
                             body, \
                             __connectionHandle); \
}

#define BOOSTER_API_CALL_ASYNC_1(NAME, METHOD, PATH, ...) \
const booster::data::share::StringTemplate Z_PATH_TEMPLATE_##NAME = parsePathTemplate(#NAME, PATH); \
\
static void Z_ADD_HEADERS_##NAME(booster::web::client::ApiClient::Headers& headers, ...) { \
  (void) headers; \
} \
\
booster::async::CoroutineStarterForResult<const std::shared_ptr<booster::web::protocol::http::incoming::Response>&> NAME(\
  BOOSTER_MACRO_FOREACH(BOOSTER_MACRO_API_CLIENT_PARAM_DECL, __VA_ARGS__) \
  const std::shared_ptr<booster::web::client::RequestExecutor::ConnectionHandle>& __connectionHandle = nullptr \
) { \
  booster::web::client::ApiClient::Headers __headers; \
  Z_ADD_HEADERS_##NAME(__headers, 1); \
  std::unordered_map<booster::String, booster::String> __pathParams; \
  std::unordered_map<booster::String, booster::String> __queryParams; \
  std::shared_ptr<booster::web::protocol::http::outgoing::Body> __body; \
  BOOSTER_MACRO_FOREACH(BOOSTER_MACRO_API_CLIENT_PARAM_PUT, __VA_ARGS__) \
  return executeRequestAsync(METHOD, \
                             Z_PATH_TEMPLATE_##NAME, \
                             __headers, \
                             __pathParams, \
                             __queryParams, \
                             __body, \
                             __connectionHandle); \
}

#define BOOSTER_API_CALL_ASYNC_MACRO_0(METHOD, PATH, NAME) \
BOOSTER_API_CALL_ASYNC_0(NAME, METHOD, PATH)

#define BOOSTER_API_CALL_ASYNC_MACRO_1(METHOD, PATH, NAME, ...) \
BOOSTER_API_CALL_ASYNC_1(NAME, METHOD, PATH, __VA_ARGS__)


#define API_CALL_ASYNC(METHOD, PATH, ...) \
BOOSTER_MACRO_EXPAND(BOOSTER_MACRO_MACRO_BINARY_SELECTOR(BOOSTER_API_CALL_ASYNC_MACRO_, (__VA_ARGS__)) (METHOD, PATH, __VA_ARGS__))


#define API_CALL_HEADERS(NAME) \
\
static void Z_ADD_HEADERS_##NAME(booster::web::client::ApiClient::Headers& headers, int) { \
  Z_ADD_HEADERS_##NAME(headers);  \
  Z_ADD_DEFAULT_HEADERS_##NAME(headers); \
} \
\
static void Z_ADD_DEFAULT_HEADERS_##NAME(booster::web::client::ApiClient::Headers& headers)
