// UK_Et199Class.cpp : 实现文件
//

#include "stdafx.h"
//#include "ChangeAdminActiveX.h"


#include "UK_Et199Class.h"



#define	countof(a)			(sizeof(a)/ sizeof(CK_ATTRIBUTE))

CK_BBOOL bTrue = TRUE;
CK_BBOOL bFalse = FALSE;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// byte to string according with special format
//static CString nByteToStr(DWORD dwSize, void* pData, DWORD dwByte, DWORD dwSplit);
//
//CString nByteToStr(DWORD dwSize, void* pData, DWORD dwByte, DWORD dwSplit)
//{
//	BYTE* pBuf = (BYTE*)pData; // local pointer to a BYTE in the BYTE array.
//	
//	CString strRet("");	
//	DWORD nLine = 0;	
//	DWORD dwLines = 0;
//	DWORD dwRest = 0;
//	bool bNeedSplit = true;
//	char szTemp[20] = {0, };
//	
//	DWORD dwBlock = 0;	
//	if(0 == dwSplit)
//	{
//		dwSplit = dwSize;
//		bNeedSplit = false;
//	}
//	
//	dwRest = dwSize % dwSplit;
//	dwLines = dwSize / dwSplit;
//	
//	
//	DWORD i, j, k, m;
//	for(i = 0; i < dwLines; i++)
//	{
//		DWORD dwRestTemp = dwSplit % dwByte;
//		DWORD dwByteBlock = dwSplit / dwByte;
//		
//		for(j = 0; j < dwByteBlock; j++)
//		{
//			for(k = 0; k < dwByte; k++)
//			{
//				wsprintf(szTemp, "%02X", pBuf[i * dwSplit + j * dwByte + k]);
//				strRet += szTemp;
//			}
//			strRet += " ";
//		}
//		if(dwRestTemp)
//		{
//			for(m = 0; m < dwRestTemp; m++)
//			{
//				wsprintf(
//					szTemp, "%02X",
//					pBuf[i * dwSplit + j * dwByte + k - 3 + dwRestTemp]);
//				strRet += szTemp;
//			}
//		}
//		if(bNeedSplit)
//			strRet += NEWLINE;
//	}
//	
//	if(dwRest)
//	{
//		DWORD dwRestTemp = dwRest % dwByte;
//		DWORD dwByteBlock = dwRest / dwByte;
//		for(j = 0; j < dwByteBlock; j++)
//		{
//			for(k = 0; k < dwByte; k++)
//			{
//				wsprintf(szTemp, "%02X", pBuf[dwSize - dwRest + k]);
//				strRet += szTemp;
//			}
//			strRet += " ";
//		}
//		if(dwRestTemp)
//		{
//			for(m = 0; m < dwRestTemp; m++)
//			{
//				wsprintf(
//					szTemp, "%02X",
//					pBuf[dwSize - dwRest + k - 1 + dwRestTemp]);
//				strRet += szTemp;
//			}
//		}
//		if(bNeedSplit)
//			strRet += NEWLINE;
//	}
//	
//	
//	return strRet;
//}  // End of ByteToStr

CUK_Et199Class::CUK_Et199Class()
{
	m_pSlotList = NULL_PTR;
	m_pApplication = new char[255];
	ZeroMemory(m_pApplication, 255);
	//USES_CONVERSION;
	//char *pStr = T2A("PKCS ET199 App");
	//memcpy(m_pApplication,pStr,)
    //m_pApplication="PKCS ET199 App";
	memcpy(m_pApplication,"PKCS ET199 App",strlen("PKCS ET199 App"));
	//lstrcpy(m_pApplication, "PKCS ET199 App");
	m_hSession = NULL_PTR;

}

CUK_Et199Class::~CUK_Et199Class()
{
	DisConnectDev();
	/*if(m_pApplication)
	{
		delete[] m_pApplication;
		
	}*/
	
	delete[] m_pApplication;
	if(m_pSlotList)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}


}
void CUK_Et199Class::DisConnectDev(void)
{
	if(m_hSession)
	{
		C_Logout(m_hSession);
		C_CloseSession(m_hSession);
		m_hSession = NULL_PTR;
	}
	/*delete[] m_pApplication;
	if(m_pSlotList)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}*/
	m_strUserPIN="";
}

