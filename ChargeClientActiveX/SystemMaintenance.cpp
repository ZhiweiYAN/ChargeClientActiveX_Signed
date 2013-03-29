#include "StdAfx.h"
#include "SystemMaintenance.h"
#include <windows.h>
#include <strsafe.h>
#include <curl/curl.h>
#include "afxsock.h"
//#include "io.h"
#include "MacroDefinition.h"
//#include "UsbSignature.h"
#pragma warning(push)
#pragma warning(disable:4995)
#undef ERROR
#include <glog/logging.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(pop)

size_t write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}

CSystemMaintenance::CSystemMaintenance(void)
{
}

CSystemMaintenance::~CSystemMaintenance(void)
{
}

// ad_flag, 1, 显示版权信息, 0 不显示版权信息
int CSystemMaintenance::UpdateDbFile(CString ip_addr, int ad_flag)
{
		
	int err = 0;
	int ret = 0;
	DWORD  dwCode = 0;

	fflush(NULL);

	//delete file, success
	err = ::DeleteFile(_T(DB_FILE));
	USES_CONVERSION;

	CString cmd_str(DB_URL);
	cmd_str.Replace(_T(TureURL),ip_addr);
	char *cmd_buf = T2A(cmd_str);

	/*
	PROCESS_INFORMATION ProcessInfo; 
	STARTUPINFO StartupInfo; //This is an [in] parameter
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo)  ; //Only compulsory field
	if(CreateProcess(NULL, A2W(cmd_buf), NULL,NULL,
		FALSE,REALTIME_PRIORITY_CLASS|CREATE_NO_WINDOW,NULL, NULL,&StartupInfo,&ProcessInfo))
	{
		
		WaitForSingleObject(ProcessInfo.hProcess,INFINITE);
		GetExitCodeProcess(ProcessInfo.hProcess, &dwCode);
		CloseHandle(ProcessInfo.hThread);
		CloseHandle(ProcessInfo.hProcess);
		ret = dwCode;
	}
	else
	{
		ErrorExit(_T("ERROR:"));
		ret = -1;
	}
	*/
	//err = system(T2A(cmd_str));

	CURL *easyhandle = NULL;
	CURLcode res;

	easyhandle = curl_easy_init();

	if(easyhandle) {

		FILE *out_file = NULL;
		err = fopen_s(&out_file, DB_FILE, "wb");

		curl_easy_setopt(easyhandle, CURLOPT_URL, cmd_buf);
		curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, out_file);
		curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_func); 

		res = curl_easy_perform(easyhandle);
		if(res==0){
			ret = 0;
		}
		else{
			ret = -1;
		}
		fclose(out_file);
		out_file = NULL;

		/* always cleanup */
		curl_easy_cleanup(easyhandle);
	}
	else{
		ret = -1;

	}
	fflush(NULL);

	if(1==ad_flag){
		int ret_2 = system("color 17 && echo \"正在升级,请稍候.  \Updating Packet Templates... Now.\" \
						   && echo ====================================================== \
						   && echo  本软件 2011 卓题实验室(DrumTm Lab, drumtm.com) 保留所有权利. \
						   && echo  如果您有疑问, 请联系我们(ssurui@gamil.com, jerod.yan@gmail.com). \
						   && echo ====================================================== ");		
		fflush(NULL);
		Sleep(TIME_OUT_INTERVAL);
	}

	return ret;
}

void CSystemMaintenance::ErrorExit(LPTSTR lpszFunction)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	//ExitProcess(dw); 
	return ;
}


void CSystemMaintenance::ExitProc(void)
{
	ExitProcess(-1);
}
void CSystemMaintenance::ResetSystem(void)
{
	int ret = 0;
	
	ret = ::MoveFileEx(_T(OCX_FILE),NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
	ret = ::MoveFileEx(_T(DB_FILE), NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
	ret = ::MoveFileEx(_T(INF_FILE), NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
	ret = ::MoveFileEx(_T(ICCARD_FILE_1), NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
	ret = ::MoveFileEx(_T(ICCARD_FILE_2), NULL,MOVEFILE_DELAY_UNTIL_REBOOT);

	system(CMD_REMOVE_LOGS);
	system(CMD_SHUTDOWN);
	return;
}
int CSystemMaintenance::ClearLogFiles(void)
{
	int ret = 0;

	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

	FileOp.fFlags = FOF_NOCONFIRMATION|FOF_SILENT;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = _T(CHARGE_CLIENT_LOG_FILES);
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;

	ret = SHFileOperation(&FileOp);

	return 0;
}

int CSystemMaintenance::DeleteFiles(char *files_name)
{
	int ret = 0;
	USES_CONVERSION;

	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

	FileOp.fFlags = FOF_NOCONFIRMATION|FOF_SILENT|FOF_FILESONLY ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = A2T("c:\\charge_client\0\0");
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;

	ret = SHFileOperation(&FileOp);

	return 0;
}

//
//int CSystemMaintenance::CheckUKey(CString &usb_token_name)
//{
//	CUsbSignature usb_token;
//	int ret = 0;
//
//	ret = usb_token.GetTokenName(usb_token_name);
//	if(-1 == ret){
//		LOG(ERROR)<<"While GetTokenName, failed. Sorry.";
//		return -1;
//	}
//
//	char *signature = NULL;
//	int signature_len = 0;
//	signature = (char *)malloc(MAX_SIGNATURE_LEN);
//	CHECK(NULL!=signature)<<"Memory allocation, failed.";
//	memset(signature, 0, MAX_SIGNATURE_LEN);
//	
//	ret = usb_token.GetSignData("DrumTm_Products", 15, signature, &signature_len);
//
//	if(NULL!=signature){
//		free(signature);
//		signature = NULL;
//		signature_len = 0;
//	}
//
//	if(-1 == ret){
//		LOG(ERROR)<<"While GetSignData digital signature, failed. Sorry.";
//		return -1;
//	}
//	else{
//		LOG(INFO)<<"While GetSignData digital signature, success. Good." ;
//		return 1;
//	}
//}
