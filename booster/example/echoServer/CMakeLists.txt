cmake_minimum_required(VERSION 3.8)
project(my_project)
# 设置包含目录，假设源文件目录结构中有controller目录
include_directories(${PROJECT_SOURCE_DIR}/controller)
# 其他相关设置，如添加库、源文件等
add_executable(my_executable Main.cpp)
target_link_libraries(my_executable /mnt/nfs/data/home/1120231424/booster/booster/example/echoServer/lib/libbooster.so)