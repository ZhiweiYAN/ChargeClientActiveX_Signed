// UK_Et199Class.cpp : 实现文件
//

#include "stdafx.h"
//#include "ChangeAdminActiveX.h"


#include "UK_Et199Class.h"
#undef ERROR
#include <glog/logging.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define	countof(a)			(sizeof(a)/ sizeof(CK_ATTRIBUTE))

CK_BBOOL bTrue = TRUE;
CK_BBOOL bFalse = FALSE;

#define BUFFER_SIZE 255
#define PROGRAM_NAME "PKCS ET199 App"



CUK_Et199Class::CUK_Et199Class()
{
	m_pSlotList = NULL_PTR;
	m_pApplication = new char[BUFFER_SIZE];
	ZeroMemory(m_pApplication, BUFFER_SIZE);
	memcpy(m_pApplication,PROGRAM_NAME, strlen(PROGRAM_NAME));
	m_hSession = NULL_PTR;

}

CUK_Et199Class::~CUK_Et199Class()
{
	DisConnectDev();

	if(NULL!=m_pApplication)
	{
		delete[] m_pApplication;
		m_pApplication = NULL;
	}
	
	if(m_pSlotList)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}


}
void CUK_Et199Class::DisConnectDev(void)
{
	if(NULL!=m_hSession)
	{
		C_Logout(m_hSession);
		C_CloseSession(m_hSession);
		m_hSession = NULL_PTR;
	}
	m_strUserPIN="";
}


