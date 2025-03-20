#pragma once

// ATL头文件
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

// 导入接口定义
#import "..\Interface\IpcInterface.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids

// MessageService类 - 实现IMessageService接口
class ATL_NO_VTABLE CMessageService : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CMessageService, &CLSID_MessageService>,
    public IMessageService
{
public:
    CMessageService() 
    {
        // 构造函数
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_MESSAGESERVICE)
    DECLARE_NOT_AGGREGATABLE(CMessageService)

    BEGIN_COM_MAP(CMessageService)
        COM_INTERFACE_ENTRY(IMessageService)
        COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
        // 清理代码
    }

    // IMessageService接口实现
    STDMETHOD(SendMessage)(BSTR message, BSTR* response);
    STDMETHOD(SendData)(LONG data, LONG* result);
    STDMETHOD(GetStatus)(BSTR* status);
};

OBJECT_ENTRY_AUTO(__uuidof(MessageService), CMessageService) 