// ChargeClientActiveXPropPage.cpp : CChargeClientActiveXPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "ChargeClientActiveX.h"
#include "ChargeClientActiveXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CChargeClientActiveXPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CChargeClientActiveXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CChargeClientActiveXPropPage, "CHARGECLIENTAC.ChargeClientAcPropPage.1",
	0xf0d01f85, 0xc851, 0x4f40, 0xa7, 0xd, 0x7c, 0x13, 0xe3, 0x7c, 0x87, 0x83)



// CChargeClientActiveXPropPage::CChargeClientActiveXPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CChargeClientActiveXPropPage ��ϵͳע�����

BOOL CChargeClientActiveXPropPage::CChargeClientActiveXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CHARGECLIENTACTIVEX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CChargeClientActiveXPropPage::CChargeClientActiveXPropPage - ���캯��

CChargeClientActiveXPropPage::CChargeClientActiveXPropPage() :
	COlePropertyPage(IDD, IDS_CHARGECLIENTACTIVEX_PPG_CAPTION)
{
}



// CChargeClientActiveXPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CChargeClientActiveXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CChargeClientActiveXPropPage ��Ϣ�������
