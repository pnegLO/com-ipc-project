# 使用Windows容器基础镜像
FROM mcr.microsoft.com/windows/servercore:ltsc2019

# 安装构建工具
SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]

# 安装Visual Studio Build Tools
RUN Invoke-WebRequest -OutFile vs_buildtools.exe -Uri https://aka.ms/vs/16/release/vs_buildtools.exe; \
    Start-Process -FilePath vs_buildtools.exe -ArgumentList '--quiet', '--wait', '--norestart', '--nocache', \
    '--installPath', 'C:\BuildTools', \
    '--add', 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64', \
    '--add', 'Microsoft.VisualStudio.Component.Windows10SDK.19041', \
    '--add', 'Microsoft.VisualStudio.Component.VC.ATL', \
    '--add', 'Microsoft.VisualStudio.Component.VC.ATLMFC' \
    -NoNewWindow -Wait; \
    Remove-Item -Force vs_buildtools.exe

# 设置工作目录
WORKDIR C:/src

# 复制项目文件
COPY . .

# 编译IDL
RUN cd Interface && \
    & 'C:\BuildTools\VC\Auxiliary\Build\vcvarsall.bat' x64 && \
    midl /tlb IpcInterface.tlb /h IpcInterface.h IpcInterface.idl

# 构建服务器和客户端
RUN & 'C:\BuildTools\VC\Auxiliary\Build\vcvarsall.bat' x64 && \
    cd ComIpcServer && \
    msbuild ComIpcServer.vcxproj /p:Configuration=Release /p:Platform=x64 && \
    cd ../ComIpcClient && \
    msbuild ComIpcClient.vcxproj /p:Configuration=Release /p:Platform=x64

# 创建输出目录
RUN mkdir -p C:/output && \
    cp ComIpcServer/x64/Release/ComIpcServer.exe C:/output/ && \
    cp ComIpcClient/x64/Release/ComIpcClient.exe C:/output/ && \
    cp README.md C:/output/

# 设置卷
VOLUME C:/output

# 设置入口点
ENTRYPOINT ["powershell"] 