BOOL CUK_Et199Class::FormatDev(CString strTokenName,CString strSOPin,CString strUserPin,CString &info)
{
	if(m_hSession !=NULL_PTR)
	{
		C_CloseSession(m_hSession);
		m_hSession = NULL_PTR;
	}
    AUX_INIT_TOKEN_LOWLEVL_PKI_V11 param = {0};
	param.version.major = 0x01;
	param.version.minor = 0x01;
	USES_CONVERSION;
	param.strTokenName = T2A(strTokenName);
	param.strSOPin = T2A(strSOPin);
	param.strUserPin = T2A(strUserPin);

	//param.strTokenName=strTokenName.GetBuffer(strTokenName.GetLength());
	//param.strSOPin = strSOPin.GetBuffer(strSOPin.GetLength());
	//param.strUserPin = strUserPin.GetBuffer(strUserPin.GetLength());
	param.ucSOMaxPinEC = 10;
	param.ucUserMaxPinEC = 10;
	param.nRSAKeyPairCount = 0; //not used to ET199
	param.ulPubSize = 30000;
	param.ulPrvSize = 15000;
	param.nDSAKeyPairCount = 0; //not used to ET199

	

	AUX_FUNC_LIST_PTR pAuxFunc = NULL;
	CK_RV rv = CKR_OK;
	CK_SLOT_ID slotList[10] = {0};
	CK_ULONG ulList = 10;
	
	rv = C_GetSlotList(CK_TRUE, slotList, &ulList);
	if (CKR_OK != rv)
	{
		info="Can not get slot list!";
		//MessageBox("Can not get slot list!", "ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	if (0 == ulList)
	{
		info=_T("USB token is not attached to your USB port!");
		//MessageBox("USB token is not attached to your USB port!", "ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	if (1 < ulList)
	{
		info.Format(_T("More than one USB token has been attached to your USB port! Only one can be leaved!"));
		//MessageBox("More than one USB token has been attached to your USB port! Only one can be leaved!", "ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	rv = E_GetAuxFunctionList(&pAuxFunc);
	if (CKR_OK != rv || NULL == pAuxFunc)
	{
		info.Format(_T("Can not get the format function information!"));//, "ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	CWaitCursor waitcursor;
	rv = ((EP_InitTokenPrivate)(pAuxFunc->pFunc[EP_INIT_TOKEN_PRIVATE]))(slotList[0], &param);
	if (CKR_OK == rv)
	{
		info = "Initialize succeeded.";
		return TRUE;
	}
	else
	{
		info = "Initialize failed!";
		return FALSE;
	}

}

BOOL CUK_Et199Class::ConnectDev(CString strUserPIN,CString &info) 
{
	if(m_hSession !=NULL_PTR)return TRUE;

	/*::SetCursor(::LoadCursor(NULL, IDC_WAIT));*/

	CK_RV rv;
	CK_ULONG ulCount = 0;
	rv = C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if(CKR_OK != rv )
	{
		info.Format(_T("Can't acquire information of slot, ErrorCode: 0x%08X."), rv);

		return FALSE;
	}
	if(0 >= ulCount)
	{
		info.Format(_T("Can't connect to token, make sure one token has been inserted."));
		return FALSE;
	}

	m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
	if (! m_pSlotList) 
	{
		info.Format(_T("Not enough memory!"));
		return FALSE;
	}

	rv = C_GetSlotList(TRUE, m_pSlotList, &ulCount);
	if(CKR_OK != rv )
	{
		info.Format(_T("Can't acquire information of slot, ErrorCode: 0x%08X."), rv);
		return FALSE;
	}
	if(0 >= ulCount)
	{
		info.Format(_T("Can't connect to token, make sure one token has been inserted."));
		return FALSE;
	}

	rv = C_OpenSession(
		m_pSlotList[0],  CKF_RW_SESSION | CKF_SERIAL_SESSION,
		&m_pApplication, NULL_PTR, &m_hSession);
	if(CKR_OK != rv )
	{
		info.Format(_T("Can't Acquire information of slot, ErrorCode: 0x%08X."), rv);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
		return FALSE;
	}
	else
	{
		info.Format(_T("Connect to token Successfully !"));		
		
	}
	//1、用户登录
	CK_TOKEN_INFO tokenInfo = {0};
	rv = C_GetTokenInfo(m_pSlotList[0], &tokenInfo);
	if (CKR_OK != rv)
	{
		info.Format(_T("Can not get token information!"));
		DisConnectDev();
		return FALSE;
	}
    
	CK_ULONG ulPIN = strUserPIN.GetLength();
	BYTE pBIN[128];//=(BYTE *)strUserPIN.GetBuffer(ulPIN);
	for(int i=0;i<(int)ulPIN;i++)
	{
		pBIN[i]=(BYTE)strUserPIN.GetAt(i);
	}
	
	::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	rv = C_Login(m_hSession, CKU_USER, pBIN, ulPIN);
	
	if(CKR_OK != rv)
	{
		DisConnectDev();
		info.Format(_T("Can't use your User PIN login to token ,ErrorCode: 0x%08X."), rv);
		return FALSE;
	}
	m_strUserPIN=strUserPIN;
	return TRUE;
}

BOOL CUK_Et199Class::MakeKeypairgen(CString &info) 
{
	//1、用户登录
	if(m_hSession ==NULL_PTR)
	{
		info.Format(_T("have not connect dev"));
		return FALSE;
	}
	
	
	//2、生产密钥对
	CK_ULONG ulModulusBits = MODULUS_BIT_LENGTH; 
	CK_BYTE subject[] = "local RSA Key Pair";
	CK_ULONG keyType = CKK_RSA;
	CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY;
	CK_BYTE pubExponent[4] = {0x00, 0x01, 0x00, 0x01};
	CK_ATTRIBUTE pubTemplate[] = { 
		{CKA_CLASS,			&pubClass,		sizeof(pubClass)},
		{CKA_KEY_TYPE,		&keyType,		sizeof(keyType)},
		{CKA_SUBJECT,		subject,		sizeof(subject)},
		{CKA_MODULUS_BITS,	&ulModulusBits, sizeof(ulModulusBits)},
		{CKA_ENCRYPT,		&bTrue,			sizeof(bTrue)},
		{CKA_TOKEN,			&bTrue,			sizeof(bTrue)},
		{CKA_WRAP,			&bTrue,			sizeof(bTrue)},		
		{CKA_PUBLIC_EXPONENT, pubExponent,	sizeof(pubExponent)}
	};	

	CK_OBJECT_CLASS priClass	= CKO_PRIVATE_KEY;
	CK_ATTRIBUTE priTemplate[] = {
		{CKA_CLASS,			&priClass,	sizeof(priClass)},
		{CKA_KEY_TYPE,		&keyType,	sizeof(keyType)},
		{CKA_SUBJECT,		subject,	sizeof(subject)},
		{CKA_DECRYPT,		&bTrue,		sizeof(bTrue)},
		{CKA_PRIVATE,		&bTrue,		sizeof(bTrue)},
		{CKA_SENSITIVE,		&bTrue,		sizeof(bTrue)},
		{CKA_TOKEN,			&bTrue,		sizeof(bTrue)},
		{CKA_EXTRACTABLE,	&bTrue,		sizeof(bTrue)},
		{CKA_UNWRAP,		&bTrue,		sizeof(bTrue)},
	};

	CK_MECHANISM keyGenMechanism = {
		CKM_RSA_PKCS_KEY_PAIR_GEN, 
			NULL_PTR, 
			0
	};	
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	CK_RV rv;
	CK_OBJECT_HANDLE m_hPubKey;
	CK_OBJECT_HANDLE m_hPriKey;
	//BOOL m_bKeyGen;

	rv = C_GenerateKeyPair(
		m_hSession, &keyGenMechanism,
		pubTemplate, countof(pubTemplate),
		priTemplate, countof(priTemplate),
		&m_hPubKey, &m_hPriKey);
	if(CKR_OK != rv)
	{
		info.Format(_T("Can't generate RSA key pair, ErrorCode: 0x%08X."), rv);
		return FALSE;
	}
	else
	{
		
		info.Format(_T("Generate Key Pair Successfully!"));
		//ShowMsg(NEWLINE"Now,You can Sign,Verify,Encryp and Decrypt use the Key Pair!"NEWLINE);
		return TRUE;
	}
}

BOOL CUK_Et199Class::SaveRemotePublicKey(unsigned char keybuffer[],DWORD keylen,CString &info) 
{
	//1、用户登录
	if(m_hSession ==NULL_PTR)
	{
		info.Format(_T("have not connect dev"));
		return FALSE;
	}
		
	/////////////////////////////
	CK_OBJECT_HANDLE hKey;

	CK_RV rv;
	//2、生产公钥
	CK_ULONG ulModulusBits = MODULUS_BIT_LENGTH; 
	CK_BYTE subjects[] = "Romete RSA Public Key";
	CK_ULONG keyType = CKK_RSA;
	CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY;
	CK_BYTE pubExponent[4] = {0x00, 0x01, 0x00, 0x01};
	CK_ATTRIBUTE pubTemplate[] = { 
		{CKA_CLASS,			&pubClass,		sizeof(pubClass)},
		{CKA_KEY_TYPE,		&keyType,		sizeof(keyType)},
		{CKA_SUBJECT,		subjects,		sizeof(subjects)},
		{CKA_MODULUS_BITS,	&ulModulusBits, sizeof(ulModulusBits)},
		{CKA_MODULUS,(void*)keybuffer, keylen},
		{CKA_ENCRYPT,		&bTrue,			sizeof(bTrue)},
		{CKA_TOKEN,			&bTrue,			sizeof(bTrue)},
		{CKA_WRAP,			&bTrue,			sizeof(bTrue)},		
		{CKA_PUBLIC_EXPONENT, pubExponent,	sizeof(pubExponent)}
	};	
	/* Create an RSA public key object */
	rv = C_CreateObject(m_hSession, pubTemplate, 9, &hKey);
	
    /////////////////////////////

	if(CKR_OK != rv)
	{
		info.Format(_T("Can't Generate Remote RSA Public Key , ErrorCode: 0x%08X."), rv);
		return FALSE;
	}
	else
	{
		
		info.Format(_T("Generate Remote RSA Public Key Successfully!"));
		//ShowMsg(NEWLINE"Now,You can Sign,Verify,Encryp and Decrypt use the Key Pair!"NEWLINE);
		return TRUE;
	}
}

BOOL CUK_Et199Class::SaveUSBData(CString label,BOOL bPrivate,unsigned char databuffer[],DWORD len,CString &Info)
{
	//数据对象句柄 
	CK_OBJECT_HANDLE hObject = NULL; 
	CK_RV ck_rv; 
	for(int erro=0;GetDataObject(label,bPrivate,hObject,Info)==FALSE;erro++)
	{
		if (erro>2)
		{
			return FALSE;
		}
		//1、建立数据对象		
		//数据类型：普通数据 
		CK_OBJECT_CLASS objClass = CKO_DATA; 
		CK_BBOOL bTrue = TRUE; 
		CK_BBOOL bFalse = FALSE; 
		//数据对象名称 
		CK_BYTE pbLabel[256];
		CK_ULONG ulPIN = label.GetLength();
		for(int i=0;i<(int)ulPIN;i++)
		{
			pbLabel[i]=(CK_BYTE)label.GetAt(i);
		}
		//memcpy(pbLabel,label,label.GetLength());// = (CK_BYTE_PTR)label.GetBuffer(label.GetLength()); 
		//数据对象句柄 
		CK_OBJECT_HANDLE hObject = NULL; 		
		//数据对象模版 
		CK_ATTRIBUTE pDataTemplate[]={ 
			//数据类型 
			{CKA_CLASS, &objClass, sizeof(CK_OBJECT_CLASS)},  
				//存储在 ET199 硬件中，设为 TRUE 
			{CKA_TOKEN, &bTrue, sizeof(CK_BBOOL)},   
			//是否存储在私有区里， 
			//为 TRUE，在私有区创建，需要验证 User PIN 才能读写 
			//为 FALSE，在公有区创建，不需要验证 User PIN 就可以读写 
			{CKA_PRIVATE, &bPrivate, sizeof(CK_BBOOL)},       
			//数据的标识 
			{CKA_LABEL, pbLabel,label.GetLength()},// sizeof(pbLabel)},         
			//数据内容，在创建时设为 NULL 
			{CKA_VALUE, NULL, 0},           
		};   
		//创建数据对象，第三个参数 5 表示模版中有五项 
		//创建对象的句柄在 hObject 中 
		ck_rv = C_CreateObject(m_hSession, pDataTemplate, 5, &hObject); 
		if(ck_rv != CKR_OK) 
		{ 
			Info.Format(_T("C_CreateObject Error! 0x%08x\n"),ck_rv); 
            return FALSE;
		} 
		else 
		{ 
			Info.Format(_T("Create Data OK!\n")); 
		} 
		
	}	
	////////////////////////////////////////
	//2、将数据对象填充数据后写入
//	CK_BYTE_PTR pbData = NULL;  
//	pbData =new CK_BYTE[len ];;
//	memcpy(pbData,databuffer,len);
//	DWORD dwDataLen = sizeof(pbData); 
	//写入数据模版，pbData 和 dwDataLen 是写入数据的内容和长度 
	CK_ATTRIBUTE pWriteTemplate[]={ 
		 {CKA_VALUE,(void*)databuffer, len},
		 
		} ;
	//写入数据 
	ck_rv = C_SetAttributeValue(m_hSession, hObject, pWriteTemplate, 1); 
	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_SetAttributeValue Error! 0x%08x\n"),ck_rv); 
        return FALSE; 
	} 
	else 
	{ 
		Info.Format(_T("Write Data OK!\n")); 
	}
	return TRUE;

}

//BOOL CUK_Et199Class::SavePublicKey(CString label,unsigned char databuffer[],DWORD len,CString &Info)
//{
//	//数据对象句柄 
//	CK_OBJECT_HANDLE hObject = NULL; 
//	CK_RV ck_rv; 
//	if(GetUsbKeyObject(FALSE,FALSE,hObject,Info)==FALSE)
//	{
//		return FALSE;		
//	}	
//	////////////////////////////////////////
//	//2、将数据对象填充数据后写入
//	CK_BBOOL bFalse = FALSE;
//	CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY;
//	//写入数据模版，pbData 和 dwDataLen 是写入数据的内容和长度
//	CK_ULONG ulModulusBits = len;
//	CK_ATTRIBUTE pWriteTemplate[]={ 
//		 {CKA_MODULUS,(void*)databuffer, len},
//		//数据的标识 
//		//{CKA_LABEL, pbLabel,label.GetLength()},// sizeof(pbLabel)},         
//		//数据内容，在创建时设为 NULL 
//		//{CKA_VALUE, NULL, 0},           
//		} ;
//	//写入数据 
//	ck_rv = C_SetAttributeValue(m_hSession, hObject, pWriteTemplate, 1); 
//	if(ck_rv != CKR_OK) 
//	{ 
//		Info.Format("C_SetAttributeValue Error! 0x%08x\n",ck_rv); 
//        return FALSE; 
//	} 
//	else 
//	{ 
//		Info.Format("Write USBKey OK!\n"); 
//	}
//	return TRUE;
//
//}


DWORD CUK_Et199Class::GetData(CString label,BOOL bPrivate,unsigned char databuffer[],DWORD MaxBufferlen,CString &Info)
{
	/*databuffer[0] = 'H';
	databuffer[1] = 'S';
	databuffer[2]= '9';
	databuffer[3]= '0';
	databuffer[4]= '8';
	databuffer[5]= '8';
	databuffer[6]= '8';
	databuffer[7]= '8';
	databuffer[8] = '1';
	databuffer[9] = '0';
	databuffer[10]= '0';
	databuffer[11]= '1';
	return 12;*/
	//数据对象句柄 
	CK_OBJECT_HANDLE hObject = NULL; 
	if(GetDataObject(label,bPrivate,hObject,Info)==FALSE)
	{
		return 0;
	}
	/////////////////////////////////////////
	CK_BYTE_PTR pbReadData = NULL;  
	DWORD dwReadDataLen =0; 
	
	//模版中为 NULL 
	CK_ATTRIBUTE pReadTemplate[]={ 
		{CKA_VALUE,NULL,0}, 
	} ;
	//读取数据对象 
	CK_RV ck_rv; 
	//第一次调用先得到数据长度 
	ck_rv = C_GetAttributeValue(m_hSession, hObject, pReadTemplate, 1); 
	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_GetAttributeValue 1 Error! 0x%08x\n"),ck_rv); 
		return 0; 
	} 
	
	//分配空间，多分配一个字节补 0，为字符串结尾 
	dwReadDataLen = pReadTemplate[0].ulValueLen; 
	pbReadData = new CK_BYTE[dwReadDataLen + 1]; 
	memset(pbReadData,0, dwReadDataLen + 1); 
	
	pReadTemplate[0].pValue = pbReadData; 
	pReadTemplate[0].ulValueLen = dwReadDataLen; 
	
	ck_rv = C_GetAttributeValue(m_hSession, hObject, pReadTemplate, 1); 
	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_GetAttributeValue 2 Error! 0x%08x\n"),ck_rv); 
		return 0 ; 
	} 

	if (dwReadDataLen>MaxBufferlen)
	{
		Info.Format(_T("read OK!but buffer size too short， Len>: %d \n"), dwReadDataLen); 
		return 0;
	}
	memset(databuffer,0, dwReadDataLen + 1); 
	memcpy(databuffer,pbReadData,dwReadDataLen);
	//CString show;
	//show=nByteToStr(dwReadDataLen, databuffer,1,16);
	Info.Format(_T("Read Data OK! Len: %d "), dwReadDataLen);
	return dwReadDataLen;
	
}
DWORD CUK_Et199Class::GetPublicKey(BOOL bRemotePublicKey,unsigned char databuffer[],DWORD MaxBufferlen,CString &Info)
{
	//数据对象句柄 
	CK_OBJECT_HANDLE hObject = NULL; 
	if(GetUsbKeyObject(SRV_PUBLIC_KEY,hObject,Info)==FALSE)
	{
		return 0;
	}
	/////////////////////////////////////////
	CK_BYTE_PTR pbReadData = NULL;  
	DWORD dwReadDataLen =0; 
	
	//模版中为 NULL 
	CK_ATTRIBUTE pReadTemplate[]={ 
			{CKA_CLASS, NULL, 0},
			{CKA_KEY_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_WRAP,NULL,0},
			{CKA_ENCRYPT,NULL,0},
			{CKA_MODULUS,NULL,0},
			{CKA_PUBLIC_EXPONENT,NULL,0},
	} ;
	//读取数据对象 
	CK_RV ck_rv; 
	//第一次调用先得到数据长度 
	ck_rv = C_GetAttributeValue(m_hSession, hObject, pReadTemplate, 7); 
	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_GetAttributeValue 1 Error! 0x%08x\n"),ck_rv); 
		return 0; 
	} 
		
	//分配空间，多分配一个字节补 0，为字符串结尾 
	dwReadDataLen = pReadTemplate[5].ulValueLen; 
	pbReadData = new CK_BYTE[dwReadDataLen ]; 
	memset(pbReadData,0, dwReadDataLen ); 
	pReadTemplate[5].pValue = pbReadData; 
	pReadTemplate[5].ulValueLen = dwReadDataLen; 
		
	pReadTemplate[0].pValue = new char[pReadTemplate[0].ulValueLen];
	pReadTemplate[1].pValue = new char[pReadTemplate[1].ulValueLen];
	pReadTemplate[2].pValue = new char[pReadTemplate[2].ulValueLen+1];
	pReadTemplate[3].pValue = new char[pReadTemplate[3].ulValueLen];
	pReadTemplate[4].pValue = new char[pReadTemplate[4].ulValueLen];
	//pReadTemplate[5].pValue = new char[pReadTemplate[5].ulValueLen];
	pReadTemplate[6].pValue = new char[pReadTemplate[6].ulValueLen];
		
	ZeroMemory(pReadTemplate[0].pValue, pReadTemplate[0].ulValueLen);
	ZeroMemory(pReadTemplate[1].pValue, pReadTemplate[1].ulValueLen);
	ZeroMemory(pReadTemplate[2].pValue, pReadTemplate[2].ulValueLen+1);	
	ZeroMemory(pReadTemplate[3].pValue, pReadTemplate[3].ulValueLen);
	ZeroMemory(pReadTemplate[4].pValue, pReadTemplate[4].ulValueLen);
	//ZeroMemory(pReadTemplate[5].pValue, pReadTemplate[5].ulValueLen);
	ZeroMemory(pReadTemplate[6].pValue, pReadTemplate[6].ulValueLen);
	
	ck_rv = C_GetAttributeValue(m_hSession, hObject, pReadTemplate, 7);
	if(ck_rv != CKR_OK)
	{
		Info.Format(_T("C_GetAttributeValue 2 Error! 0x%08x\n"),ck_rv); 
		delete[] pReadTemplate[0].pValue;
		delete[] pReadTemplate[1].pValue;
		delete[] pReadTemplate[2].pValue;
		delete[] pReadTemplate[3].pValue;
		delete[] pReadTemplate[4].pValue;
		delete[] pReadTemplate[5].pValue;
		delete[] pReadTemplate[6].pValue;
		return 0;
	}

	if (dwReadDataLen>MaxBufferlen)
	{
		Info.Format(_T("readOK!but buffer size too short，shoule Len> %d \n"), dwReadDataLen); 
		return 0;
	}
	memcpy(databuffer,pbReadData,dwReadDataLen);
	delete[] pReadTemplate[0].pValue;
	delete[] pReadTemplate[1].pValue;
	delete[] pReadTemplate[2].pValue;
	delete[] pReadTemplate[3].pValue;
	delete[] pReadTemplate[4].pValue;
	delete[] pReadTemplate[5].pValue;
	delete[] pReadTemplate[6].pValue;
    //CString show;
	//show=nByteToStr(dwReadDataLen, databuffer,1,16);
	Info.Format(_T("Read USBKEY OK! Len: %d "), dwReadDataLen);
	return dwReadDataLen;
	
}
BOOL CUK_Et199Class::GetDataObject(CString label,BOOL bPrivate,CK_OBJECT_HANDLE &hObject,CString &Info)
{
	//数据对象句柄 
	hObject = NULL; 
	if(m_hSession ==NULL_PTR)
	{
		Info.Format(_T("have not connect dev"));
		return FALSE;
	}
	//1、建立数据对象
	CK_RV ck_rv; 
	//数据类型：普通数据 
	CK_OBJECT_CLASS objClass = CKO_DATA; 
	CK_BBOOL bTrue = TRUE; 
	CK_BBOOL bFalse = FALSE; 
	//数据对象名称 
	CK_BYTE pbLabel[256];
	//memcpy(pbLabel,label,label.GetLength());// = (CK_BYTE_PTR)label.GetBuffer(label.GetLength()); 
	CK_ULONG ulPIN = label.GetLength();
	for(int i=0;i<(int)ulPIN;i++)
	{
		pbLabel[i]=(CK_BYTE)label.GetAt(i);
	}
	
	
	//数据对象模版 
	CK_ATTRIBUTE pDataTemplate[]={ 
		//数据类型 
		{CKA_CLASS, &objClass, sizeof(CK_OBJECT_CLASS)},  
			//存储在 ET199 硬件中，设为 TRUE 
		{CKA_TOKEN, &bTrue, sizeof(CK_BBOOL)},   
		//是否存储在私有区里， 
		//为 TRUE，在私有区创建，需要验证 User PIN 才能读写 
		//为 FALSE，在公有区创建，不需要验证 User PIN 就可以读写 
		{CKA_PRIVATE, &bPrivate, sizeof(CK_BBOOL)},       
		//数据的标识 
		{CKA_LABEL, pbLabel,label.GetLength()},// sizeof(pbLabel)},         
		//数据内容，在创建时设为 NULL 
		{CKA_VALUE, NULL, 0},           
	}; 
	
	////////////////////////////////////////
	CK_ULONG ulFindObjectCount = 0; 	
	//2、查找数据对象 
	//按照模板的前四项查找，即数据对象，硬件里面，私有区和标签符合的 
	ck_rv = C_FindObjectsInit(m_hSession, pDataTemplate, 4);  
	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_FindObjectsInit Error! 0x%08x\n"),ck_rv); 
		return FALSE; 
	}  
	//将查找到的数据对象的句柄放到 hObject 中 
	//第 3 个参数见 PKCS#11 的接口文档 
	//当为 0 时，查找所有符合标准的数据对象 
	//当为 1 时，查找一个符合标准的数据对象 
	hObject = 0;  
	ck_rv = C_FindObjects(m_hSession, &hObject, 1, &ulFindObjectCount); 
	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_FindObjects Error! 0x%08x\n"),ck_rv); 
		return FALSE; 
	} 
	
	ck_rv = C_FindObjectsFinal(m_hSession); 
	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_FindObjectsFinal Error! 0x%08x\n"),ck_rv); 
		return FALSE; 
	} 
	
	if(ulFindObjectCount == 0) 
	{ 
		Info.Format(_T("Not data To Read!\n")); 
		return FALSE; 
	}
	/////////////////////////////////////////
	return TRUE;
	
}
//BOOL CUK_Et199Class::GetUsbKeyObject(BOOL bRemotePublicKey,BOOL bPrivate,CK_OBJECT_HANDLE &hObject,CString &Info)
BOOL CUK_Et199Class::GetUsbKeyObject(int ukey_object_type, CK_OBJECT_HANDLE &hObject, CString &Info)

