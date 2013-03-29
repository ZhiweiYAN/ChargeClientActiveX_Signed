#pragma once
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果必须将位于下面指定平台之前的平台作为目标，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

#include <afxctl.h>         // ActiveX 控件的 MFC 支持
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

/*
// 如果不希望使用 MFC 数据库类，
//  请删除下面的两个包含文件
#ifndef _WIN64

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO 数据库类
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



