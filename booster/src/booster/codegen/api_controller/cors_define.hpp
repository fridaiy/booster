

#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_ORIGIN "*"
#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_METHODS "GET, POST, OPTIONS"
#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_HEADERS "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization"
#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_MAX_AGE "1728000"

#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY(ARG_ORIGIN, ARG_METHODS, ARG_HEADERS, ARG_MAX_AGE) \
  resp->putHeaderIfNotExists(booster::web::protocol::http::Header::CORS_ORIGIN, ARG_ORIGIN); \
  resp->putHeaderIfNotExists(booster::web::protocol::http::Header::CORS_METHODS, ARG_METHODS); \
  resp->putHeaderIfNotExists(booster::web::protocol::http::Header::CORS_HEADERS, ARG_HEADERS);\
  resp->putHeaderIfNotExists(booster::web::protocol::http::Header::CORS_MAX_AGE, ARG_MAX_AGE);

#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_OPTIONS_DECL(ENDPOINTNAME) \
BOOSTER_MACRO_API_CONTROLLER_ENDPOINT_DECL_DEFAULTS(Z__CORS_OPTIONS_DECL_##ENDPOINTNAME, "OPTIONS", "") \
EndpointInfoBuilder Z__CREATE_ENDPOINT_INFO_Z__CORS_OPTIONS_DECL_##ENDPOINTNAME = [this](){ \
  auto info = Z__EDNPOINT_INFO_GET_INSTANCE_Z__CORS_OPTIONS_DECL_##ENDPOINTNAME(); \
  info->name = "CORS_OPTIONS_DECL_" #ENDPOINTNAME; \
  info->path = Z__ENDPOINT_##ENDPOINTNAME->info()->path; \
  info->method = "OPTIONS"; \
  info->pathParams = Z__ENDPOINT_##ENDPOINTNAME->info()->pathParams; \
  info->hide = true; \
  return info; \
}; \
\
const std::shared_ptr<booster::web::server::api::Endpoint>               \
  Z__ENDPOINT_Z__CORS_OPTIONS_DECL_##ENDPOINTNAME = createEndpoint(m_endpoints, \
  Z__ENDPOINT_HANDLER_GET_INSTANCE_Z__CORS_OPTIONS_DECL_##ENDPOINTNAME(this), \
  Z__CREATE_ENDPOINT_INFO_Z__CORS_OPTIONS_DECL_##ENDPOINTNAME);\
\
std::shared_ptr<booster::web::protocol::http::outgoing::Response> \
Z__PROXY_METHOD_Z__CORS_OPTIONS_DECL_##ENDPOINTNAME(const std::shared_ptr<booster::web::protocol::http::incoming::Request>& __request) \
{ \
  (void)__request; \
  return Z__CORS_OPTIONS_DECL_##ENDPOINTNAME(); \
} \
\
std::shared_ptr<booster::web::protocol::http::outgoing::Response> Z__CORS_OPTIONS_DECL_##ENDPOINTNAME()

#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_MACRO_1(ENDPOINTNAME, ...) \
ENDPOINT_INTERCEPTOR(ENDPOINTNAME, CORS) { \
  auto resp = (this->*intercepted)(request); \
  BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY( \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_ORIGIN, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_METHODS, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_HEADERS, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_MAX_AGE \
  ) \
  return resp; \
} \
BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_OPTIONS_DECL(ENDPOINTNAME) { \
  auto resp = createResponse(Status::CODE_204, ""); \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY( \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_ORIGIN, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_METHODS, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_HEADERS, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_MAX_AGE \
  ) \
  return resp; \
}

#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_MACRO_2(ENDPOINTNAME, ORIGIN) \
ENDPOINT_INTERCEPTOR(ENDPOINTNAME, CORS) { \
  auto resp = (this->*intercepted)(request); \
  BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY( \
      ORIGIN, \
      BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_METHODS, \
      BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_HEADERS, \
      BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_MAX_AGE \
  ) \
  return resp; \
} \
BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_OPTIONS_DECL(ENDPOINTNAME) { \
  auto resp = createResponse(Status::CODE_204, ""); \
  BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY( \
    ORIGIN, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_METHODS, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_HEADERS, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_MAX_AGE \
  ) \
  return resp; \
}

#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_MACRO_3(ENDPOINTNAME, ORIGIN, METHODS) \
ENDPOINT_INTERCEPTOR(ENDPOINTNAME, CORS) { \
  auto resp = (this->*intercepted)(request); \
  BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY( \
      ORIGIN, \
      METHODS, \
      BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_HEADERS, \
      BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_MAX_AGE \
  ) \
  return resp; \
} \
BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_OPTIONS_DECL(ENDPOINTNAME) { \
  auto resp = createResponse(Status::CODE_204, ""); \
  BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY( \
    ORIGIN, \
    METHODS, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_HEADERS, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_MAX_AGE \
  ) \
  return resp; \
}

#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_MACRO_4(ENDPOINTNAME, ORIGIN, METHODS, HEADERS) \
ENDPOINT_INTERCEPTOR(ENDPOINTNAME, CORS) { \
  auto resp = (this->*intercepted)(request); \
  BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY( \
      ORIGIN, \
      METHODS, \
      HEADERS, \
      BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_MAX_AGE \
  ) \
  return resp; \
} \
BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_OPTIONS_DECL(ENDPOINTNAME) { \
  auto resp = createResponse(Status::CODE_204, ""); \
  BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY( \
    ORIGIN, \
    METHODS, \
    HEADERS, \
    BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY_DEFAULT_MAX_AGE \
  ) \
  return resp; \
}

#define BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_MACRO_5(ENDPOINTNAME, ORIGIN, METHODS, HEADERS, MAX_AGE) \
ENDPOINT_INTERCEPTOR(ENDPOINTNAME, CORS) { \
  auto resp = (this->*intercepted)(request); \
  BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY(ORIGIN, METHODS, HEADERS, MAX_AGE) \
  return resp; \
} \
BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_OPTIONS_DECL(ENDPOINTNAME) { \
  auto resp = createResponse(Status::CODE_204, ""); \
  BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_BODY(ORIGIN, METHODS, HEADERS, MAX_AGE) \
  return resp; \
}

#define ADD_CORS(...) \
BOOSTER_MACRO_EXPAND(BOOSTER_MACRO_MACRO_SELECTOR(BOOSTER_MACRO_API_CONTROLLER_ADD_CORS_MACRO_, (__VA_ARGS__)) (__VA_ARGS__))