{
	//Check ET199 Session Valid.
	if(m_hSession ==NULL_PTR)
	{
		Info.Format(_T("have NOT a valid session about ET199. Failed."));
		LOG(ERROR)<<"have NOT a valid session about ET199. Failed.";
		return FALSE;
	}

	CK_RV ck_rv; 
	hObject = NULL; 


	CK_OBJECT_CLASS objClass = CKO_DATA; 
	CK_BYTE label_subject[LABEL_SIZE];
	memset(label_subject, 0, LABEL_SIZE);
	
	//如果是找服务器的公钥，那就找模板前三项匹配的; 否则找前两项匹配的。
	int first_item_sum = 0;

	switch(ukey_object_type)
	{
	case SRV_PUBLIC_KEY:
		objClass=CKO_PUBLIC_KEY;
		memcpy(label_subject, SRV_PUBKEY_LABEL, strlen(SRV_PUBKEY_LABEL));
		first_item_sum = 3;
		LOG(INFO)<<"To find the public key of verify server.";
		break;
	case ET199_PRIVATE_KEY:
		objClass=CKO_PRIVATE_KEY;
		first_item_sum = 2;
		LOG(INFO)<<"To find the private key of ET199";
		break;
	case ET199_PUBLIC_KEY:
		objClass=CKO_PUBLIC_KEY;
		first_item_sum = 2;
		LOG(INFO)<<"To find the public key of ET199";
		break;
	default:
		return FALSE;
		break;
	}


	CK_BBOOL bTrue = TRUE; 
	CK_BBOOL bFalse = FALSE; 

	CK_ULONG ulModulusBits = MODULUS_BIT_LENGTH; 	
	CK_ULONG keyType = CKK_RSA;

	CK_BYTE pubExponent[4] = {0x00, 0x01, 0x00, 0x01};

	CK_BYTE subject[] = SRV_PUBKEY_LABEL;

	CK_ATTRIBUTE pubTemplate[] = { 
		//数据类型，此处表示是公钥还是私钥
		{CKA_CLASS,			&objClass,		sizeof(objClass)},
		{CKA_KEY_TYPE,		&keyType,		sizeof(keyType)},
		//{CKA_SUBJECT,		label_subject,	strlen(SRV_PUBKEY_LABEL)},
		{CKA_SUBJECT,		subject,		sizeof(subject)},
		{CKA_MODULUS_BITS,	&ulModulusBits, sizeof(ulModulusBits)},
		{CKA_ENCRYPT,		&bTrue,			sizeof(bTrue)},
		//在ET199硬件中，为CKA_TOKEN为TRUE
		{CKA_TOKEN,			&bTrue,			sizeof(bTrue)},
		{CKA_WRAP,			&bTrue,			sizeof(bTrue)},		
		{CKA_PUBLIC_EXPONENT, pubExponent,	sizeof(pubExponent)}
	};		
	////////////////////////////////////////
	CK_ULONG ulFindObjectCount = 0; 	

	//2、查找数据对象 
	//如果是找服务器的公钥，那就找模板前三项匹配的; 否则找前两项匹配的。
	ck_rv = C_FindObjectsInit(m_hSession, pubTemplate, first_item_sum); 

	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_FindObjectsInit Error! 0x%08x\n"),ck_rv); 
		return FALSE; 
	} 

	//将查找到的数据对象的句柄放到 hObject 中 
	//第 3 个参数见 PKCS#11 的接口文档 
	//当为 0 时，查找所有符合标准的数据对象 
	//当为 1 时，查找一个符合标准的数据对象 
	//此处，我们只找第一个
	ck_rv = C_FindObjects(m_hSession, &hObject, 1, &ulFindObjectCount); 
	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_FindObjects Error! 0x%08x\n"),ck_rv); 
		LOG(ERROR)<<"C_FindObjects Error code: "<<ck_rv;
		return FALSE; 
	} 
	
	ck_rv = C_FindObjectsFinal(m_hSession); 
	if(ck_rv != CKR_OK) 
	{ 
		Info.Format(_T("C_FindObjectsFinal Error! 0x%08x\n"),ck_rv); 
		LOG(ERROR)<<"C_FindObjectsFinal Error code:"<<ck_rv;
		return FALSE; 
	} 
	
	if(ulFindObjectCount == 0) 
	{ 
		Info.Format(_T("Not Find key To Read!\n")); 
		LOG(ERROR)<<"Find nothing about key.";
		return FALSE; 
	}
	LOG(INFO)<<"The key has been found.";

	return TRUE;
	
}

