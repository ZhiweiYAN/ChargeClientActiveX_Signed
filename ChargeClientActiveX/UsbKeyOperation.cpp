#include "StdAfx.h"
#include "UsbKeyOperation.h"

#undef ERROR
#include <glog/logging.h>

#define UKIDNAME "UKID"
#define Padding_Len 1

#define SHA1_LEN 20
#define SignedData_LEN 128

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
#define DATA_BEGIN_POS (BZ_LEN+UKID_LEN+RESERVE_LEN+DATASize_LEN)
CUsbKeyOperation::CUsbKeyOperation(void)
{
}

CUsbKeyOperation::~CUsbKeyOperation(void)
{
}


bool CUsbKeyOperation::InitInstance(void)
{
	//����ET199�Ŀ��ļ������л�����ʼ��
	CK_RV rv;
	// Load PKCS#11 library
	rv = C_Initialize(NULL);
	if(CKR_OK != rv)
	{
		LOG(ERROR)<<"Can't Load PKCS#11 Library!";
		//AfxMessageBox(_T("Can't Load PKCS#11 Library!"), MB_OK | MB_ICONERROR);
		return false;
	}
	else
	{
		LOG(INFO)<<"Load PKCS#11 Library ok";
		//AfxMessageBox(_T("Load PKCS#11 Library ok"));
	}
	m_bServerPublic=TRUE;
	return true;

}
void CUsbKeyOperation::ExitInstance(void)
{
	C_Finalize(NULL);
}
bool CUsbKeyOperation::SelfCheck(CString &info)
{
	USES_CONVERSION;
	CString test_plain_txt("1234567890ABCDEF");
	int test_plain_txt_len=test_plain_txt.GetLength();
	unsigned char *pstr=(unsigned char *)test_plain_txt.GetBuffer(test_plain_txt_len);
	unsigned char *pSignedBuffer=NULL;//��Ҫ��������
	int Signedbuffer_len=0;

	//����
	//ǿ��ʹ�ñ��ع�Կ
	m_bServerPublic=FALSE;
	if(SignedEncrypt(pstr,test_plain_txt_len,(unsigned char**)&pSignedBuffer,Signedbuffer_len)==false)
	{
		info= info + _T("USB KEY �Լ�, ǩ��, ���� ʧ��.");
		LOG(ERROR)<<T2A(info);
		if(NULL != pSignedBuffer)
		{
			free(pSignedBuffer);
			pSignedBuffer=NULL;
		}
		m_bServerPublic=TRUE;//�ָ�ʹ��Զ�̷�������Կ
		return false;
	}
	
	//here for general use to handle with verify packet header, maybe change it later.
	pSignedBuffer[BZ_LEN+UKID_LEN]='0';
	
	//�����Ľ���
	unsigned char *pBuffer=NULL;//��Ҫ��������
	int buffer_len=0;
	char bz;
	if(DecryptVerify(pSignedBuffer,Signedbuffer_len,bz,(unsigned char**)&pBuffer,buffer_len,info)==false)
	{		
		info= info + _T("USB KEY �Լ�, ����, ��ǩ ʧ��.");
        LOG(ERROR)<<T2A(info);
		if(NULL!=pSignedBuffer)
		{
			free(pSignedBuffer);
			pSignedBuffer=NULL;
		}
		if(NULL!=pBuffer)
		{
			free(pBuffer);
			pBuffer=NULL;
		}
		m_bServerPublic=TRUE;//�ָ�ʹ��Զ�̷�������Կ
		return false;
	}
	m_bServerPublic=TRUE;//�ָ�ʹ��Զ�̷�������Կ

	
	//���һ����
	for(int i=0;i<buffer_len;i++)
	{
		if(pBuffer[i]!=pstr[i])
		{
			info=info+_T("ԭʼ��������ܺ������������ݲ�һ��");
            LOG(ERROR)<<info;
			LOG(ERROR)<<"Origin Plain Text:"<<(pstr);
			LOG(ERROR)<<"Decrypted Text:"<<(pBuffer);
			return false;
		}
	}

	if(pSignedBuffer!=NULL)
	{
		free(pSignedBuffer);
		pSignedBuffer=NULL;
	}
	if(pBuffer!=NULL)
	{
		free(pBuffer);
		pBuffer=NULL;
	}
	
	CString terminal_id;
	CString user_id;
	CString err_info;

	terminal_id.Empty();
	user_id.Empty();

	terminal_id = GetTerminalID(err_info);
	user_id = GetUserID(err_info);



	info=info + _T("USBKey ID: ")+terminal_id + user_id;

	return true;

	
}
bool CUsbKeyOperation::GetUKID(unsigned char ukid[],DWORD len,CString &info)
{
	memset(ukid,NULL,len);
	USES_CONVERSION;
	//���ȣ�UK�豸����
	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return false;
	}	
	if(m_ET199.GetData(_T(UKIDNAME),TRUE,ukid,len,info)!=UKID_LEN)
	{
		LOG(ERROR)<<T2A(info);

		//AfxMessageBox(info);
		return false;
	}
	//���Ͽ��豸
	m_ET199.DisConnectDev();
	return true;
}