BOOL CUK_Et199Class::TestHardDev(CString &Info)
{

 
  return 0;

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

BOOL CUK_Et199Class::SaveRometePublicKey(unsigned char keybuffer[],DWORD keylen,CString &info) 
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
		info.Format(_T("Can't Generate Romete RSA Public Key , ErrorCode: 0x%08X."), rv);
		return FALSE;
	}
	else
	{
		
		info.Format(_T("Generate Romete RSA Public Key Successfully!"));
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
DWORD CUK_Et199Class::GetPublicKey(BOOL bRometPublickey,unsigned char databuffer[],DWORD MaxBufferlen,CString &Info)
{
	//数据对象句柄 
	CK_OBJECT_HANDLE hObject = NULL; 
	if(GetUsbKeyObject(bRometPublickey,FALSE,hObject,Info)==FALSE)
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
BOOL CUK_Et199Class::GetUsbKeyObject(BOOL bRometPublickey,BOOL bPrivate,CK_OBJECT_HANDLE &hObject,CString &Info)
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
	if (bPrivate==TRUE)
	{
		objClass=CKO_PRIVATE_KEY;
	}
	else
	{
		objClass=CKO_PUBLIC_KEY;
	}
	CK_BBOOL bTrue = TRUE; 
	CK_BBOOL bFalse = FALSE; 
	//数据对象名称 
	//CK_BYTE pbLabel[256];
	//memcpy(pbLabel,label,label.GetLength());// = (CK_BYTE_PTR)label.GetBuffer(label.GetLength()); 
	//2、生产密钥对
	CK_ULONG ulModulusBits = MODULUS_BIT_LENGTH; 	
	CK_ULONG keyType = CKK_RSA;
	//CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY;
	CK_BYTE pubExponent[4] = {0x00, 0x01, 0x00, 0x01};
	CK_BYTE subject[] = "Romete RSA Public Key";
	CK_ATTRIBUTE pubTemplate[] = { 
		{CKA_CLASS,			&objClass,		sizeof(objClass)},
		{CKA_KEY_TYPE,		&keyType,		sizeof(keyType)},
		{CKA_SUBJECT,		subject,		sizeof(subject)},
		{CKA_MODULUS_BITS,	&ulModulusBits, sizeof(ulModulusBits)},
		{CKA_ENCRYPT,		&bTrue,			sizeof(bTrue)},
		{CKA_TOKEN,			&bTrue,			sizeof(bTrue)},
		{CKA_WRAP,			&bTrue,			sizeof(bTrue)},		
		{CKA_PUBLIC_EXPONENT, pubExponent,	sizeof(pubExponent)}
	};		
	////////////////////////////////////////
	CK_ULONG ulFindObjectCount = 0; 	
	//2、查找数据对象 
	//按照模板的前四项查找，即数据对象，硬件里面，私有区和标签符合的 
	if (bRometPublickey==TRUE)
	{
		ck_rv = C_FindObjectsInit(m_hSession, pubTemplate, 3); 
	}
	else
	{
		ck_rv = C_FindObjectsInit(m_hSession, pubTemplate, 2);  
	}
	
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
		Info.Format(_T("Not Find key To Read!\n")); 
		return FALSE; 
	}
	/////////////////////////////////////////
	return TRUE;
	
}

