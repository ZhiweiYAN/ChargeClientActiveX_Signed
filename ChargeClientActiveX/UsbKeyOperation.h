#pragma once
#include "UK_Et199Class.h"
class CUsbKeyOperation
{
public:
	CUK_Et199Class m_ET199;	//加解密
public:
	CUsbKeyOperation(void);
public:
	~CUsbKeyOperation(void);
	
private:
	union unit_data
	{
		UINT64 myuint64;//8
		INT64 myint64;//8
		DWORD myDWORD[2];//
		char mychars[8];//1
		BYTE mybytes[8];//1
		INT32 myint32;//4
		int myint; //4
		unsigned long myUlong;
		double myDouble;
		HWND my_hWnd;
		
	}TempData;

	wchar_t wchar[10000]; 
public:
	BOOL m_bServerPublic;//使用远程公钥

	void GetErroInfo(unsigned char revbuffer[],int reclen,CString &erro);
	////////////////////////////////////
	//初始化UK环境
	bool InitInstance();
	

	/////////////////////////////////////
	//退出UK环境
	void ExitInstance();

	///////////////////////////////////
	//自测试
	bool SelfCheck(CString &info);

	////////////////////////////////////
	//获取12位的UKID len=12
	bool GetUKID(unsigned char ukid[],DWORD len,CString &info);

	///////////////////////////////////
	// 对数据包进行签名、加密处理（先散列，再签名，最后加密）后，形成带帧头的通信包
	bool SignedEncrypt(unsigned char *in_buffer, int in_buffer_Len, unsigned char** out_buffer, int &out_buffer_Len);

	////////////////////////////////////
	// 对带帧头的通信包进行解密处理（先解密，再验证签名）后，形成数据区
    bool DecryptVerify(unsigned char *in_buffer, int in_buffer_Len, char &RetState,unsigned char** out_buffer, int &out_buffer_Len,CString &erro);

	///////////////////////////////////
	//获取终端ID
	CString GetTerminalID(CString &info);

	///////////////////////////////////
	//获取userID
	CString GetUserID(CString &info);
};
