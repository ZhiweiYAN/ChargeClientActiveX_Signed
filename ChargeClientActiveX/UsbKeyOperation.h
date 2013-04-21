#pragma once
#include "UK_Et199Class.h"



#define UKIDNAME "UKID"
#define Padding_Len 1

#define SHA1_LEN 20
#define SIGNATURE_LEN 128

#define UK_PIN "1uTZxyaR1TAP"

//ͨ��֡
#define SignedBZ "10"
#define BZ_LEN 2
#define TerminalID_LEN 8
#define UserID_LEN 4
#define UKID_LEN (TerminalID_LEN+UserID_LEN)
#define RESERVE_Char ' '
#define RESERVE_LEN 16+30
#define DATASize_LEN 4
//#define VERIFY_DATA_PKT_HDR_LEN (BZ_LEN+UKID_LEN+RESERVE_LEN+DATASize_LEN)
#define DECRYPT_BUFFER_PADDING 256

#define MSG_TYPE_LEN 2
#define TERMINAL_ID_LEN 8
#define WORKER_ID_LEN 4
#define ACK_INFO_LEN 44
#define PAYLOAD_LEN 6

#define ACK_INFO_POSITION 14
#define VERIFY_DATA_PKT_HDR_LEN (MSG_TYPE_LEN+TERMINAL_ID_LEN+WORKER_ID_LEN+ACK_INFO_LEN+ PAYLOAD_LEN)

#define NORMAL_MSG_CODE "00"
#define ERROR_MSG_CODE "11"
//#define VERIFY_DATA_PKT_HDR_LEN (BZ_LEN+UKID_LEN+RESERVE_LEN+DATASize_LEN)
#undef ERROR

typedef struct VerifyPktHdr{
	char msg_type[MSG_TYPE_LEN];
	char termina_id[TERMINAL_ID_LEN];
	char user_id[WORKER_ID_LEN];
	char ack_info[ACK_INFO_LEN];
	INT32 data_size;
} VerifyPktHdr;


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
	BOOL m_bServerPublic;//Զ����֤��������Կ

	void GetErroInfo(unsigned char revbuffer[],int reclen,CString &erro);
	////////////////////////////////////
	//��ʼ��UK����
	int InitInstance();
	

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
	//bool SignedEncrypt(unsigned char *in_buffer, int in_buffer_Len, unsigned char** out_buffer, int &out_buffer_Len);
	bool SignedEncryptPkt(int test_flag, unsigned char *in_buffer, int in_buffer_Len, unsigned char** out_buffer, int &out_buffer_Len);


	////////////////////////////////////
	// �Դ�֡ͷ��ͨ�Ű����н��ܴ����Ƚ��ܣ�����֤ǩ�������γ�������
	int DecryptVerifyPkt(int test_flag, unsigned char *in_buffer, int in_buffer_Len, char &RetState,unsigned char** out_buffer, int &out_buffer_len,CString &erro);

	///////////////////////////////////
	//��ȡ�ն�ID
	CString GetTerminalID(CString &info);

	///////////////////////////////////
	//��ȡuserID
	CString GetUserID(CString &info);
};
