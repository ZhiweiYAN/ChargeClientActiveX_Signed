#pragma once

// ChargeClientActiveXPropPage.h : CChargeClientActiveXPropPage 属性页类的声明。


// CChargeClientActiveXPropPage : 有关实现的信息，请参阅 ChargeClientActiveXPropPage.cpp。

class CChargeClientActiveXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CChargeClientActiveXPropPage)
	DECLARE_OLECREATE_EX(CChargeClientActiveXPropPage)

// 构造函数
public:
	CChargeClientActiveXPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_CHARGECLIENTACTIVEX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

