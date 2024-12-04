

#include "./ResponseFactory.hpp"

#include "./BufferBody.hpp"

namespace booster { namespace web { namespace protocol { namespace http { namespace outgoing {

std::shared_ptr<Response>
ResponseFactory::createResponse(const Status& status) {
  return Response::createShared(status, nullptr);
}

std::shared_ptr<Response>
ResponseFactory::createResponse(const Status& status, const booster::String& text) {
  return Response::createShared(status, BufferBody::createShared(text));
}

std::shared_ptr<Response>
ResponseFactory::createResponse(const Status& status,
                                const booster::Void& dto,
                                const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper) {
  return Response::createShared(status, BufferBody::createShared(
    objectMapper->writeToString(dto),
    objectMapper->getInfo().httpContentType
  ));
}

  
}}}}}
