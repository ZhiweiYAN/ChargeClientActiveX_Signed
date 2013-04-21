#include "StdAfx.h"
#include "UsbKeyOperation.h"
#include <glog/logging.h>


CUsbKeyOperation::CUsbKeyOperation(void)
{
}

CUsbKeyOperation::~CUsbKeyOperation(void)
{
}


int CUsbKeyOperation::InitInstance(void)
{
	//引导ET199的库文件，进行环境初始化
	CK_RV rv;
	// Load PKCS#11 library
	rv = C_Initialize(NULL);
	if(CKR_OK != rv)
	{
		LOG(ERROR)<<"Can't Load PKCS#11 ET199 USBKEY Library!";
		return -1;
	}
	else
	{
		LOG(INFO)<<"Load PKCS#11 ET199 USBKEY Library OK.";
		return 1;
	}

}
void CUsbKeyOperation::ExitInstance(void)
{
	CK_RV rv;
	rv = C_Finalize(NULL);
	if(CKR_OK != rv)
	{
		LOG(ERROR)<<"Can't Unload PKCS#11 ET199 Library!";
	}
	else
	{
		LOG(INFO)<<"Unload PKCS#11 USBKEY Library OK.";
	}
	return;
}
bool CUsbKeyOperation::SelfCheck(CString &info)
{
	USES_CONVERSION;

	unsigned char test_plain_text[]="123456789ABCDEF";
	int test_plain_txt_len= 15;
	unsigned char *pSignedBuffer=NULL;//需要清理缓冲区
	int Signedbuffer_len=0;

	//加密
	unsigned char* pstr = test_plain_text;

	if(SignedEncryptPkt(1, pstr,test_plain_txt_len,(unsigned char**)&pSignedBuffer,Signedbuffer_len)==false)
	{
		info= info + _T("USB KEY 自检, 签名, 加密 失败.");
		LOG(ERROR)<<T2A(info);
		if(NULL != pSignedBuffer)
		{
			free(pSignedBuffer);
			pSignedBuffer=NULL;
		}
		return false;
	}
	

	////here for general use to handle with verify packet header, maybe change it later.
	//pSignedBuffer[BZ_LEN+UKID_LEN]='0';
	//
	////对密文解密
	unsigned char *pBuffer=NULL;//需要清理缓冲区
	int buffer_len=0;
	char bz;
	if(DecryptVerifyPkt(1, pSignedBuffer,Signedbuffer_len,bz,(unsigned char**)&pBuffer,buffer_len,info)==false)
	{		
		info= info + _T("USB KEY 自检, 解密, 验签 失败.");
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
		return false;
	}

	
	//检查一致性
	for(int i=0;i<buffer_len;i++)
	{
		if(pBuffer[i]!=pstr[i])
		{
			info=info+_T("原始明文与解密后明文两者内容不一致");
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
	//首先：UK设备联结
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
	//最后断开设备
	m_ET199.DisConnectDev();
	return true;
}

//// 对数据包进行加密处理（先散列，再签名，最后加密）后，形成带帧头的通信包
//bool cusbkeyoperation::signedencrypt(unsigned char *in_buffer, int in_buffer_len, unsigned char** out_buffer, int &out_buffer_len)
//{
//
//	uses_conversion;
//	/*char * key_buffer = null;
//	key_buffer = (char *)malloc(key_length);*/
//	cstring info;//提示信息
//	if(in_buffer_len<1 || in_buffer==null)
//	{
//		log(error)<<"原始数据长度为0，无法加密";		
//		return false;
//	}
//	//首先：uk设备联结
//	if(m_et199.connectdev(_t(uk_pin),info)==false)
//	{
//		log(error)<<t2a(info);
//		//afxmessagebox(info);
//		return false;
//	}	
//	/////////////////////////////////////////////////////////////
//	//第一步：获取ukid码	
//	unsigned char ukidstr[ukid_len+1];
//	memset(ukidstr,null,ukid_len+1);
//	if(m_et199.getdata(_t(ukidname),true,ukidstr,ukid_len,info)!=ukid_len)
//	{
//		log(error)<<t2a(info);
//		m_et199.disconnectdev();
//		//afxmessagebox(info);
//		return false;
//	}
//
//	//第二步：对数据进行散列，散列后数据长度=sha1_len
//	unsigned char msgsha1[sha1_len+1];
//	memset(msgsha1,null,sha1_len+1);
//	dword msgsha1_len=sha1_len;
//	if(m_et199.rsa_digest(in_buffer,in_buffer_len,msgsha1,sha1_len+1,msgsha1_len,info)==false)
//	{
//		log(error)<<t2a(info);	
//		m_et199.disconnectdev();
//		return false;
//	}
//	//log(info)<<"散列:"<<t2a(cstring(m_msgsha1));
//	//log(info)<<"散列处理(16):"<<t2a(nbyteto16str(msgsha1_len,m_msgsha1));
//
//	//第三步：对散列数据进行签名
//	unsigned char *m_signeddata=null;
//	m_signeddata = new unsigned char[signature_len+in_buffer_len+1];	
//	memset(m_signeddata,null,signature_len+in_buffer_len+1);
//	//3.1 制造签名
//	dword signeddata_len=signature_len+1;
//	if(m_et199.rsa_signed(msgsha1,msgsha1_len,m_signeddata,signeddata_len,info)==false)
//	{
//		if(m_signeddata!=null)
//		{
//			delete[] m_signeddata;
//			m_signeddata=null;
//		}
//		log(error)<<t2a(info);
//		m_et199.disconnectdev();
//		return false;
//	}
//	//3.2 签名+原信息 = 组合成签名后信息包
//	memcpy(m_signeddata+signeddata_len,in_buffer,in_buffer_len);	
//	//log(info)<<"签名:"<<t2a(cstring(m_signeddata));
//	log(info)<<"签名(16):"<<t2a(nbyteto16str(signature_len+in_buffer_len,m_signeddata));
//	//第四步：对签名后信息包进行加密
//	dword encrypt_len=signature_len+in_buffer_len+signature_len;//预测加密长度比原文长度不会超过128；
//    unsigned char *m_encryptdata=new unsigned char[encrypt_len+1];	
//	memset(m_encryptdata,null,encrypt_len+1);
//	
//
//	if(m_et199.rsa_encrypt(m_bserverpublic,m_signeddata,signature_len+in_buffer_len,m_encryptdata,encrypt_len,info)==false)
//	{
//		log(error)<<t2a(info);
//		if(m_signeddata!=null)
//		{
//			delete[] m_signeddata;
//			m_signeddata=null;
//		}
//		if(m_encryptdata!=null)
//		{
//			delete[] m_encryptdata;
//			m_encryptdata=null;
//		}
//		m_et199.disconnectdev();
//		return false;
//	}
//	m_encryptdata[encrypt_len]=null;
//	//log(info)<<"加密:"<<t2a(cstring(m_encryptdata));
//	log(info)<<"加密(16):"<<t2a(nbyteto16str(encrypt_len,m_encryptdata));
//	//第五步：组合一个34字节帧头
//	//通信帧    = 帧头+数据区
//	//帧头      = 2字节（标志：申请公钥=“00”;验证ukid="01"）+12字节（ukid）+16字节（保留）+4字节（数据区的长度）
//	out_buffer_len=verify_data_pkt_hdr_len+encrypt_len;
//	*out_buffer= (unsigned char *)malloc(out_buffer_len);
//	memset(*out_buffer,null,out_buffer_len);
//	//5.1  "10"是加密交易标志
//	/*memset(*out_buffer,'1',1);
//	memset(*out_buffer+1,'0',1);*/
//	memcpy(*out_buffer,signedbz,bz_len);
//	/*(*out_buffer)[0]='1';
//	(*out_buffer)[1]='0';*/
//	//5.2  加入ukid	 
//	memcpy((*out_buffer)+bz_len,ukidstr,ukid_len);
//	//5.3  16字节（保留）
//
//	memset((*out_buffer)+bz_len+ukid_len,reserve_char,reserve_len);
//	//5.4  4字节（数据区的长度）
//    tempdata.myint32=encrypt_len;
//	memcpy((*out_buffer)+verify_data_pkt_hdr_len-datasize_len,tempdata.mychars,datasize_len);	
//	//5.5 拷贝密文
//	memcpy((*out_buffer)+verify_data_pkt_hdr_len,m_encryptdata,encrypt_len);
//	//第六步：清理本函数建立的临时缓冲区，其中out_buffer不用清理
//	if(m_signeddata!=null)
//	{
//		delete[] m_signeddata;
//		m_signeddata=null;
//	}
//	if(m_encryptdata!=null)
//	{
//		delete[] m_encryptdata;
//		m_encryptdata=null;
//	}
//	m_et199.disconnectdev();
//	return true;
//}


bool CUsbKeyOperation::SignedEncryptPkt(int test_flag,
										unsigned char *in_buffer, 
										int in_buffer_len, 
										unsigned char** out_buffer, 
										int &out_buffer_len)
{

	USES_CONVERSION;
	BOOL ret = FALSE;

	CString info;//提示信息
	if(in_buffer_len<1 || in_buffer==NULL)
	{
		LOG(ERROR)<<"原始数据长度为0，无法加密";		
		return false;
	}

	//UK设备联结
	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		return false;
	}	

	//获取UKID码	
	unsigned char ukey_id_str[UKID_LEN+1];
	memset(ukey_id_str,NULL,UKID_LEN+1);
	if(m_ET199.GetData(_T(UKIDNAME),TRUE,ukey_id_str,UKID_LEN,info)!=UKID_LEN)
	{
		LOG(ERROR)<<T2A(info);
		m_ET199.DisConnectDev();
		return false;
	}

	//对明文数据进行SHA1散列，散列后数据长度应该为 （20）SHA1_LEN
	unsigned char msg_sha1[SHA1_LEN+1];
	memset(msg_sha1,NULL,SHA1_LEN+1);

	unsigned long int msg_sha1_len = SHA1_LEN;
	unsigned long int max_msg_sha1_len = 2*SHA1_LEN;
	
	if(m_ET199.RSA_Digest(in_buffer,
		in_buffer_len,
		msg_sha1, 
		msg_sha1_len, info)==FALSE){
		LOG(ERROR)<<T2A(info);	
		m_ET199.DisConnectDev();
		return false;
	}

	//对sha1的20个字节进行签名，会形成一个128字节(1024bit)的数据区。
	unsigned char *signature = NULL;
	signature = (unsigned char*) malloc(SIGNATURE_LEN+1);
	CHECK(NULL!=signature)<<"malloc memory, failed.";
	memset(signature, 0, SIGNATURE_LEN+1);

	unsigned long int signature_len = SIGNATURE_LEN;

	ret = m_ET199.RSA_Signed(msg_sha1, msg_sha1_len,signature, &signature_len, info);
	if(FALSE == ret){
		LOG(ERROR)<<"Adding signature, failed.";
		goto SignedEncryptPkt_END;
	}

	int uncrypted_data_len = 0;
	uncrypted_data_len = signature_len + in_buffer_len;

	unsigned char* uncrypted_data = NULL;
	uncrypted_data = (unsigned char*)malloc(uncrypted_data_len);
	CHECK(NULL!=uncrypted_data)<<"malloc memory, failed";
	memset(uncrypted_data, 0, uncrypted_data_len);

	//combine the signature txt and inbuffer text.
	memcpy(uncrypted_data, signature, signature_len);
	memcpy(uncrypted_data+signature_len, in_buffer, in_buffer_len);

	//encrypt the data
	unsigned char* cipher_buf = NULL;
	int cipher_buf_len = 0;
	cipher_buf_len = 2*(signature_len+in_buffer_len);
	cipher_buf = (unsigned char*)malloc(cipher_buf_len);
	CHECK(NULL!=cipher_buf);
	memset(cipher_buf, 0, cipher_buf_len);
	unsigned long int  cipher_data_len = 0;

	ret = m_ET199.RSA_Encrypt(test_flag, 
		uncrypted_data, (unsigned long int )uncrypted_data_len, 
		cipher_buf, &cipher_data_len, info);
	if(FALSE==ret){
		LOG(ERROR)<<"Rsa encrypt, failed.";
		goto SignedEncryptPkt_END;
	}

	//通信帧    = 帧头+数据区
	//帧头      = 2字节（标志：申请公钥=“00”;验证UKID="01"）+12字节（UKID）+16字节（保留）+4字节（数据区的长度）

	out_buffer_len = VERIFY_DATA_PKT_HDR_LEN + cipher_data_len;
	*out_buffer= (unsigned char *)malloc(out_buffer_len);
	CHECK(NULL!=out_buffer)<<"malloc memory, failed.";

	memset(*out_buffer, ' ', out_buffer_len);

	//trans pkt
	memcpy(*out_buffer,"10",MSG_TYPE_LEN);
	//ukey id
	memcpy((*out_buffer)+MSG_TYPE_LEN, ukey_id_str, UKID_LEN);
	//pkt_len binary
	char cipher_data_len_str[2*PAYLOAD_LEN+1];
	memset(cipher_data_len_str, 0, 2*PAYLOAD_LEN+1);

	sprintf_s(cipher_data_len_str, 2*PAYLOAD_LEN, "%06d", cipher_data_len);
	memcpy((*out_buffer)+VERIFY_DATA_PKT_HDR_LEN-PAYLOAD_LEN, cipher_data_len_str, PAYLOAD_LEN);
	//cipher data copy
	memcpy((*out_buffer)+VERIFY_DATA_PKT_HDR_LEN,cipher_buf,cipher_data_len);


SignedEncryptPkt_END:
	m_ET199.DisConnectDev();

if(NULL!=signature){
	free(signature);
	signature = NULL;
}
if(NULL!=uncrypted_data){
    free(uncrypted_data);
	uncrypted_data = NULL;
	}
if(NULL!=cipher_buf){
	free(cipher_buf);
	cipher_buf = NULL;
}

	return true;
}
//功能：从接受的缓冲区获取存放的信息对
//输入：unsigned char revbuffer  接受缓冲区
//      int reclen
//输出：CString reckeys[]        信息对
//      CString recValues[]
//      int maxnum               信息对最大个数
//返回：int                      信息对个数
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
		erro=CString(wchar) ;   //先将 UTF-8 转为 Unicode	
		return ;
	}
	erro=_T("接收缓冲区过小,只有一部分帧头!");
	
}

