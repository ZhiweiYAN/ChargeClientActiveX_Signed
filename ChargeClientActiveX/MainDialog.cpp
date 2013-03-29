// MainDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ChargeClientActiveX.h"
#include "MainDialog.h"


// CMainDialog 对话框

IMPLEMENT_DYNAMIC(CMainDialog, CDialog)

CMainDialog::CMainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDialog::IDD, pParent)
{

}

CMainDialog::~CMainDialog()
{
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIANLOGO, m_logo);
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	ON_WM_CREATE()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CMainDialog 消息处理程序

int CMainDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CMainDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// todo:  在此添加额外的初始化
	if (m_logo.Load(MAKEINTRESOURCE(IDR_LIANLOGO),_T("GIF")))
		m_logo.Draw();

	return true;  // return true unless you set the focus to a control
	// 异常: ocx 属性页应返回 false
}
