#include "StdAfx.h"
#include "UsbKeyOperation.h"

#undef ERROR
#include <glog/logging.h>

#define UKIDNAME "UKID"
#define Padding_Len 1

#define SHA1_LEN 20
#define SignedData_LEN 128

#define UK_PIN "1uTZxyaR1TAP"
//通信帧
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
	//引导ET199的库文件，进行环境初始化
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
	unsigned char *pSignedBuffer=NULL;//需要清理缓冲区
	int Signedbuffer_len=0;

	//加密
	//强制使用本地公钥
	m_bServerPublic=FALSE;
	if(SignedEncrypt(pstr,test_plain_txt_len,(unsigned char**)&pSignedBuffer,Signedbuffer_len)==false)
	{
		info= info + _T("USB KEY 自检, 签名, 加密 失败.");
		LOG(ERROR)<<T2A(info);
		if(NULL != pSignedBuffer)
		{
			free(pSignedBuffer);
			pSignedBuffer=NULL;
		}
		m_bServerPublic=TRUE;//恢复使用远程服务器公钥
		return false;
	}
	
	//here for general use to handle with verify packet header, maybe change it later.
	pSignedBuffer[BZ_LEN+UKID_LEN]='0';
	
	//对密文解密
	unsigned char *pBuffer=NULL;//需要清理缓冲区
	int buffer_len=0;
	char bz;
	if(DecryptVerify(pSignedBuffer,Signedbuffer_len,bz,(unsigned char**)&pBuffer,buffer_len,info)==false)
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
		m_bServerPublic=TRUE;//恢复使用远程服务器公钥
		return false;
	}
	m_bServerPublic=TRUE;//恢复使用远程服务器公钥

	
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

// 对数据包进行加密处理（先散列，再签名，最后加密）后，形成带帧头的通信包
bool CUsbKeyOperation::SignedEncrypt(unsigned char *in_buffer, int in_buffer_Len, unsigned char** out_buffer, int &out_buffer_Len)
{

	USES_CONVERSION;
	/*char * key_buffer = NULL;
	key_buffer = (char *)malloc(key_length);*/
	CString info;//提示信息
	if(in_buffer_Len<1 || in_buffer==NULL)
	{
		LOG(ERROR)<<"原始数据长度为0，无法加密";		
		return false;
	}
	//首先：UK设备联结
	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return false;
	}	
	/////////////////////////////////////////////////////////////
	//第一步：获取UKID码	
	unsigned char UKIDStr[UKID_LEN+1];
	memset(UKIDStr,NULL,UKID_LEN+1);
	if(m_ET199.GetData(_T(UKIDNAME),TRUE,UKIDStr,UKID_LEN,info)!=UKID_LEN)
	{
		LOG(ERROR)<<T2A(info);
		m_ET199.DisConnectDev();
		//AfxMessageBox(info);
		return false;
	}

	//第二步：对数据进行散列，散列后数据长度=SHA1_LEN
	unsigned char m_MsgSHA1[SHA1_LEN+1];
	memset(m_MsgSHA1,NULL,SHA1_LEN+1);
	DWORD MsgSHA1_Len=SHA1_LEN;
	if(m_ET199.RSA_Digest(in_buffer,in_buffer_Len,m_MsgSHA1,SHA1_LEN+1,MsgSHA1_Len,info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);	
		m_ET199.DisConnectDev();
		return false;
	}
	LOG(INFO)<<"散列:"<<T2A(CString(m_MsgSHA1));
	LOG(INFO)<<"散列处理(16):"<<T2A(nByteTo16Str(MsgSHA1_Len,m_MsgSHA1));
	//第三步：对散列数据进行签名
	unsigned char *m_signeddata=new unsigned char[SignedData_LEN+in_buffer_Len+1];	
	memset(m_signeddata,NULL,SignedData_LEN+in_buffer_Len+1);
	//3.1 制造签名
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
	//3.2 签名+原信息 = 组合成签名后信息包
	memcpy(m_signeddata+SignedData_Len,in_buffer,in_buffer_Len);	
	//LOG(INFO)<<"签名:"<<T2A(CString(m_signeddata));
	LOG(INFO)<<"签名(16):"<<T2A(nByteTo16Str(SignedData_LEN+in_buffer_Len,m_signeddata));
	//第四步：对签名后信息包进行加密
	DWORD Encrypt_Len=SignedData_LEN+in_buffer_Len+SignedData_LEN;//预测加密长度比原文长度不会超过128；
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
	//LOG(INFO)<<"加密:"<<T2A(CString(m_Encryptdata));
	LOG(INFO)<<"加密(16):"<<T2A(nByteTo16Str(Encrypt_Len,m_Encryptdata));
	//第五步：组合一个34字节帧头
	//通信帧    = 帧头+数据区
	//帧头      = 2字节（标志：申请公钥=“00”;验证UKID="01"）+12字节（UKID）+16字节（保留）+4字节（数据区的长度）
	out_buffer_Len=DATA_BEGIN_POS+Encrypt_Len;
	*out_buffer= (unsigned char *)malloc(out_buffer_Len);
	memset(*out_buffer,NULL,out_buffer_Len);
	//5.1  "10"是加密交易标志
	/*memset(*out_buffer,'1',1);
	memset(*out_buffer+1,'0',1);*/
	memcpy(*out_buffer,SignedBZ,BZ_LEN);
	/*(*out_buffer)[0]='1';
	(*out_buffer)[1]='0';*/
	//5.2  加入UKID	 
	memcpy((*out_buffer)+BZ_LEN,UKIDStr,UKID_LEN);
	//5.3  16字节（保留）

	memset((*out_buffer)+BZ_LEN+UKID_LEN,RESERVE_Char,RESERVE_LEN);
	//5.4  4字节（数据区的长度）
    TempData.myint32=Encrypt_Len;
	memcpy((*out_buffer)+DATA_BEGIN_POS-DATASize_LEN,TempData.mychars,DATASize_LEN);	
	//5.5 拷贝密文
	memcpy((*out_buffer)+DATA_BEGIN_POS,m_Encryptdata,Encrypt_Len);
	//第六步：清理本函数建立的临时缓冲区，其中out_buffer不用清理
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