//摘要
BOOL CUK_Et199Class::RSA_Digest(CK_BYTE_PTR msg, CK_ULONG msg_len,
								CK_BYTE_PTR msg_sha1, CK_ULONG msg_sha1_len,
								CString &info)
{
	CK_RV rv;	

	//散列算法机制，这里使用SHA1散列算法
	CK_MECHANISM digestMechan[] = {CKM_SHA_1, NULL_PTR, 0};

	//散列初始化
	rv = C_DigestInit(m_hSession, digestMechan);
	if(CKR_OK != rv)
	{
		info.Format(_T("Fail to call C_DigestInit!Error code 0x%08X."), rv);
		LOG(ERROR)<<"Fail to call C_DigestInit!Error code: "<<rv;
		return FALSE;
	}
	LOG(INFO)<<"C_DigestInit, Success";

    //将原文数据进行SHA1散列，第一次先得到散列后的数据长度
	rv = C_Digest(m_hSession, msg, msg_len, NULL, &msg_sha1_len);
	if(CKR_OK != rv)
	{
		info.Format(_T("C_Digest 1st Error: %08x\n"), rv);
		LOG(ERROR)<<"C_Digest 1st, Know the length, failed. Err code:"<<rv;
		return FALSE;
		
	}
	LOG(INFO)<<"C_Digest, Know the length, Success"; 

	//SHA1_LEN = 20 according to the protocols
	if(20!=msg_sha1_len){
		info.Format(_T("SHA1_LEN equals 20, but it equals to %d.\n"), msg_sha1_len);
		LOG(ERROR)<< "SHA1_LEN equals 20, but it equals to "<< msg_sha1_len;
		return FALSE;
	}
	LOG(INFO)<<"SHA1_LEN equals 20, but it equals to " <<msg_sha1_len;

	memset(msg_sha1, 0, msg_sha1_len);
	//将原文数据进行SHA1散列，第二次得到散列后的数据结果
	rv = C_Digest(m_hSession, msg, msg_len, msg_sha1, &msg_sha1_len);	
	if(CKR_OK != rv)
	{
		info.Format(_T("Fail to C_Digest 2nd !Error code 0x%08X."), rv);
		LOG(ERROR)<<"Fail to C_Digest 2nd !Error code:"<<rv; 
		return FALSE;
	}
	else
	{
		info.Format(_T(" SHA1 successfully!"));
		LOG(INFO)<<" C_Digest 2nd, SHA1 successfully!";
	}
	return TRUE;
	
}

