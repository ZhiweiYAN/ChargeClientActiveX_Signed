#pragma once
#include "stdafx.h"
#include "maindialog.h"
#include "dotprinter.h"
#include "systemmaintenance.h"
#include "MacroDefinition.h"
#include "berkeleydb.h"
#include "Network.h"
//#include "usbsignature.h"
#include "UsbKeyOperation.h"


static int init_glog_once =0 ;
//static int transcation_token = 0;
static int download_db_flag = 0;

static int init_UKInit_once =0 ;

typedef enum
{SysMain_ClearLog=0, SysMain_AccessLocalDb, SysMain_Printer,  
	SysMain_DigiSign, SysMain_Tcp, 
	SysMain_UpdateLocalDb, SysMain_RestoreFactory=911
}SysMainTestOptions;

// ChargeClientActiveXCtrl.h : CChargeClientActiveXCtrl ActiveX 控件类的声明。


// CChargeClientActiveXCtrl : 有关实现的信息，请参阅 ChargeClientActiveXCtrl.cpp。

class CChargeClientActiveXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CChargeClientActiveXCtrl)

// 构造函数
public:
	CChargeClientActiveXCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();

// 实现
protected:
	~CChargeClientActiveXCtrl();

	DECLARE_OLECREATE_EX(CChargeClientActiveXCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CChargeClientActiveXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CChargeClientActiveXCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CChargeClientActiveXCtrl)		// 类型名称和杂项状态

	// 子类控件支持
	BOOL IsSubclassedControl();
	LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidGetUserID = 42L,
		dispidGetTerminalID = 41L,
		dispidActivexErrorInfo = 40,
		dispidActivexErrorCode = 39,
		dispidPrintInvoice = 38L,
		dispidPrepareInvoiceItem = 37L,
		dispidTestingFunction = 36L,
		dispidWebOrder = 35,
		dispidOutputFields = 34,
		//dispidRemoveTemplate = 33L,
		dispidInputFields = 32,
		//dispidAddTemplate = 8L,
		dispidInputParameterActName = 7,
		dispidInputParameterComName = 6,
		eventidDisplayAtPosition = 5L,
		eventidDisplayInternetWorking = 4L,
		eventidDisplayLocalDbWorking = 3L,
		eventidDisplayActivexWorking = 2L,
		eventidParameterLoaded = 1L,
		dispidLoadParameter = 4L,
		dispidOutputParameterA = 3,
		dispidInputParameterDebugFlag = 2,
		dispidInputParameterA = 1
	};
public:
	CMainDialog m_MainDialog;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	void OnInputParameterComNameChanged(void);
	CString m_InputParameterComName;

	void OnInputParameterActNameChanged(void);
	CString m_InputParameterActName;

	void OnInputParameterAChanged(void);
	CString m_InputParameterA;

	void OnInputParameterDebugFlagChanged(void);
	CString m_InputParameterDebugFlag;
	//void OnInputParameterCChanged(void);

	void OnOutputParameterAChanged(void);
	CString m_OutputParameterA;

	void OnActivexErrorCodeChanged(void);
	LONG m_ActivexErrorCode;

	void OnActivexErrorInfoChanged(void);
	CString m_ActivexErrorInfo;

	void OnWebOrderChanged(void);
	CString m_WebOrder;

private:
		CString m_FieldValueBuffer[MAX_PACKET_FIELD_NUM];
		CString m_InputFieldBuffer[MAX_PACKET_FIELD_NUM];
		CString m_OutputFieldBuffer[MAX_PACKET_FIELD_NUM];

protected:

	void FireParameterLoaded(void)
	{
		FireEvent(eventidParameterLoaded, EVENT_PARAM(VTS_NONE));
	}


	void FireDisplayActivexWorking(void)
	{
		FireEvent(eventidDisplayActivexWorking, EVENT_PARAM(VTS_NONE));
	}

	void FireDisplayLocalDbWorking(void)
	{
		FireEvent(eventidDisplayLocalDbWorking, EVENT_PARAM(VTS_NONE));
	}

	void FireDisplayInternetWorking(void)
	{
		FireEvent(eventidDisplayInternetWorking, EVENT_PARAM(VTS_NONE));
	}

	void FireDisplayAtPosition(LONG position_idx)
	{
		FireEvent(eventidDisplayAtPosition, EVENT_PARAM(VTS_I4), position_idx);
	}


//输出给Javascript脚本使用的函数_BEGIN
protected:
	void LoadParameter(void);
	/*void AddTemplate(LPCTSTR key, LONG key_len, LPCTSTR data, LONG data_Len);
	void RemoveTemplate(LPCTSTR key, LONG key_len);*/
	void TestingFunction(LONG f);

	void PrepareInvoiceItem(LONG x, LONG y, LPCTSTR data, LONG data_len);
	void PrintInvoice(void);

protected:
	BSTR GetInputFields(SHORT field_index);
	void SetInputFields(SHORT field_index, LPCTSTR newVal);
	BSTR GetOutputFields(SHORT field_index);
	void SetOutputFields(SHORT field_index, LPCTSTR newVal);
	void ClearOutputFields(void);
//输出给Javascript脚本使用的函数_END

public:
	void DisplayDebugInfoToWebPage(CString  info);
	void DisplayBackToWebPage(CString str, long positon);
	void SetErrorInfo4Web(long err_code);

private:
	int GetCStringBytesNumber(CString str);
    int GetPacketTemplateFieldNumberEx(CString pkt_template);
	int GetPacketTemplateFieldLength(CString pkt_template, int field_num, int *len_array);
	int FormSendingPacket(CString & pkt_data, CString pkt_template, int field_num);
	int SplitRecvingPacket(CString pkt_data, CString pkt_template, int field_num);

private:
	CString m_tcp_recv_buff;
	CString m_tcp_send_buff;

private:
	int GetDbVersionID(void);
	//int ScanTemplateDB(char * database);

private:
	CDotPrinter m_dot_printer;
	//CVerifySign m_verify_signature;
	CSystemMaintenance m_sys;
	CBerkeleyDB m_db;
	CNetwork m_net;
private:
	//CUsbSignature m_signature;
	CUsbKeyOperation m_usbkey;
protected:
	BSTR GetTerminalID(void);
	BSTR GetUserID(void);
};

