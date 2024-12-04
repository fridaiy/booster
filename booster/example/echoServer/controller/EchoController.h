//
// Created by 1120231424 on 12/2/24.
//

#ifndef OATPP_MASTER_ECHOCONTROLLER_H
#define OATPP_MASTER_ECHOCONTROLLER_H

#include "booster/web/server/api/ApiController.hpp"
#include "booster/web/mime/ContentMappers.hpp"
#include "booster/macro/codegen.hpp"

#include BOOSTER_CODEGEN_BEGIN(ApiController) //<- Begin Codegen


class EchoController : public booster::web::server::api::ApiController {
public:
    EchoController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers))
    : oatpp::web::server::api::ApiController(apiContentMappers)
            {}
private:
    UserService m_userService; // Create user service.
public:

    static std::shared_ptr<UserController> createShared(
            OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers) // Inject ContentMappers
    ){
        return std::make_shared<UserController>(apiContentMappers);
    }


    ENDPOINT("POST", "users", createUser,
    BODY_DTO(Object<UserDto>, userDto))
    {
        return createDtoResponse(Status::CODE_200, m_userService.createUser(userDto));
    }



    ENDPOINT("PUT", "users/{userId}", putUser,
    PATH(Int32, userId),
    BODY_DTO(Object<UserDto>, userDto))
    {
        userDto->id = userId;
        return createDtoResponse(Status::CODE_200, m_userService.updateUser(userDto));
    }


    ENDPOINT("GET", "users/{userId}", getUserById,
    PATH(Int32, userId))
    {
        return createDtoResponse(Status::CODE_200, m_userService.getUserById(userId));
    }



    ENDPOINT("GET", "users/offset/{offset}/limit/{limit}", getUsers,
    PATH(UInt32, offset),
    PATH(UInt32, limit))
    {
        return createDtoResponse(Status::CODE_200, m_userService.getAllUsers(offset, limit));
    }


    ENDPOINT("DELETE", "users/{userId}", deleteUser,
    PATH(Int32, userId))
    {
        return createDtoResponse(Status::CODE_200, m_userService.deleteUserById(userId));
    }

};
#endif //OATPP_MASTER_ECHOCONTROLLER_H