//签名
BOOL CUK_Et199Class::RSA_Signed(CK_BYTE_PTR pbMsg, CK_ULONG ulMsgLen,
								CK_BYTE_PTR pSignature, CK_ULONG_PTR ulSignatureLen,
								CString &Info)
{
	//get the private key
	CK_OBJECT_HANDLE hPriKey = NULL; 
	if(GetUsbKeyObject(ET199_PRIVATE_KEY,hPriKey,Info)==FALSE)
	{
		LOG(ERROR)<<"Get private key for signed, failed.";
		return FALSE;
	}
	LOG(INFO)<<"Get private key for signed, success.";

	//PKCS 标准填充前15个字节
	CK_BYTE tsha1[35] = 
	{0x30, 0x21, 0x30, 0x09, 0x06, 
	 0x05, 0x2b, 0x0e, 0x03, 0x02, 
	 0x1a, 0x05, 0x00, 0x04, 0x14};

	memcpy(tsha1+15, pbMsg, ulMsgLen);

	int tsha1_len = 0;
	tsha1_len = 35;

	CK_RV rv;
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	rv = C_SignInit(m_hSession, &ckMechanism, hPriKey);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Fail to call SignInit!Error code 0x%08X."), rv);
		LOG(ERROR)<<"C_SignInit, failed. err code: "<<rv;
		return FALSE;
	}
	LOG(INFO)<<"C_signInit, success.";

	rv = C_Sign(m_hSession, tsha1, tsha1_len, pSignature, ulSignatureLen);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Fail to Sign!Error code 0x%08X."), rv);
		LOG(ERROR)<<"C_Sign, failed. err code:"<<rv;
		return FALSE;
	}
	else
	{
		Info.Format(_T(" Adding the signature successfully!"));	
		LOG(INFO)<<"C_Sign, success.";
	}
	return TRUE;
	
}
//签名验证
BOOL CUK_Et199Class::RSA_Verify(int test_flag, 
								CK_BYTE_PTR pbMsg, CK_ULONG ulMsgLen,
								CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen,
								CString &info)
{
	//read the RSA public key of UKEY or verify server according to the value of bRmotePublicKey
	CK_OBJECT_HANDLE hPubKey = NULL; 

	int public_key_flag = 0;
	if(1==test_flag){
		public_key_flag = ET199_PUBLIC_KEY;
	}else{
		public_key_flag = SRV_PUBLIC_KEY;
	}

	//PKCS 标准填充前15个字节
	CK_BYTE tsha1[35] = 
	{0x30, 0x21, 0x30, 0x09, 0x06, 
	0x05, 0x2b, 0x0e, 0x03, 0x02, 
	0x1a, 0x05, 0x00, 0x04, 0x14};

	memcpy(tsha1+15, pbMsg, ulMsgLen);

	int tsha1_len = 0;
	tsha1_len = 35;

	if(GetUsbKeyObject(public_key_flag,hPubKey,info)==FALSE)
	{
		LOG(ERROR)<<"Get Public key for verify, failed.";
		return FALSE;
	}
	LOG(INFO)<<"Get public key for verify, success";

	CK_RV rv;
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	rv = C_VerifyInit(m_hSession, &ckMechanism, hPubKey);
	if(CKR_OK != rv)
	{
		info.Format(_T("Failed to call VerifyInit!Error code 0x%08X."), rv);
		LOG(ERROR)<<"C_VerifyInit, failed, erro code:"<<rv;
		return FALSE;
	}
	rv = C_Verify(m_hSession, tsha1, tsha1_len, pSignature, ulSignatureLen);
	if(CKR_OK != rv)
	{
		info.Format(_T("Fail to call verify!Error code 0x%08X."), rv);
		LOG(ERROR)<<"C_Verify, failed. err code:" << rv;
		return FALSE;
	}
	else{
		info.Format(_T("Verify Successfully!"));
		LOG(INFO)<<"C_Verify, success.";
		return TRUE;
	}
	
}