// �����ݰ����м��ܴ�����ɢ�У���ǩ���������ܣ����γɴ�֡ͷ��ͨ�Ű�
bool CUsbKeyOperation::SignedEncrypt(unsigned char *in_buffer, int in_buffer_Len, unsigned char** out_buffer, int &out_buffer_Len)
{

	USES_CONVERSION;
	/*char * key_buffer = NULL;
	key_buffer = (char *)malloc(key_length);*/
	CString info;//��ʾ��Ϣ
	if(in_buffer_Len<1 || in_buffer==NULL)
	{
		LOG(ERROR)<<"ԭʼ���ݳ���Ϊ0���޷�����";		
		return false;
	}
	//���ȣ�UK�豸����
	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return false;
	}	
	/////////////////////////////////////////////////////////////
	//��һ������ȡUKID��	
	unsigned char UKIDStr[UKID_LEN+1];
	memset(UKIDStr,NULL,UKID_LEN+1);
	if(m_ET199.GetData(_T(UKIDNAME),TRUE,UKIDStr,UKID_LEN,info)!=UKID_LEN)
	{
		LOG(ERROR)<<T2A(info);
		m_ET199.DisConnectDev();
		//AfxMessageBox(info);
		return false;
	}

	//�ڶ����������ݽ���ɢ�У�ɢ�к����ݳ���=SHA1_LEN
	unsigned char m_MsgSHA1[SHA1_LEN+1];
	memset(m_MsgSHA1,NULL,SHA1_LEN+1);
	DWORD MsgSHA1_Len=SHA1_LEN;
	if(m_ET199.RSA_Digest(in_buffer,in_buffer_Len,m_MsgSHA1,SHA1_LEN+1,MsgSHA1_Len,info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);	
		m_ET199.DisConnectDev();
		return false;
	}
	LOG(INFO)<<"ɢ��:"<<T2A(CString(m_MsgSHA1));
	LOG(INFO)<<"ɢ�д���(16):"<<T2A(nByteTo16Str(MsgSHA1_Len,m_MsgSHA1));
	//����������ɢ�����ݽ���ǩ��
	unsigned char *m_signeddata=new unsigned char[SignedData_LEN+in_buffer_Len+1];	
	memset(m_signeddata,NULL,SignedData_LEN+in_buffer_Len+1);
	//3.1 ����ǩ��
	DWORD SignedData_Len=SignedData_LEN+1;
	if(m_ET199.RSA_Signed(m_MsgSHA1,MsgSHA1_Len,m_signeddata,SignedData_Len,info)==FALSE)
	{
		if(m_signeddata!=NULL)
		{
			delete[] m_signeddata;
			m_signeddata=NULL;
		}
		LOG(ERROR)<<T2A(info);
		m_ET199.DisConnectDev();
		return false;
	}
	//3.2 ǩ��+ԭ��Ϣ = ��ϳ�ǩ������Ϣ��
	memcpy(m_signeddata+SignedData_Len,in_buffer,in_buffer_Len);	
	//LOG(INFO)<<"ǩ��:"<<T2A(CString(m_signeddata));
	LOG(INFO)<<"ǩ��(16):"<<T2A(nByteTo16Str(SignedData_LEN+in_buffer_Len,m_signeddata));
	//���Ĳ�����ǩ������Ϣ�����м���
	DWORD Encrypt_Len=SignedData_LEN+in_buffer_Len+SignedData_LEN;//Ԥ����ܳ��ȱ�ԭ�ĳ��Ȳ��ᳬ��128��
    unsigned char *m_Encryptdata=new unsigned char[Encrypt_Len+1];	
	memset(m_Encryptdata,NULL,Encrypt_Len+1);
	

	if(m_ET199.RSA_Encrypt(m_bServerPublic,m_signeddata,SignedData_LEN+in_buffer_Len,m_Encryptdata,Encrypt_Len,info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		if(m_signeddata!=NULL)
		{
			delete[] m_signeddata;
			m_signeddata=NULL;
		}
		if(m_Encryptdata!=NULL)
		{
			delete[] m_Encryptdata;
			m_Encryptdata=NULL;
		}
		m_ET199.DisConnectDev();
		return false;
	}
	m_Encryptdata[Encrypt_Len]=NULL;
	//LOG(INFO)<<"����:"<<T2A(CString(m_Encryptdata));
	LOG(INFO)<<"����(16):"<<T2A(nByteTo16Str(Encrypt_Len,m_Encryptdata));
	//���岽�����һ��34�ֽ�֡ͷ
	//ͨ��֡    = ֡ͷ+������
	//֡ͷ      = 2�ֽڣ���־�����빫Կ=��00��;��֤UKID="01"��+12�ֽڣ�UKID��+16�ֽڣ�������+4�ֽڣ��������ĳ��ȣ�
	out_buffer_Len=DATA_BEGIN_POS+Encrypt_Len;
	*out_buffer= (unsigned char *)malloc(out_buffer_Len);
	memset(*out_buffer,NULL,out_buffer_Len);
	//5.1  "10"�Ǽ��ܽ��ױ�־
	/*memset(*out_buffer,'1',1);
	memset(*out_buffer+1,'0',1);*/
	memcpy(*out_buffer,SignedBZ,BZ_LEN);
	/*(*out_buffer)[0]='1';
	(*out_buffer)[1]='0';*/
	//5.2  ����UKID	 
	memcpy((*out_buffer)+BZ_LEN,UKIDStr,UKID_LEN);
	//5.3  16�ֽڣ�������

	memset((*out_buffer)+BZ_LEN+UKID_LEN,RESERVE_Char,RESERVE_LEN);
	//5.4  4�ֽڣ��������ĳ��ȣ�
    TempData.myint32=Encrypt_Len;
	memcpy((*out_buffer)+DATA_BEGIN_POS-DATASize_LEN,TempData.mychars,DATASize_LEN);	
	//5.5 ��������
	memcpy((*out_buffer)+DATA_BEGIN_POS,m_Encryptdata,Encrypt_Len);
	//������������������������ʱ������������out_buffer��������
	if(m_signeddata!=NULL)
	{
		delete[] m_signeddata;
		m_signeddata=NULL;
	}
	if(m_Encryptdata!=NULL)
	{
		delete[] m_Encryptdata;
		m_Encryptdata=NULL;
	}
	m_ET199.DisConnectDev();
	return true;
}

