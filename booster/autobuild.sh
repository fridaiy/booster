#!/bin/bash

set -e

# 如果没有build目录，创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/tinymuduo  so库拷贝到 /usr/lib    PATH
if [ ! -d /usr/include/booster ]; then
    mkdir /usr/include/booster
fi

#!/bin/bash

# 指定目标文件夹


target_dir="/usr/include/booster"

# 遍历 src/booster 下的所有 .hpp 文件
find ./src/booster -type f -name "*.hpp" | while read header; do
    # 获取相对路径，并去掉 'booster/' 部分
    relative_path=$(echo "$header" | sed 's#^./src/booster/##')

    # 拼接目标路径（去掉 booster 文件夹后的新目录结构）
    target_path="$target_dir/$(dirname "$relative_path")"

    # 创建目标路径（如果不存在）
    mkdir -p "$target_path"

    # 拷贝文件到目标路径
    cp "$header" "$target_path"
done

echo "All header files have been copied to $target_dir without the 'booster' directory level."


cp `pwd`/lib/libbooster.so /usr/lib

ldconfig