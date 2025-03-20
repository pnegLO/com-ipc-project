#include <windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <iostream>
#include <string>

// 导入接口定义
#import "..\Interface\IpcInterface.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids

// 显示错误消息
void ShowError(HRESULT hr, const wchar_t* operation)
{
    wchar_t errorMsg[256] = { 0 };
    swprintf_s(errorMsg, 256, L"%s 失败，错误码: 0x%08X", operation, hr);
    std::wcout << errorMsg << std::endl;
}

// COM客户端主函数
int main()
{
    // 设置控制台输出为中文
    setlocale(LC_ALL, "chs");
    std::wcout << L"COM IPC 客户端正在启动...\n" << std::endl;
    
    HRESULT hr = S_OK;
    
    // 初始化COM
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) 
    {
        ShowError(hr, L"初始化COM");
        return -1;
    }
    
    // 智能指针，确保资源自动释放
    CComPtr<IMessageService> spMessageService;
    
    // 创建COM组件实例
    hr = spMessageService.CoCreateInstance(CLSID_MessageService);
    if (FAILED(hr))
    {
        ShowError(hr, L"创建COM组件实例");
        CoUninitialize();
        return -1;
    }
    
    std::wcout << L"成功连接到COM服务器\n" << std::endl;
    
    bool running = true;
    while (running)
    {
        std::wcout << L"\n选择操作:\n";
        std::wcout << L"1. 发送消息\n";
        std::wcout << L"2. 发送数据\n";
        std::wcout << L"3. 获取服务器状态\n";
        std::wcout << L"0. 退出\n";
        std::wcout << L"请输入选项: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice)
        {
            case 1: // 发送消息
            {
                std::wcout << L"请输入要发送的消息: ";
                std::wstring message;
                std::cin.ignore(); // 清除之前的换行符
                std::getline(std::wcin, message);
                
                // 发送消息
                CComBSTR bstrMessage(message.c_str());
                CComBSTR bstrResponse;
                hr = spMessageService->SendMessage(bstrMessage, &bstrResponse);
                
                if (SUCCEEDED(hr))
                {
                    std::wcout << L"服务器响应: " << (LPCWSTR)bstrResponse << std::endl;
                }
                else
                {
                    ShowError(hr, L"发送消息");
                }
                break;
            }
            
            case 2: // 发送数据
            {
                std::wcout << L"请输入要发送的整数: ";
                LONG data;
                std::cin >> data;
                
                // 发送数据
                LONG result;
                hr = spMessageService->SendData(data, &result);
                
                if (SUCCEEDED(hr))
                {
                    std::wcout << L"服务器计算结果: " << result << std::endl;
                }
                else
                {
                    ShowError(hr, L"发送数据");
                }
                break;
            }
            
            case 3: // 获取服务器状态
            {
                CComBSTR bstrStatus;
                hr = spMessageService->GetStatus(&bstrStatus);
                
                if (SUCCEEDED(hr))
                {
                    std::wcout << L"服务器状态: " << (LPCWSTR)bstrStatus << std::endl;
                }
                else
                {
                    ShowError(hr, L"获取服务器状态");
                }
                break;
            }
            
            case 0: // 退出
                running = false;
                break;
                
            default:
                std::wcout << L"无效选项，请重新输入\n";
                break;
        }
    }
    
    // 释放COM资源
    spMessageService.Release();
    CoUninitialize();
    
    std::wcout << L"\nCOM IPC 客户端已退出。\n";
    return 0;
} 