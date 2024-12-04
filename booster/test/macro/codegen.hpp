



#ifndef booster_macro_codegen_hpp
#define booster_macro_codegen_hpp

#include "./basic.hpp"

#define BOOSTER_CODEGEN_DEFINE_ApiClient "booster/codegen/ApiClient_define.hpp"
#define BOOSTER_CODEGEN_UNDEF_ApiClient "booster/codegen/ApiClient_undef.hpp"

#define BOOSTER_CODEGEN_DEFINE_ApiController "booster/codegen/ApiController_define.hpp"
#define BOOSTER_CODEGEN_UNDEF_ApiController "booster/codegen/ApiController_undef.hpp"

#define BOOSTER_CODEGEN_DEFINE_DbClient "booster/codegen/DbClient_define.hpp"
#define BOOSTER_CODEGEN_UNDEF_DbClient "booster/codegen/DbClient_undef.hpp"

#define BOOSTER_CODEGEN_DEFINE_DTO "booster/codegen/DTO_define.hpp"
#define BOOSTER_CODEGEN_UNDEF_DTO "booster/codegen/DTO_undef.hpp"

#define BOOSTER_CODEGEN_BEGIN(NAME) BOOSTER_MACRO_EXPAND(BOOSTER_CODEGEN_DEFINE_ ## NAME)
#define BOOSTER_CODEGEN_END(NAME) BOOSTER_MACRO_EXPAND(BOOSTER_CODEGEN_UNDEF_ ## NAME)

#endif
