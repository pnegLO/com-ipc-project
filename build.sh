#!/bin/bash

# 注意：需要在Windows机器上运行Docker Desktop，并在MacOS上使用远程Docker

# 设置Docker主机
# 如果使用远程Docker主机，取消下面一行的注释并设置正确的IP
# export DOCKER_HOST=tcp://192.168.x.x:2375

# 构建Docker镜像
echo "构建Docker镜像..."
docker build -t com-ipc-builder .

# 创建输出目录
mkdir -p ./output

# 运行容器并从容器中提取编译好的文件
echo "从Docker容器提取编译好的文件..."
docker run --rm -v "$(pwd)/output:/host-output" com-ipc-builder -Command "Copy-Item -Path C:/output/* -Destination /host-output/ -Recurse -Force"

echo "构建完成，输出文件位于 ./output 目录" 