//摘要
BOOL CUK_Et199Class::RSA_Digest(unsigned char pbMsg[],DWORD ulMsgLen,
								unsigned char pMsgSHA1[],DWORD ulMaxMsgSHA1Len,DWORD &ulMsgSHA1Len,
								CString &Info)
{

	////////////////////////////////////////
	//散列结果
//	CK_BYTE_PTR pbMsgSHA1 = NULL;
//	//散列结果长度
//	CK_ULONG cbMsgSHA1 = 0;
	CK_RV rv;	
	//散列算法机制，这里使用SHA1散列算法
	CK_MECHANISM digestMechan[] = {CKM_SHA_1, NULL_PTR, 0};
	//散列初始化
	rv = C_DigestInit(m_hSession, digestMechan);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Fail to call C_DigestInit!Error code 0x%08X."), rv);
		return FALSE;
	}
	
    //将原文数据进行SHA1散列，第一次先得到散列后的数据长度
	rv = C_Digest(m_hSession, pbMsg, ulMsgLen, NULL, &ulMsgSHA1Len);
	if(CKR_OK != rv)
	{
		Info.Format(_T("C_Digest 1 Error: %08x\n"), rv);
		return FALSE;
		
	}
	if (ulMsgSHA1Len>ulMaxMsgSHA1Len)
	{
		Info.Format(_T("SHA1 buffer size too short， ulMsgSHA1Len: %d\n"), ulMsgSHA1Len);
		return FALSE;
	}
	//分配空间
	memset(pMsgSHA1, 0, ulMsgSHA1Len);
	//将原文数据进行SHA1散列，第二次得到散列后的数据结果
	rv = C_Digest(m_hSession, pbMsg, ulMsgLen, pMsgSHA1, &ulMsgSHA1Len);	
	if(CKR_OK != rv)
	{
		Info.Format(_T("Fail to C_Digest2!Error code 0x%08X."), rv);
		return FALSE;
	}
	else
	{
		/*Info.Format(NEWLINE"Data:"NEWLINE);
		Info=Info+((char*)pbMsg);*/
		CString temps;
		temps.Format(_T(" SHA1 successfully!"));
		/*Info+=temps;
		temps.Format(nByteToStr(ulMsgSHA1Len, pMsgSHA1, 1, 16));
		Info+=temps;
		temps.Format(NEWLINE"Now you can 对散列结果进行签名，或校验!\n");*/
		Info+=temps;
		//////////////////////
//		CString show;
//		show=nByteToStr(ulSignatureLen, pSignature,1,16);
//		temps.Format("Signed Data OK! Len: %d Signed Data is: \r\n%s\n", ulSignatureLen,show);
//		Info+=temps;
	}
	return TRUE;
	
}

//签名
BOOL CUK_Et199Class::RSA_Signed(unsigned char pbMsg[],DWORD ulMsgLen,
								unsigned char pSignature[],DWORD &ulSignatureLen,
								CString &Info)
{
	//数据对象句柄 
	CK_OBJECT_HANDLE m_hPriKey = NULL; 
	if(GetUsbKeyObject(FALSE,TRUE,m_hPriKey,Info)==FALSE)
	{
		return FALSE;
	}
	/////////////////////////////////////////
	CK_RV rv;
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	rv = C_SignInit(m_hSession, &ckMechanism, m_hPriKey);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Fail to call SignInit!Error code 0x%08X."), rv);
		return FALSE;
	}
	
	/*Info.Format(_T("Signed Data is:"NEWLINE);
	Info=Info+nByteToStr(ulMsgLen, pbMsg, 1, 16);*/
	//unsigned char pchar[1024];
	//memset(pchar,0,1024);
	//ulSignatureLen=SignedData_LEN;//不可删除
	rv = C_Sign(m_hSession, 
		pbMsg,
		ulMsgLen, 
		pSignature, &ulSignatureLen);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Fail to Sign!Error code 0x%08X."), rv);
		return FALSE;
	}
	else
	{
		/*Info.Format(NEWLINE"Data:"NEWLINE);
		Info=Info+((char*)pbMsg);*/
		Info.Format(_T(" was Signed successfully!"));		
	}
	return TRUE;
	
}
//签名验证
BOOL CUK_Et199Class::RSA_Verify_PublicKey(BOOL bRometPublicKey,unsigned char pbMsg[],DWORD ulMsgLen,
								unsigned char pSignature[],DWORD ulSignatureLen,
								CString &Info)
{
	//数据对象句柄 
	CK_OBJECT_HANDLE m_hPubKey = NULL; 
	if(GetUsbKeyObject(bRometPublicKey,FALSE,m_hPubKey,Info)==FALSE)
	{
		return FALSE;
	}
	/////////////////////////////////////////
	CK_RV rv;
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	rv = C_VerifyInit(m_hSession, &ckMechanism, m_hPubKey);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Failed to call VerifyInit!Error code 0x%08X."), rv);
		return FALSE;
	}
	rv = C_Verify(m_hSession,
		pbMsg, ulMsgLen, 
		pSignature, ulSignatureLen);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Fail to call verify!Error code 0x%08X."), rv);
		return FALSE;
	}
	else
	{	/*Info.Format(NEWLINE"Signed Data is:"NEWLINE);
	    Info=Info+nByteToStr(ulMsgLen, pbMsg, 1, 16);*/
		Info.Format(_T("Verify Successfully!"));
	}
	return TRUE;
	
}

