// MainDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChargeClientActiveX.h"
#include "MainDialog.h"


// CMainDialog �Ի���

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


// CMainDialog ��Ϣ�������

int CMainDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}

void CMainDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// todo:  �ڴ���Ӷ���ĳ�ʼ��
	if (m_logo.Load(MAKEINTRESOURCE(IDR_LIANLOGO),_T("GIF")))
		m_logo.Draw();

	return true;  // return true unless you set the focus to a control
	// �쳣: ocx ����ҳӦ���� false
}
