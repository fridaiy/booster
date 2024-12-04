#ifndef DTOs_hpp
#define DTOs_hpp

#include <booster/macro/codegen.hpp>
#include <booster/Types.hpp>

#include BOOSTER_CODEGEN_BEGIN(DTO)


class MessageDto : public booster::DTO {

  DTO_INIT(MessageDto, DTO)

  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);

};

#include BOOSTER_CODEGEN_END(DTO)

#endif
