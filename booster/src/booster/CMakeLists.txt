# 收集所有源文件
file(GLOB_RECURSE BOOSTER_SRC_FILES *.cpp)

if(NOT BOOSTER_SRC_FILES)
    message(FATAL_ERROR "No source files found in ${PROJECT_SOURCE_DIR}/src/booster!")
endif()

# 添加动态库
add_library(booster SHARED ${BOOSTER_SRC_FILES})

# 使用target_include_directories为booster目标设置包含目录
target_include_directories(booster PUBLIC
        .
        async
        async/utils
        async/worker
        base
        codegen
        codegen/api_controller
        codegen/dto
        concurrency
        data
        encoding
        json
        macro
        network
        network/monitor
        network/tcp
        network/tcp/server
        network/virtual_/server
        provider
        utils
        utils/parser
        web
        web/mime
        web/mime/multipart
        web/protocol
        web/protocol/http
        web/protocol/http/encoding
        web/protocol/http/incoming
        web/protocol/http/outgoing
        web/protocol/http/utils
        web/server
        web/server/api
        web/server/handler
        web/server/interceptor
        web/url
        web/url/mapping
)