//
//加密
BOOL CUK_Et199Class::RSA_Encrypt(BOOL bRometePublicKey,unsigned char pbMsg[],DWORD ulMsgLen,
								unsigned char pCipherBuffer[],DWORD &ulCipherLen,
								CString &Info)
{
	//数据对象句柄 
	CK_OBJECT_HANDLE m_hPubKey = NULL; 
	if(GetUsbKeyObject(bRometePublicKey,FALSE,m_hPubKey,Info)==FALSE)
	{
		return FALSE;
	}
	/////////////////////////////////////////
	CK_RV rv;
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	rv = C_EncryptInit(m_hSession, &ckMechanism, m_hPubKey);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Fail to call C_EncryptInit!Error code 0x%08X."), rv);
		return FALSE;
	}
	rv = C_Encrypt(m_hSession, pbMsg, ulMsgLen, NULL_PTR, &ulCipherLen);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Can't acquire the size of Data After encrypt,Error code 0x%08X."), rv);
		return FALSE;
	}

	rv = C_Encrypt(m_hSession, pbMsg, ulMsgLen, pCipherBuffer, &ulCipherLen);
	if (CKR_OK != rv)
	{
		Info.Format(_T("Fail to encrypt!Error code 0x%08X."), rv);
		return FALSE;
	}
	else
	{
		/*Info.Format(NEWLINE"Data:"NEWLINE);
		Info=Info+((char*)pbMsg);*/
		CString temps;
		temps.Format(_T(" was Encrypt successfully!"));
		/*Info+=temps;
		temps.Format(nByteToStr(ulCipherLen, pCipherBuffer, 1, 16));
		Info+=temps;
		temps.Format(NEWLINE"Now you can do RSA 解密!\n");*/
		Info+=temps;
		//////////////////////
	}
	return TRUE;
	
}

//解密
BOOL CUK_Et199Class::RSA_Decrypt(unsigned char pCipherBuffer[],DWORD ulCipherLen,
								 unsigned char pbMsg[],DWORD &ulMsgLen,								
								 CString &Info)
{
	//数据对象句柄 
	CK_OBJECT_HANDLE m_hPriKey = NULL; 
	if(GetUsbKeyObject(FALSE,TRUE,m_hPriKey,Info)==FALSE)
	{
		return FALSE;
	}
	/////////////////////////////////////////
	CK_RV rv;
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	rv = C_DecryptInit(m_hSession, &ckMechanism, m_hPriKey);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Fail to call C_DecryptInit!Error code 0x%08X."), rv);
		return FALSE;
	}
	rv = C_Decrypt(m_hSession, pCipherBuffer, ulCipherLen, NULL_PTR, &ulMsgLen);
	if(CKR_OK != rv)
	{
		Info.Format(_T("Can't acuire size of Data after Decrypt,Error code 0x%08X."), rv);
		return FALSE;
	}
	memset(pbMsg,0,ulMsgLen+1);	
	rv = C_Decrypt(m_hSession, pCipherBuffer, ulCipherLen, pbMsg, &ulMsgLen);
	if (CKR_OK != rv)
	{
		Info.Format(_T("Fail to Decrypt!Error code 0x%08X."), rv);
		//////////////////////////
		//?????????????
		DisConnectDev();
		/*if(m_hSession)
		{
			C_CloseSession(m_hSession);
			m_hSession = NULL_PTR;
		}		
		if(m_pSlotList)
		{
			delete[] m_pSlotList;
			m_pSlotList = NULL_PTR;
		}*/
		
		ConnectDev(m_strUserPIN,Info);	
		return FALSE;
	}
	else
	{
		
		//CString temps;
		Info.Format(_T("Decrypt Successfulluy"));
		//temps.Format(nByteToStr(ulMsgLen, pbMsg, 1, 16));
		//Info+=temps;
		
	}
	return TRUE;
	
}