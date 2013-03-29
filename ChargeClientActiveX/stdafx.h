#pragma once
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ������뽫λ������ָ��ƽ̨֮ǰ��ƽ̨��ΪĿ�꣬���޸����ж��塣
// �йز�ͬƽ̨��Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ����ֵ����Ϊ�ʵ���ֵ����ָ���� Windows Me ����߰汾��ΪĿ�ꡣ
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

#include <afxctl.h>         // ActiveX �ؼ��� MFC ֧��
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

/*
// �����ϣ��ʹ�� MFC ���ݿ��࣬
//  ��ɾ����������������ļ�
#ifndef _WIN64

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#endif // _WIN64
*/
#include <afxwin.h>


//add common functions.2012-12-06
#define NEWLINE "\r\n"
static CString nByteTo16Str(DWORD dwSize, void* pData);

CString nByteTo16Str(DWORD dwSize, void* pData)
{
	BYTE* pBuf = (BYTE*)pData; // local pointer to a BYTE in the BYTE array.
	
	CString strRet("");	
	CString szTemp;
	
	for(DWORD i = 0; i < dwSize; i++)
	{
		DWORD dwRestTemp = pBuf[i];
		szTemp.Format(_T("%02x"),pBuf[i]);
		strRet=strRet + szTemp;
		DWORD dwByteBlock =strRet.GetLength() % (16*2);
		if(dwByteBlock==0)
		{
			strRet=strRet + _T(NEWLINE);
		}		
	}	
	return strRet;
}  // End of ByteToStr
static CString StrTo16Str(CString sz);
CString StrTo16Str(CString sz)
{
	CString strRet("");	
	CString szTemp;
	DWORD dwSize=sz.GetLength();
	
	for(DWORD i = 0; i < dwSize; i++)
	{
		DWORD dwRestTemp = sz.GetAt(i);
		szTemp.Format(_T("%02x"),dwRestTemp);
		strRet=strRet + szTemp;
		DWORD dwByteBlock =strRet.GetLength() % (16*2);
		if(dwByteBlock==0)
		{
			strRet=strRet + _T(NEWLINE);
		}		
	}	
	return strRet;
}  // End of ByteToStr



