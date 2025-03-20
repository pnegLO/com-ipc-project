# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  # 使用Windows Server 2019评估版本
  config.vm.box = "gusztavvargadr/windows-server"
  
  # 配置虚拟机
  config.vm.provider "virtualbox" do |vb|
    vb.memory = "4096"
    vb.cpus = 2
    vb.gui = true
  end
  
  # 端口转发
  config.vm.network "forwarded_port", guest: 3389, host: 33389
  
  # 共享文件夹
  config.vm.synced_folder ".", "/vagrant_data"
  
  # 安装所需软件
  config.vm.provision "shell", inline: <<-SHELL
    # 安装Chocolatey
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
    
    # 安装Visual Studio Build Tools
    choco install visualstudio2019buildtools -y
    choco install visualstudio2019-workload-vctools -y
    choco install windows-sdk-10-version-2004-all -y
    
    # 设置环境变量
    [Environment]::SetEnvironmentVariable("PATH", $Env:PATH + ";C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\MSBuild\\Current\\Bin", "Machine")
    
    # 编译步骤
    cd C:\\vagrant_data
    
    # 编译IDL
    cd Interface
    midl /tlb IpcInterface.tlb /h IpcInterface.h IpcInterface.idl
    cd ..
    
    # 构建服务器和客户端
    msbuild ComIpc.sln /p:Configuration=Release /p:Platform=x64
    
    # 创建输出目录
    mkdir -p build
    cp ComIpcServer/x64/Release/ComIpcServer.exe build/
    cp ComIpcClient/x64/Release/ComIpcClient.exe build/
    cp README.md build/
  SHELL
end 