//
//加密
BOOL CUK_Et199Class::RSA_Encrypt(int test_flag,
								 CK_BYTE_PTR pbMsg, CK_ULONG ulMsgLen,
								CK_BYTE_PTR pCipherBuffer, CK_ULONG_PTR ulCipherLen,
								CString &info)
{
	//read the RSA public key of UKEY or verify server according to the value of bRmotePublicKey
	CK_OBJECT_HANDLE hPubKey = NULL; 
	int public_key_flag = 0;
	if(1==test_flag){
		public_key_flag = ET199_PUBLIC_KEY;
	}else{
		public_key_flag = SRV_PUBLIC_KEY;
	}

	if(GetUsbKeyObject(public_key_flag, hPubKey,info)==FALSE)
	{
		LOG(ERROR)<<"Get Public key for Encrypt, failed.";
		return FALSE;
	}

	LOG(INFO)<<"Get public key for encrypt, success.";
	CK_RV rv;
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	rv = C_EncryptInit(m_hSession, &ckMechanism, hPubKey);
	if(CKR_OK != rv)
	{
		info.Format(_T("Fail to call C_EncryptInit!Error code 0x%08X."), rv);
		LOG(ERROR)<<"C_EncryptInit, failed. err code:"<<rv;
		return FALSE;
	}
	LOG(INFO)<<"C_EncryptInit, success.";

	rv = C_Encrypt(m_hSession, pbMsg, ulMsgLen, NULL_PTR, ulCipherLen);
	if(CKR_OK != rv)
	{
		info.Format(_T("Can't acquire the size of Data After encrypt,Error code 0x%08X."), rv);
		LOG(ERROR)<<"C_Encrypt 1st, to get length of cipher, failed"<<rv;
		return FALSE;
	}

	LOG(INFO)<<"C_Encrypt 1st, to get length of cipher, success." << *ulCipherLen;

	rv = C_Encrypt(m_hSession, pbMsg, ulMsgLen, pCipherBuffer, ulCipherLen);
	if (CKR_OK != rv)
	{
		info.Format(_T("Fail to encrypt!Error code 0x%08X."), rv);
		LOG(ERROR)<<"C_Encrypt 2nd, do encrypt, failed. err code:"<<rv;
		return FALSE;
	}
	else
	{
		info.Format(_T(" was Encrypt successfully!"));
		LOG(INFO)<<"C_Encrypt 2nd, do encrypt, success.";
	}

	return TRUE;
	
}

