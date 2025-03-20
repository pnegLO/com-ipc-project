#include <windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlsafe.h>
#include <iostream>

// 导入COM服务定义
#include "MessageService.h"

// ATL模块
class CComIpcModule : public ATL::CAtlExeModuleT<CComIpcModule>
{
public:
    DECLARE_LIBID(LIBID_IpcServerLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MESSAGESERVICE, "{982860CF-3A5E-4ED0-B407-D9C99E4FB12F}")
};

CComIpcModule _AtlModule;

// 应用程序入口点
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    // 打开控制台窗口以显示调试输出
    AllocConsole();
    FILE* pFile = nullptr;
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    
    std::wcout << L"COM IPC 服务器正在启动...\n";
    
    // 初始化COM
    HRESULT hr = _AtlModule.WinMain(nShowCmd);
    
    if (FAILED(hr))
    {
        std::wcout << L"COM服务器启动失败，错误码: " << hr << L"\n";
    }
    
    // 释放控制台
    if (pFile)
    {
        fclose(pFile);
    }
    FreeConsole();
    
    return hr;
}

// 命令行入口点，转发到Windows入口点
extern "C" int WINAPI main(int /*argc*/, char* /*argv*/[])
{
    return _AtlModule.WinMain(SW_SHOWDEFAULT);
} 