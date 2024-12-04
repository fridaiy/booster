#ifndef MyController_hpp
#define MyController_hpp

#include "./DTOs.hpp"


#include <booster/web/server/api/ApiController.hpp>
#include <booster/web/mime/ContentMappers.hpp>

#include "booster/macro/codegen.hpp"
#include "booster/macro/component.hpp"

#include BOOSTER_CODEGEN_BEGIN(ApiController)


class MyController : public booster::web::server::api::ApiController {
private:
  typedef MyController __ControllerType;
public:

  MyController(BOOSTER_COMPONENT(std::shared_ptr<booster::web::mime::ContentMappers>, mappers))
    : booster::web::server::api::ApiController(mappers)
  {}
public:
  
  ENDPOINT_ASYNC("GET", "/hello", Root) {
    
    ENDPOINT_ASYNC_INIT(Root)
    
    Action act() override {
      auto dto = MessageDto::createShared();
      dto->statusCode = 200;
      dto->message = "Hello World Async!";
      return _return(controller->createDtoResponse(Status::CODE_200, dto));
    }
    
  };
  

  
};

#include BOOSTER_CODEGEN_BEGIN(ApiController)

#endif
