#!/bin/bash




# 定义目标目录
target_dir="/mnt/nfs/data/home/1120231424/booster/booster/test"

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