//解密
BOOL CUK_Et199Class::RSA_Decrypt(CK_BYTE_PTR pCipherBuffer, CK_ULONG ulCipherLen,
								 CK_BYTE_PTR pbMsg, CK_ULONG_PTR ulMsgLen,								
								 CString &info)
{
	//read the RSA private key of UKEY 
	CK_OBJECT_HANDLE hPriKey = NULL; 
	if(GetUsbKeyObject(ET199_PRIVATE_KEY,hPriKey,info)==FALSE)
	{
		LOG(ERROR)<<"get private key for decrypt, failed.";
		return FALSE;
	}
	LOG(INFO)<<"get private key for decrypt, success.";

	CK_RV rv;
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	rv = C_DecryptInit(m_hSession, &ckMechanism, hPriKey);
	if(CKR_OK != rv)
	{
		info.Format(_T("Fail to call C_DecryptInit!Error code 0x%08X."), rv);
		LOG(ERROR)<<"C_DecryptInit failed, error code:"<<rv;
		return FALSE;
	}
	LOG(INFO)<<"C_DecryptInit, success.";
	rv = C_Decrypt(m_hSession, pCipherBuffer, ulCipherLen, NULL_PTR, ulMsgLen);
	if(CKR_OK != rv)
	{
		info.Format(_T("Can't acquire size of Data after Decrypt,Error code 0x%08X."), rv);
		LOG(ERROR)<<" C_Decrypte 1st. Can't acquire size of Data after Decrypt,Error code:"<<rv;
		return FALSE;
	}
	LOG(INFO)<<" C_Decrypte 1st. Acquire size of Data after Decrypt, success." << *ulMsgLen;
	rv = C_Decrypt(m_hSession, pCipherBuffer, ulCipherLen, pbMsg, ulMsgLen);
	if (CKR_OK != rv)
	{
		info.Format(_T("Fail to Decrypt!Error code 0x%08X."), rv);
		LOG(ERROR)<<"C_Decrypt 2nd, Failed. erro code: "<<rv;
		return FALSE;
	}
	else
	{
		info.Format(_T("Decrypt Successfully."));
		LOG(INFO)<<"C_Decrypt 2nd, success.";
	}
	return TRUE;
	
}