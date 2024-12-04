# 设置项目名称
cmake_minimum_required(VERSION 3.10)
project(example)

# 设置C++标准，例如C++11、C++14等，这里以C++11为例
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



# 添加可执行文件，并指定源文件
add_executable(example
        App.cpp
        controller/MyController.cpp
)

target_include_directories(example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dto)

target_link_libraries(example
        booster
)