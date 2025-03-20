#include "MessageService.h"
#include <string>
#include <atlstr.h>

// 实现SendMessage方法
STDMETHODIMP CMessageService::SendMessage(BSTR message, BSTR* response)
{
    // 检查参数
    if (!response)
        return E_POINTER;

    // 处理输入消息
    CComBSTR inputMsg(message);
    CComBSTR respMsg(L"服务器已收到消息: ");
    respMsg += inputMsg;
    
    // 打印接收到的消息 (用于调试)
    wprintf(L"服务器接收到消息: %s\n", (LPCWSTR)inputMsg);
    
    // 输出响应
    *response = respMsg.Detach();
    return S_OK;
}

// 实现SendData方法
STDMETHODIMP CMessageService::SendData(LONG data, LONG* result)
{
    // 检查参数
    if (!result)
        return E_POINTER;

    // 对输入数据进行处理 (示例为计算平方)
    *result = data * data;
    
    // 打印接收到的数据 (用于调试)
    wprintf(L"服务器接收到数据: %d，处理结果: %d\n", data, *result);
    
    return S_OK;
}

// 实现GetStatus方法
STDMETHODIMP CMessageService::GetStatus(BSTR* status)
{
    // 检查参数
    if (!status)
        return E_POINTER;

    // 返回服务器状态
    CComBSTR serverStatus(L"COM服务器运行正常 - 当前时间: ");
    
    // 获取当前时间
    SYSTEMTIME st;
    GetLocalTime(&st);
    
    // 格式化时间字符串
    wchar_t timeStr[64];
    swprintf_s(timeStr, 64, L"%04d-%02d-%02d %02d:%02d:%02d", 
               st.wYear, st.wMonth, st.wDay,
               st.wHour, st.wMinute, st.wSecond);
    
    serverStatus += timeStr;
    
    // 输出状态
    *status = serverStatus.Detach();
    return S_OK;
} 