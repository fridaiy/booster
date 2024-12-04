cmake_minimum_required(VERSION 3.8)

# 设置C++标准为C++17并要求必须遵循该标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(booster)

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 收集所有源文件
file(GLOB_RECURSE BOOSTER_SRC_FILES ${PROJECT_SOURCE_DIR}/src/booster/*.cpp)

if(NOT BOOSTER_SRC_FILES)
    message(FATAL_ERROR "No source files found in ${PROJECT_SOURCE_DIR}/src/booster!")
endif()

# 添加动态库，先定义booster这个目标
add_library(booster SHARED ${BOOSTER_SRC_FILES})

# 使用target_include_directories为booster目标设置包含目录，放在目标定义之后
target_include_directories(booster PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

# 设置动态库的输出路径
set_target_properties(booster PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
)