// ChargeClientActiveXPropPage.cpp : CChargeClientActiveXPropPage 属性页类的实现。

#include "stdafx.h"
#include "ChargeClientActiveX.h"
#include "ChargeClientActiveXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CChargeClientActiveXPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CChargeClientActiveXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CChargeClientActiveXPropPage, "CHARGECLIENTAC.ChargeClientAcPropPage.1",
	0xf0d01f85, 0xc851, 0x4f40, 0xa7, 0xd, 0x7c, 0x13, 0xe3, 0x7c, 0x87, 0x83)



// CChargeClientActiveXPropPage::CChargeClientActiveXPropPageFactory::UpdateRegistry -
// 添加或移除 CChargeClientActiveXPropPage 的系统注册表项

BOOL CChargeClientActiveXPropPage::CChargeClientActiveXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CHARGECLIENTACTIVEX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CChargeClientActiveXPropPage::CChargeClientActiveXPropPage - 构造函数

CChargeClientActiveXPropPage::CChargeClientActiveXPropPage() :
	COlePropertyPage(IDD, IDS_CHARGECLIENTACTIVEX_PPG_CAPTION)
{
}



// CChargeClientActiveXPropPage::DoDataExchange - 在页和属性间移动数据

void CChargeClientActiveXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CChargeClientActiveXPropPage 消息处理程序
