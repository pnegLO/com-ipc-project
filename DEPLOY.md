# 在macOS上构建Windows COM应用程序的详细部署指南

由于COM是Windows特有技术，本文档提供了在macOS上构建Windows COM应用的详细步骤。

## 方案比较

| 部署方案 | 优点 | 缺点 | 推荐场景 |
|---------|------|------|----------|
| GitHub Actions | 无需本地Windows环境<br>自动化程度高<br>便于持续集成 | 需要公开代码或付费私有仓库<br>构建时间受限 | 团队协作<br>开源项目 |
| Docker | 配置简单<br>可重复使用<br>便于自动化 | 需要Windows Docker主机<br>配置Docker较复杂 | 有可用Windows服务器<br>需要频繁构建 |
| Vagrant VM | 完全本地化<br>可自定义环境<br>无网络依赖 | 资源消耗大<br>初始设置时间长 | 单机开发<br>对环境要求高 |

## 1. GitHub Actions 详细配置

### 步骤1: 创建GitHub仓库
1. 在GitHub上创建新仓库
2. 将代码提交到该仓库

### 步骤2: 配置工作流
1. 确保`.github/workflows/build.yml`文件已添加到仓库
2. 推送代码触发自动构建

### 步骤3: 下载构建产物
1. 在GitHub仓库页面，点击"Actions"选项卡
2. 选择最新的成功构建工作流
3. 在"Artifacts"部分下载`com-ipc-package`

## 2. Docker构建详细配置

### 前提条件
- 安装Docker Desktop for Mac
- 访问Windows Docker主机（可以是远程服务器或虚拟机）

### 步骤1: 配置Docker主机
1. 在Windows机器上安装Docker
2. 配置Docker daemon开启远程API:
   ```powershell
   # 在Windows PowerShell中执行(管理员权限)
   New-Item -Path 'C:\ProgramData\Docker\config' -ItemType Directory -Force
   Set-Content -Path 'C:\ProgramData\Docker\config\daemon.json' -Value '{ "hosts": ["tcp://0.0.0.0:2375", "npipe://"] }'
   Restart-Service docker
   ```

3. 确保Windows防火墙允许2375端口

### 步骤2: 配置macOS客户端
1. 编辑`build.sh`，设置正确的Docker主机地址:
   ```bash
   export DOCKER_HOST=tcp://你的Windows主机IP:2375
   ```

2. 设置脚本权限并运行:
   ```bash
   chmod +x build.sh
   ./build.sh
   ```

## 3. Vagrant虚拟机详细配置

### 前提条件
- macOS上安装VirtualBox
- macOS上安装Vagrant

### 步骤1: 安装依赖
```bash
brew install --cask virtualbox
brew install --cask vagrant
```

### 步骤2: 初始化并启动虚拟机
```bash
# 确保Vagrantfile已存在
vagrant up
```

### 步骤3: 访问构建结果
构建完成后，编译好的可执行文件将放在`./build`目录中

### 注意事项
- 首次启动会下载Windows镜像，可能需要较长时间
- 虚拟机需要至少4GB内存和2个CPU核心
- 可以通过`vagrant ssh`进入虚拟机进行调试

## 常见问题

### Q: GitHub Actions构建失败
A: 检查日志确认错误原因。常见问题包括权限问题或代码错误。

### Q: Docker无法连接到Windows主机
A: 确保网络连通，Windows防火墙已配置，并且Docker已开启远程访问。

### Q: Vagrant虚拟机无法启动
A: 检查VirtualBox安装是否正确，内存是否足够，并确认没有其他虚拟化软件冲突。

### Q: 构建结果无法在Windows上运行
A: 确保所有依赖项已正确编译和包含。检查是否需要注册COM组件。 