#pragma once
#include "UK_Et199Class.h"
class CUsbKeyOperation
{
public:
	CUK_Et199Class m_ET199;	//�ӽ���
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
	BOOL m_bServerPublic;//ʹ��Զ�̹�Կ

	void GetErroInfo(unsigned char revbuffer[],int reclen,CString &erro);
	////////////////////////////////////
	//��ʼ��UK����
	bool InitInstance();
	

	/////////////////////////////////////
	//�˳�UK����
	void ExitInstance();

	///////////////////////////////////
	//�Բ���
	bool SelfCheck(CString &info);

	////////////////////////////////////
	//��ȡ12λ��UKID len=12
	bool GetUKID(unsigned char ukid[],DWORD len,CString &info);

	///////////////////////////////////
	// �����ݰ�����ǩ�������ܴ�����ɢ�У���ǩ���������ܣ����γɴ�֡ͷ��ͨ�Ű�
	bool SignedEncrypt(unsigned char *in_buffer, int in_buffer_Len, unsigned char** out_buffer, int &out_buffer_Len);

	////////////////////////////////////
	// �Դ�֡ͷ��ͨ�Ű����н��ܴ����Ƚ��ܣ�����֤ǩ�������γ�������
    bool DecryptVerify(unsigned char *in_buffer, int in_buffer_Len, char &RetState,unsigned char** out_buffer, int &out_buffer_Len,CString &erro);

	///////////////////////////////////
	//��ȡ�ն�ID
	CString GetTerminalID(CString &info);

	///////////////////////////////////
	//��ȡuserID
	CString GetUserID(CString &info);
};