//���ܣ��ӽ��ܵĻ�������ȡ��ŵ���Ϣ��
//���룺unsigned char revbuffer  ���ܻ�����
//      int reclen
//�����CString reckeys[]        ��Ϣ��
//      CString recValues[]
//      int maxnum               ��Ϣ��������
//���أ�int                      ��Ϣ�Ը���
void CUsbKeyOperation::GetErroInfo(unsigned char revbuffer[],int reclen,CString &erro)
{
	
	//2+1+4+data
	if(reclen>BZ_LEN+UKID_LEN+1)
	{
		int strlen=reclen-(BZ_LEN+UKID_LEN+1);
		int pos=BZ_LEN+UKID_LEN+1;
		erro.Empty();
		memset(wchar,NULL,10000);
		MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)(revbuffer+pos), strlen, (LPWSTR)wchar,0x200 );
		erro=CString(wchar) ;   //�Ƚ� UTF-8 תΪ Unicode	
		return ;
	}
	erro=_T("���ջ�������С,ֻ��һ����֡ͷ!");
	
}


// �Դ�֡ͷ��ͨ�Ű����н��ܴ����Ƚ��ܣ�����֤ǩ�������γ�������
bool CUsbKeyOperation::DecryptVerify(unsigned char *in_buffer, int in_buffer_Len, char &RetState,unsigned char** out_buffer, int &out_buffer_Len,CString &erro)
{
	USES_CONVERSION;
	/*char * key_buffer = NULL;
	key_buffer = (char *)malloc(key_length);*/
	CString info;//��ʾ��Ϣ
	if(in_buffer_Len<=DATA_BEGIN_POS ||NULL==in_buffer)
	{
		LOG(ERROR)<<"���յ�����ͨ�Ű�="<<in_buffer;
		LOG(ERROR)<<"���յ�����ͨ�Ű�û�з��ֱ����ܵ�������,�����޷��������������";	
        GetErroInfo(in_buffer,in_buffer_Len,info);
		LOG(ERROR)<<T2A(info);	
		return false;
	}
	//���ȣ�UK�豸����
	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return false;
	}	
	/////////////////////////////////////////////////////////////
	//��һ������֡ͷ���д���
	//1.1  ��ȡUKID��	
	unsigned char UKIDStr[UKID_LEN+1];
	memset(UKIDStr,NULL,UKID_LEN+1);
	if(m_ET199.GetData(_T(UKIDNAME),TRUE,UKIDStr,UKID_LEN,info)!=UKID_LEN)
	{
		LOG(ERROR)<<T2A(info);
		m_ET199.DisConnectDev();
		//AfxMessageBox(info);
		return false;
	}
	//1.2  �Է������ukidһ���Խ���У��
	for(int i=0;i<UKID_LEN;i++)
	{
		if(UKIDStr[i]!=in_buffer[BZ_LEN+i])
		{
			LOG(ERROR)<<"UKID��һ��,"<<UKIDStr<<"������"<<in_buffer+BZ_LEN;
		}
	}
	//1.3  ��ȡ����֡�Ĺ���״̬
	RetState=in_buffer[BZ_LEN+UKID_LEN];//֡ͷ��������һ���ֽڴ��״̬
	switch(RetState)
	{
		case '0':
			//��������
			break;
		default:
			m_ET199.DisConnectDev();
			return false;
	}
	//�ڶ�������֡ͷ30�ֽ��Ժ����������н���
	//Ԥ��������ݿ϶��ȼ��ܵ�����С
	DWORD dataLen=in_buffer_Len+128;
	unsigned char *m_Decryptdata=new unsigned char[dataLen+1];
	if(m_ET199.RSA_Decrypt(in_buffer+DATA_BEGIN_POS,in_buffer_Len-DATA_BEGIN_POS,m_Decryptdata,dataLen,info)==FALSE)
	{
		info.Format(_T("���ͳɹ������Է��������ʧ��"));
		LOG(ERROR)<<T2A(info);
		m_ET199.DisConnectDev();
		return false;
	}
	//����������ȡǩ������ԭʼ����
	out_buffer_Len=dataLen-SignedData_LEN;
	*out_buffer= (unsigned char *)malloc(out_buffer_Len+1);
	memset(*out_buffer,NULL,out_buffer_Len+1);
	memcpy((*out_buffer),m_Decryptdata+SignedData_LEN,out_buffer_Len);
	//����������ԭʼ���ݽ���ɢ�д���
	DWORD MsgSHA1_Len=SHA1_LEN+1;
	unsigned char m_MsgSHA1[SHA1_LEN+1];
	if(m_ET199.RSA_Digest(*out_buffer,out_buffer_Len,m_MsgSHA1,SHA1_LEN+1,MsgSHA1_Len,info)==FALSE)
	{
		info.Format(_T("���ͳɹ������Է��������������ɢ��ʱʧ��"));
		LOG(ERROR)<<T2A(info);
		if(m_Decryptdata!=NULL)
		{
			delete[] m_Decryptdata;
			m_Decryptdata=NULL;
		}	
		m_ET199.DisConnectDev();
		return false;
	}
	m_MsgSHA1[MsgSHA1_Len]=NULL;
	//���Ĳ�����ɢ�����ݽ���ǩ��У��
	DWORD SignedData_Len=SignedData_LEN;
	if(m_ET199.RSA_Verify_PublicKey(m_bServerPublic,m_MsgSHA1,MsgSHA1_Len,m_Decryptdata,SignedData_LEN,info)==FALSE)
	{
		info.Format(_T("���ͳɹ������Է���������ݽ���ǩ��У��ʱʧ��"));
		LOG(ERROR)<<T2A(info);
		if(m_Decryptdata!=NULL)
		{
			delete[] m_Decryptdata;
			m_Decryptdata=NULL;
		}	
		m_ET199.DisConnectDev();
		return false;
	}	 
	//���岽������������������ʱ������������out_buffer��������
	if(m_Decryptdata!=NULL)
	{
		delete[] m_Decryptdata;
		m_Decryptdata=NULL;
	}	
	m_ET199.DisConnectDev();
	return true;
}

CString CUsbKeyOperation::GetTerminalID(CString &info)
{	
	unsigned char ukid[UKID_LEN+1];
	memset(ukid,NULL,UKID_LEN+1);
	DWORD len=UKID_LEN;
	USES_CONVERSION;
	//���ȣ�UK�豸����
	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return NULL;
	}	
	if(m_ET199.GetData(_T(UKIDNAME),TRUE,ukid,len,info)!=UKID_LEN)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return NULL;
	}
	CString strUK(ukid);
	//���Ͽ��豸
	m_ET199.DisConnectDev();
	return strUK.Mid(0,TerminalID_LEN) ;
}
CString CUsbKeyOperation::GetUserID(CString &info)
{	
	unsigned char ukid[UKID_LEN+1];
	memset(ukid,NULL,UKID_LEN+1);
	DWORD len=UKID_LEN;
	USES_CONVERSION;
	//���ȣ�UK�豸����
	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return NULL;
	}	
	if(m_ET199.GetData(_T(UKIDNAME),TRUE,ukid,len,info)!=UKID_LEN)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return NULL;
	}
	CString strUK(ukid);
	//���Ͽ��豸
	m_ET199.DisConnectDev();
	return strUK.Mid(TerminalID_LEN,UserID_LEN) ;
}