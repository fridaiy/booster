

#define AUTHORIZATION(TYPE, ...) \
BOOSTER_MACRO_API_CONTROLLER_PARAM(BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION, BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION_INFO, TYPE, (__VA_ARGS__))

// AUTHORIZATION MACRO // ------------------------------------------------------

#define BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION_1(TYPE, NAME) \
auto __param_str_val_##NAME = __request->getHeader(booster::web::protocol::http::Header::AUTHORIZATION); \
std::shared_ptr<booster::web::server::handler::AuthorizationObject> __param_aosp_val_##NAME = ApiController::handleDefaultAuthorization(__param_str_val_##NAME); \
TYPE NAME = std::static_pointer_cast<TYPE::element_type>(__param_aosp_val_##NAME);

#define BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION_2(TYPE, NAME, AUTH_HANDLER) \
auto __param_str_val_##NAME = __request->getHeader(booster::web::protocol::http::Header::AUTHORIZATION); \
std::shared_ptr<booster::web::server::handler::AuthorizationHandler> __auth_handler_##NAME = AUTH_HANDLER; \
std::shared_ptr<booster::web::server::handler::AuthorizationObject> __param_aosp_val_##NAME = __auth_handler_##NAME->handleAuthorization(__param_str_val_##NAME); \
TYPE NAME = std::static_pointer_cast<TYPE::element_type>(__param_aosp_val_##NAME);

#define BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION(TYPE, PARAM_LIST) \
BOOSTER_MACRO_API_CONTROLLER_MACRO_SELECTOR(BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION_, TYPE, BOOSTER_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

// __INFO

#define BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION_INFO_1(TYPE, NAME) \
auto __param_obj_##NAME = ApiController::getDefaultAuthorizationHandler(); \
if(__param_obj_##NAME) { \
  info->headers.add(booster::web::protocol::http::Header::AUTHORIZATION, booster::String::Class::getType()); \
  info->headers[booster::web::protocol::http::Header::AUTHORIZATION].description = __param_obj_##NAME ->getScheme(); \
  info->authorization = __param_obj_##NAME ->getScheme(); \
} else { \
  throw booster::web::protocol::http::HttpError(Status::CODE_500, "No authorization handler set up in controller before controller was added to router or swagger-doc."); \
}

#define BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION_INFO_2(TYPE, NAME, AUTH_HANDLER) \
std::shared_ptr<booster::web::server::handler::AuthorizationHandler> __auth_handler_##NAME = AUTH_HANDLER; \
if(__auth_handler_##NAME) { \
  info->headers.add(booster::web::protocol::http::Header::AUTHORIZATION, booster::String::Class::getType()); \
  info->headers[booster::web::protocol::http::Header::AUTHORIZATION].description = __auth_handler_##NAME->getScheme(); \
  info->authorization = __auth_handler_##NAME->getScheme(); \
} else { \
  throw booster::web::protocol::http::HttpError(Status::CODE_500, "Invalid authorization handler given (or not set up) in AUTHORIZATION(TYPE, NAME, AUTH_HANDLER) before controller was added to router or swagger-doc."); \
}

#define BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION_INFO(TYPE, PARAM_LIST) \
BOOSTER_MACRO_API_CONTROLLER_MACRO_SELECTOR(BOOSTER_MACRO_API_CONTROLLER_AUTHORIZATION_INFO_, TYPE, BOOSTER_MACRO_UNFOLD_VA_ARGS PARAM_LIST)
