#pragma once
#include "stdafx.h"
class CSystemMaintenance
{
public:
	CSystemMaintenance(void);
public:
	~CSystemMaintenance(void);
public:
	int UpdateDbFile(CString ip_addr,int ad_flag);
	int DeleteFiles(char *files_name);
	void ErrorExit(LPTSTR lpszFunction);
	void ResetSystem(void);
	void ExitProc(void);
	int ClearLogFiles(void);
public:
	//int CheckUKey(CString &usb_token_name);

};
