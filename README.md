# COM进程间通信（IPC）示例

本项目基于COM（组件对象模型）实现了跨进程通信机制，使用RPC（远程过程调用）技术并基于ATL框架开发。

## 项目结构

- `Interface/` - 包含COM接口定义（IDL文件）
- `ComIpcServer/` - COM服务器实现，基于ATL框架
- `ComIpcClient/` - COM客户端应用程序，用于与服务器通信

## 功能特点

- 基于COM的进程间通信
- 支持字符串和数值数据传输
- 使用RPC进行跨进程调用
- 服务器状态查询功能

## macOS上的自动部署方案

由于COM是Windows特有技术，在macOS上需要使用以下方法之一进行构建：

### 1. 使用GitHub Actions自动构建

项目已配置GitHub Actions工作流，可以在Windows环境中自动构建：

1. 将代码推送到GitHub仓库
2. Actions会自动在Windows环境中构建应用
3. 构建结果将作为Artifacts上传，可直接下载使用

配置文件位于: `.github/workflows/build.yml`

### 2. 使用Docker进行跨平台构建

通过Docker Windows容器构建（需要远程Windows Docker主机）：

1. 确保有可用的Windows Docker主机
2. 配置`build.sh`中的Docker主机地址
3. 运行`build.sh`脚本
4. 编译好的文件将在`./output`目录中

```bash
# 运行构建脚本
chmod +x build.sh
./build.sh
```

### 3. 使用Vagrant在虚拟机中构建

在macOS上使用VirtualBox和Vagrant创建Windows虚拟机并自动构建：

1. 安装VirtualBox和Vagrant
   ```bash
   brew install --cask virtualbox
   brew install --cask vagrant
   ```

2. 启动Vagrant虚拟机并构建
   ```bash
   vagrant up
   ```

3. 构建好的文件将在`./build`目录中

## 原始构建步骤

1. 使用Visual Studio打开项目
2. 首先生成COM接口类型库：
   - 使用MIDL编译器编译`Interface/IpcInterface.idl`文件
   - 命令：`midl /tlb IpcInterface.tlb /h IpcInterface.h IpcInterface.idl`

3. 构建服务器项目：
   - 编译`ComIpcServer`
   - 注册COM组件：`regsvr32 ComIpcServer.exe`或运行程序（自注册）

4. 构建客户端项目：
   - 编译`ComIpcClient`

## 运行说明

1. 先启动服务器程序`ComIpcServer.exe`
2. 在服务器启动后，运行客户端程序`ComIpcClient.exe`
3. 客户端将连接服务器并提供以下功能：
   - 发送文本消息
   - 发送数值数据（服务器将计算平方并返回）
   - 查询服务器状态

## 技术说明

- **COM接口**：使用IDL定义IMessageService接口
- **RPC机制**：通过代理-存根架构实现跨进程通信
- **ATL框架**：简化COM组件开发
- **数据传输**：使用BSTR和基本数据类型

## 系统需求

- Windows操作系统
- Visual Studio 开发环境
- ATL支持 