// 对带帧头的通信包进行解密处理（先解密，再验证签名）后，形成数据区
bool CUsbKeyOperation::DecryptVerify(unsigned char *in_buffer, int in_buffer_Len, char &RetState,unsigned char** out_buffer, int &out_buffer_Len,CString &erro)
{
	USES_CONVERSION;
	/*char * key_buffer = NULL;
	key_buffer = (char *)malloc(key_length);*/
	CString info;//提示信息
	if(in_buffer_Len<=DATA_BEGIN_POS ||NULL==in_buffer)
	{
		LOG(ERROR)<<"接收的下行通信包="<<in_buffer;
		LOG(ERROR)<<"接收的下行通信包没有发现被加密的数据区,所以无法完成数据区解密";	
        GetErroInfo(in_buffer,in_buffer_Len,info);
		LOG(ERROR)<<T2A(info);	
		return false;
	}
	//首先：UK设备联结
	if(m_ET199.ConnectDev(_T(UK_PIN),info)==FALSE)
	{
		LOG(ERROR)<<T2A(info);
		//AfxMessageBox(info);
		return false;
	}	
	/////////////////////////////////////////////////////////////
	//第一步：对帧头进行处理
	//1.1  获取UKID码	
	unsigned char UKIDStr[UKID_LEN+1];
	memset(UKIDStr,NULL,UKID_LEN+1);
	if(m_ET199.GetData(_T(UKIDNAME),TRUE,UKIDStr,UKID_LEN,info)!=UKID_LEN)
	{
		LOG(ERROR)<<T2A(info);
		m_ET199.DisConnectDev();
		//AfxMessageBox(info);
		return false;
	}
	//1.2  对返回码的ukid一致性进行校验
	for(int i=0;i<UKID_LEN;i++)
	{
		if(UKIDStr[i]!=in_buffer[BZ_LEN+i])
		{
			LOG(ERROR)<<"UKID不一致,"<<UKIDStr<<"不等于"<<in_buffer+BZ_LEN;
		}
	}
	//1.3  获取返回帧的工作状态
	RetState=in_buffer[BZ_LEN+UKID_LEN];//帧头保留区第一个字节存放状态
	switch(RetState)
	{
		case '0':
			//工作正常
			break;
		default:
			m_ET199.DisConnectDev();
			return false;
	}
	//第二步：对帧头30字节以后数据区进行解密
	//预设解密数据肯定比加密的数据小
	DWORD dataLen=in_buffer_Len+128;
	unsigned char *m_Decryptdata=new unsigned char[dataLen+1];
	if(m_ET199.RSA_Decrypt(in_buffer+DATA_BEGIN_POS,in_buffer_Len-DATA_BEGIN_POS,m_Decryptdata,dataLen,info)==FALSE)
	{
		info.Format(_T("发送成功，但对返回码解密失败"));
		LOG(ERROR)<<T2A(info);
		m_ET199.DisConnectDev();
		return false;
	}
	//第三步：获取签名码后的原始数据
	out_buffer_Len=dataLen-SignedData_LEN;
	*out_buffer= (unsigned char *)malloc(out_buffer_Len+1);
	memset(*out_buffer,NULL,out_buffer_Len+1);
	memcpy((*out_buffer),m_Decryptdata+SignedData_LEN,out_buffer_Len);
	//第三步：对原始数据进行散列处理
	DWORD MsgSHA1_Len=SHA1_LEN+1;
	unsigned char m_MsgSHA1[SHA1_LEN+1];
	if(m_ET199.RSA_Digest(*out_buffer,out_buffer_Len,m_MsgSHA1,SHA1_LEN+1,MsgSHA1_Len,info)==FALSE)
	{
		info.Format(_T("发送成功，但对返回码的数据制造散列时失败"));
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
	//第四步：对散列数据进行签名校验
	DWORD SignedData_Len=SignedData_LEN;
	if(m_ET199.RSA_Verify_PublicKey(m_bServerPublic,m_MsgSHA1,MsgSHA1_Len,m_Decryptdata,SignedData_LEN,info)==FALSE)
	{
		info.Format(_T("发送成功，但对返回码的数据进行签名校验时失败"));
		LOG(ERROR)<<T2A(info);
		if(m_Decryptdata!=NULL)
		{
			delete[] m_Decryptdata;
			m_Decryptdata=NULL;
		}	
		m_ET199.DisConnectDev();
		return false;
	}	 
	//第五步：清理本函数建立的临时缓冲区，其中out_buffer不用清理
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