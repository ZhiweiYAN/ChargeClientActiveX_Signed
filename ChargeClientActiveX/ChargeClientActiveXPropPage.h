#pragma once

// ChargeClientActiveXPropPage.h : CChargeClientActiveXPropPage ����ҳ���������


// CChargeClientActiveXPropPage : �й�ʵ�ֵ���Ϣ������� ChargeClientActiveXPropPage.cpp��

class CChargeClientActiveXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CChargeClientActiveXPropPage)
	DECLARE_OLECREATE_EX(CChargeClientActiveXPropPage)

// ���캯��
public:
	CChargeClientActiveXPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_CHARGECLIENTACTIVEX };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