int CUsbKeyOperation::DecryptVerifyPkt(int test_flag, unsigned char *in_buffer, int in_buffer_Len, 
									char &RetState,unsigned char** out_buffer, 
									int &out_buffer_len,CString &erro)
{
	USES_CONVERSION;

	int ret = 0;
	BOOL bRet = FALSE;
	CString info;

	//Check input parameters
	if(NULL==in_buffer){
		LOG(ERROR)<<"Input buffer is empty.";
		return -1;
	}

	if(VERIFY_DATA_PKT_HDR_LEN>=in_buffer_Len){
		LOG(ERROR)<<"Received Packet is not complete.";
		return -1;
	}

	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<"Connect USBKEY, failed, and "<<T2A(info);
		return -1;
	}	

	//decrypt the cipher txt.
	CK_BYTE_PTR cipher_txt = NULL;
	cipher_txt = in_buffer+VERIFY_DATA_PKT_HDR_LEN;

	CK_ULONG cipher_txt_len = 0;
	cipher_txt_len = in_buffer_Len - VERIFY_DATA_PKT_HDR_LEN;

	CK_ULONG predicted_plain_data_len = in_buffer_Len+DECRYPT_BUFFER_PADDING;
	CK_BYTE_PTR plain_data= NULL;
	plain_data = (CK_BYTE_PTR)malloc(predicted_plain_data_len);
	if(NULL==plain_data){
		LOG(ERROR)<<"malloc memory, failed.";
		ret = -1;
		goto DecryptVerifyPkt_END;
	}
	memset(plain_data, 0, predicted_plain_data_len);

	CK_ULONG real_plain_data_len = 0;

	bRet = m_ET199.RSA_Decrypt(cipher_txt, cipher_txt_len, plain_data, &real_plain_data_len, info);
	if(FALSE==bRet || 0==real_plain_data_len) {
		info.Format(_T("解密失败"));
		LOG(ERROR)<<"Decrypt data, failed." << T2A(info);
		ret = -1;
		goto DecryptVerifyPkt_END;
	}

	//SHA1 for plain data
	CK_ULONG msg_sha1_len = 0;
	msg_sha1_len = SHA1_LEN;

	CK_BYTE msg_sha1[SHA1_LEN+1];
	memset(msg_sha1, 0, SHA1_LEN+1);

	CK_BYTE_PTR msg = NULL;
	CK_ULONG msg_len = 0;

	msg_len = real_plain_data_len - SIGNATURE_LEN;
	msg = plain_data + SIGNATURE_LEN;

	bRet = m_ET199.RSA_Digest(msg, msg_len, msg_sha1,  msg_sha1_len, info);
	if(FALSE==bRet){
		info.Format(_T("SHA1失败."));
		LOG(ERROR)<<"SHA1, failed" <<T2A(info);
		ret = -1;
		goto DecryptVerifyPkt_END;

	}

	//verify signature
	CK_ULONG signature_in_pkt_len = 0;
	CK_BYTE  signature_in_pkt[SIGNATURE_LEN+1];

	signature_in_pkt_len = SIGNATURE_LEN;
	memset(signature_in_pkt, 0, SIGNATURE_LEN+1);
	memcpy(signature_in_pkt, plain_data, SIGNATURE_LEN);

	bRet = m_ET199.RSA_Verify(test_flag, msg_sha1, msg_sha1_len, signature_in_pkt, signature_in_pkt_len, info );
	if(FALSE==bRet){
		info.Format(_T("签名校验时失败."));
		LOG(ERROR)<<"Verify Failed"<<T2A(info);
		ret = -1;
		goto DecryptVerifyPkt_END;
	}
	else{
		ret = 1;
	}

	//return the original text.
	out_buffer_len=real_plain_data_len-SIGNATURE_LEN;
	*out_buffer= (unsigned char *)malloc(out_buffer_len+1);
	if(NULL==*out_buffer){
		ret = -1;
		goto DecryptVerifyPkt_END;
	}
	memset(*out_buffer,NULL,out_buffer_len+1);
	memcpy((*out_buffer),plain_data+SIGNATURE_LEN,out_buffer_len);

DecryptVerifyPkt_END:
	if(plain_data!=NULL)
	{
		free(plain_data);
		plain_data=NULL;
	}	

	m_ET199.DisConnectDev();
	return ret;
}



CString CUsbKeyOperation::GetTerminalID(CString &info)
{	
	unsigned char ukid[UKID_LEN+1];
	memset(ukid,NULL,UKID_LEN+1);
	DWORD len=UKID_LEN;
	USES_CONVERSION;
	//首先：UK设备联结
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
	//最后断开设备
	m_ET199.DisConnectDev();
	return strUK.Mid(0,TerminalID_LEN) ;
}
CString CUsbKeyOperation::GetUserID(CString &info)
{	
	unsigned char ukid[UKID_LEN+1];
	memset(ukid,NULL,UKID_LEN+1);
	DWORD len=UKID_LEN;
	USES_CONVERSION;
	//首先：UK设备联结
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
	//最后断开设备
	m_ET199.DisConnectDev();
	return strUK.Mid(TerminalID_LEN,UserID_LEN) ;
}