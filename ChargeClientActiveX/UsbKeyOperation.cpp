#include "StdAfx.h"
#include "UsbKeyOperation.h"
#include <glog/logging.h>


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
bool CUsbKeyOperation::SignedEncrypt(unsigned char *in_buffer, int in_buffer_Len, unsigned char** out_buffer, int &out_buffer_len)
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
	unsigned char *m_signeddata=new unsigned char[SIGNATURE_LEN+in_buffer_Len+1];	
	memset(m_signeddata,NULL,SIGNATURE_LEN+in_buffer_Len+1);
	//3.1 ����ǩ��
	DWORD SignedData_Len=SIGNATURE_LEN+1;
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
	LOG(INFO)<<"ǩ��(16):"<<T2A(nByteTo16Str(SIGNATURE_LEN+in_buffer_Len,m_signeddata));
	//���Ĳ�����ǩ������Ϣ�����м���
	DWORD Encrypt_Len=SIGNATURE_LEN+in_buffer_Len+SIGNATURE_LEN;//Ԥ����ܳ��ȱ�ԭ�ĳ��Ȳ��ᳬ��128��
    unsigned char *m_Encryptdata=new unsigned char[Encrypt_Len+1];	
	memset(m_Encryptdata,NULL,Encrypt_Len+1);
	

	if(m_ET199.RSA_Encrypt(m_bServerPublic,m_signeddata,SIGNATURE_LEN+in_buffer_Len,m_Encryptdata,Encrypt_Len,info)==FALSE)
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
	out_buffer_len=VERIFY_DATA_PKT_HDR_LEN+Encrypt_Len;
	*out_buffer= (unsigned char *)malloc(out_buffer_len);
	memset(*out_buffer,NULL,out_buffer_len);
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
	memcpy((*out_buffer)+VERIFY_DATA_PKT_HDR_LEN-DATASize_LEN,TempData.mychars,DATASize_LEN);	
	//5.5 ��������
	memcpy((*out_buffer)+VERIFY_DATA_PKT_HDR_LEN,m_Encryptdata,Encrypt_Len);
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
int CUsbKeyOperation::DecryptVerify(unsigned char *in_buffer, int in_buffer_Len, 
									 char &RetState,unsigned char** out_buffer, 
									 int &out_buffer_len,CString &erro)
{
	USES_CONVERSION;
	int ret = 0;
	/*char * key_buffer = NULL;
	key_buffer = (char *)malloc(key_length);*/
	CString info;//��ʾ��Ϣ

	int pkt_hdr_len_without_playload_len =0; 
	pkt_hdr_len_without_playload_len = VERIFY_DATA_PKT_HDR_LEN - PAYLOAD_LEN;
	if(pkt_hdr_len_without_playload_len >= in_buffer_Len||NULL==in_buffer)
	{
		LOG(ERROR)<<"Recv Buffer:"<<in_buffer;
		//LOG(ERROR)<<"���յ�����ͨ�Ű�û�з��ֱ����ܵ�������,�����޷��������������";	
		LOG(ERROR)<<"We failed to decrypt data because there are not the encrypted data area in recv buffer. ";	
		erro = _T("���յ�����ͨ�Ű�û�з��ֱ����ܵ�������,�����޷��������������");
		return -1;
	}


	//���ȣ�UK�豸����
	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return -1;
	}	
	/////////////////////////////////////////////////////////////
	//��һ������֡ͷ���д���
	//1.1  ��ȡUKID��	
	//unsigned char UKIDStr[UKID_LEN+1];
	//memset(UKIDStr,NULL,UKID_LEN+1);
	//if(m_et199.getdata(_t(ukidname),true,ukidstr,ukid_len,info)!=ukid_len)
	//{
	//	log(error)<<t2a(info);
	//	m_et199.disconnectdev();
	//	afxmessagebox(info);
	//	return false;
	//}
	//1.2  �Է������ukidһ���Խ���У��
	//for(int i=0;i<ukid_len;i++)
	//{
	//	if(ukidstr[i]!=in_buffer[bz_len+i])
	//	{
	//		log(error)<<"ukid��һ��,"<<ukidstr<<"������"<<in_buffer+bz_len;
	//	}
	//}
	//1.3  ��ȡ����֡�Ĺ���״̬
	//retstate=in_buffer[bz_len+ukid_len];//֡ͷ��������һ���ֽڴ��״̬
	//switch(retstate)
	//{
	//	case '0':
	//		��������
	//		break;
	//	default:
	//		m_et199.disconnectdev();
	//		return false;
	//}
	//�ڶ�������֡ͷ30�ֽ��Ժ����������н���
	//Ԥ��������ݿ϶��ȼ��ܵ�����С
	int assume_plain_data_len = in_buffer_Len+DECRYPT_BUFFER_PADDING;
	unsigned char *plain_data= NULL;
	plain_data = (unsigned char*)malloc(assume_plain_data_len + 1);
	if(NULL==plain_data){
		LOG(ERROR)<<"malloc memory, failed.";
		ret = -1;
		goto DecryptVerify_END;
	}
	memset(plain_data, 0, assume_plain_data_len + 1);
	DWORD plain_data_len;

	if(FALSE == m_ET199.RSA_Decrypt(in_buffer+VERIFY_DATA_PKT_HDR_LEN,
		in_buffer_Len-VERIFY_DATA_PKT_HDR_LEN,
		plain_data,plain_data_len,info))
	{
		info.Format(_T("����ʧ��"));
		LOG(ERROR)<<T2A(info);
		ret = -1;
		goto DecryptVerify_END;
	}
	//��ȡԭʼTEXT����
	out_buffer_len=plain_data_len-SIGNATURE_LEN;
	*out_buffer= (unsigned char *)malloc(out_buffer_len+1);
	if(NULL==*out_buffer){
		ret = -1;
		goto DecryptVerify_END;
	}
	memset(*out_buffer,NULL,out_buffer_len+1);
	memcpy((*out_buffer),plain_data+SIGNATURE_LEN,out_buffer_len);


	//����������ԭʼ���ݽ���ɢ�д���
	DWORD MsgSHA1_Len=SHA1_LEN;
	unsigned char MsgSHA1[SHA1_LEN+1];
	memset(MsgSHA1, 0, SHA1_LEN+1);
	if(FALSE == m_ET199.RSA_Digest(*out_buffer,out_buffer_len,MsgSHA1,SHA1_LEN+1,MsgSHA1_Len,info))
	{
		info.Format(_T("SHA1ɢ��ʱʧ��."));
		LOG(ERROR)<<T2A(info);
		ret = -1;
		goto DecryptVerify_END;
	}

	LOG(INFO)<<MsgSHA1;

	//���Ĳ�����ɢ�����ݽ���ǩ��У��
	//malloc the signature buffer and copy the signature from the recv pkt.
	//unsigned char *pkt_signature = NULL;
	//pkt_signature = (unsigned char*)malloc(SIGNATURE_LEN + 1);
	//if(FALSE==pkt_signature){
	//	ret = -1;
	//	goto DecryptVerify_END;
	//}else{
	//	memset(pkt_signature,0, SIGNATURE_LEN +1);
	//	memcpy(pkt_signature, plain_data, SIGNATURE_LEN);
	//}
	DWORD sig_len = SIGNATURE_LEN;
	unsigned char pkt_signature[SIGNATURE_LEN+1];
	memset(pkt_signature, 0, SIGNATURE_LEN+1);
	memcpy(pkt_signature, plain_data, SIGNATURE_LEN);
	if(FALSE==m_ET199.RSA_Verify_PublicKey(TRUE,MsgSHA1,MsgSHA1_Len,pkt_signature,sig_len,info))
	{
		info.Format(_T("�������ݽ���ǩ��У��ʱʧ��."));
		LOG(ERROR)<<T2A(info);

		ret = 1;
		goto DecryptVerify_END;
	}	 
	else{
		ret = 1;
	}
	//���岽���ͷ���ʱ������������out_buffer������
DecryptVerify_END:
	if(plain_data!=NULL)
	{
		free(plain_data);
		plain_data=NULL;
	}	
	//if(NULL!=pkt_signature){
	//	free(pkt_signature);
	//	pkt_signature = NULL;
	//}
	m_ET199.DisConnectDev();
	return ret;
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