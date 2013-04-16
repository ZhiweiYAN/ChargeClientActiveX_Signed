#pragma once

//#include "Include/pkcs11/cryptoki.h"
//#include "Include/pkcs11/wincrypt.h"
#include <cryptoki_ext.h>
#include <auxiliary.h>


const CK_ULONG MODULUS_BIT_LENGTH = 1024;	// should be  512/1024/2048

// CUK_Et199Class 命令目标
#define HASH_SHA1_LEN 20
#define ET199_PRIVATE_KEY 1
#define ET199_PUBLIC_KEY 2
#define SRV_PUBLIC_KEY 3
#define LABEL_SIZE 256
#define SRV_PUBKEY_LABEL "Romete RSA Public Key"

class CUK_Et199Class 
{
private:
	//BYTE m_bHash[HASH_SHA1_LEN];
	CK_SLOT_ID_PTR m_pSlotList;
	CK_VOID_PTR m_pApplication;
	CK_SESSION_HANDLE m_hSession;
	
	//CK_BYTE m_pSignature[MODULUS_BIT_LENGTH];
	CString m_strUserPIN;

public:
	////////////////////////////////////////////////////
	//功能：联结设备
	//输入：CString strUserPIN  用户PIN
	//输出：CString &info       辅助信息
	//返回：BOOL                联结设备状态
	BOOL ConnectDev(CString strUserPIN,CString &info);

	////////////////////////////////////////////////////
	//功能：断开联结的设备
	//输入：void
	//输出：无
	//返回：void
	void DisConnectDev(void);
	
	////////////////////////////////////////////////////
	//功能：RSA签名
	//输入：
	//      unsigned char pbMsg[]       欲签名信息
	//      DWORD ulMsgLen              长度
	//输出：pSignature[]                签名后结果信息
	//      DWORD &ulSignatureLen       长度
	//      CString &info               辅助信息
	//返回：BOOL                        状态
	BOOL RSA_Signed(CK_BYTE_PTR pbMsg, CK_ULONG ulMsgLen,
		CK_BYTE_PTR pSignature, CK_ULONG_PTR ulSignatureLen,
		CString &Info);
	
	///////////////////////////////////////////
	////////////////////////////////////////////////////
	//功能：签名前的散列操作（摘要）
	//输入：
	//      unsigned char pbMsg[]       欲签名信息
	//      DWORD ulMsgLen              长度
	//      DWORD ulMaxMsgSHA1Len     散列结果最大长度
	//输出：pMsgSHA1[]                散列结果信息
	//      DWORD &ulMsgSHA1Len       散列结果长度
	//      CString &info               辅助信息
	//返回：BOOL                        状态
	BOOL RSA_Digest(CK_BYTE_PTR msg, CK_ULONG msg_len,
		CK_BYTE_PTR msg_sha1, CK_ULONG msg_sha1_len,
		CString &info);
	////////////////////////////////////////////////////
	//功能：RSA签名验证(使用公钥PublicKey)
	//输入：BOOL bRemotePublicKey,     (TRUE:获取subjec="Romete RSA Public Key"的公钥；
	//                                  FALSE：第一个公钥)
	//      unsigned char pbMsg[]       欲签名信息
	//      DWORD ulMsgLen              长度
	//输出：pSignature[]                签名后结果信息
	//      DWORD &ulSignatureLen       长度
	//      CString &info               辅助信息
	//返回：BOOL                        状态
	BOOL RSA_Verify(BOOL bRemotePublicKey, 
		CK_BYTE_PTR pbMsg, CK_ULONG ulMsgLen,
		CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen,
		CString &info);

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	//功能：RSA加密(使用公钥PublicKey)
	//输入：BOOL bRemotePublicKey,     (TRUE:获取subjec="Romote RSA Public Key"的公钥；
	//                                  FALSE：第一个公钥)
	//      unsigned char pbMsg[]       欲签名信息
	//      DWORD ulMsgLen              长度
	//输出：pCipherBuffer[]             密文信息
	//      DWORD &ulCipherLen          长度
	//      CString &info               辅助信息
	//返回：BOOL              
	BOOL RSA_Encrypt(BOOL bRemotePublicKey,
		CK_BYTE_PTR pbMsg, CK_ULONG ulMsgLen,
		CK_BYTE_PTR pCipherBuffer, CK_ULONG_PTR ulCipherLen,
		CString &info);

	//功能：RSA解密(使用私钥)
	//输入：pCipherBuffer[]              密文信息
	//      DWORD ulCipherLen            长度     
	//输出：unsigned char pbMsg[]        解密后信息
	//      DWORD &ulMsgLen              长度
	//      CString &info                辅助信息
	//返回：BOOL              
	BOOL RSA_Decrypt(CK_BYTE_PTR pCipherBuffer, CK_ULONG ulCipherLen,
								 CK_BYTE_PTR pbMsg, CK_ULONG_PTR ulMsgLen,								
								 CString &Info);

	////////////////////////////////////////////////////
	//功能：设备底层格式化
	//输入：CString strTokenName   设备名
	//      CString strSOPin       管理员PIN
	//      CString strUserPin     用户PIN
	//输出：CString &info          辅助信息
	//返回：BOOL                   状态
	BOOL FormatDev(CString strTokenName,CString strSOPin,CString strUserPin,CString &info);
	
	BOOL TestHardDev(CString &Info);	
	
	////////////////////////////////////////////////////
	//功能：将数据保存到指定区域（非USBKey）
	//输入：CString label               信息的label
	//      BOOL bPrivate               是否存放到保密区
	//      unsigned char databuffer[]  信息
	//      DWORD len                   长度
	//输出：CString &info               辅助信息
	//返回：BOOL                        状态
	BOOL SaveUSBData(CString label,BOOL bPrivate,unsigned char databuffer[],DWORD len,CString &Info);
	
	BOOL SaveRemotePublicKey(unsigned char keybuffer[],DWORD keylen,CString &info);
	//BOOL SavePublicKey(CString label,unsigned char databuffer[],DWORD len,CString &Info);

	////////////////////////////////////////////////////
	//功能：在指定区域（非USBKey）查询指定label的数据
	//输入：CString label               信息的label
	//      BOOL bPrivate               是否保密区
	//      DWORD MaxBufferlen          信息最大长度	
	//输出：unsigned char databuffer[]  查询到的信息
	//      CString &info               辅助信息
	//返回：DWORD                       查询到的数据长度
	DWORD GetData(CString label,BOOL bPrivate,unsigned char databuffer[],DWORD MaxBufferlen,CString &Info);
	
	////////////////////////////////////////////////////
	//功能：查询公钥USBKey的数据
	//输入：BOOL bRemotePublicKey,     (TRUE:获取subjec="Romete RSA Public Key"的公钥；
	//                                  FALSE：第一个公钥)
	//      DWORD MaxBufferlen          信息最大长度	
	//输出：unsigned char databuffer[]  查询到的信息
	//      CString &info               辅助信息
	//返回：DWORD                       查询到的公钥数据长度
	DWORD GetPublicKey(BOOL bRemotePublicKey,unsigned char databuffer[],DWORD MaxBufferlen,CString &Info);
	
	////////////////////////////////////////////////////
	//功能：制作RSA标准的非对称密钥对，存于设备
	//输入：	
	//输出：CString &info               辅助信息
	//返回：BOOL                      操作结果
	BOOL MakeKeypairgen(CString &info);
	CUK_Et199Class();
	virtual ~CUK_Et199Class();
protected:
	////////////////////////////////////////////////////
	//功能：在数据区查询指定label的句柄（非USBKey）
	//输入：CString label               信息的label
	//      BOOL bPrivate               是否存放在保密区
	//输出：CK_OBJECT_HANDLE &hObject   数据句柄
	//      CString &info               辅助信息
	//返回：BOOL                        状态
	BOOL GetDataObject(CString label,BOOL bPrivate,CK_OBJECT_HANDLE &hObject,CString &Info);

	////////////////////////////////////////////////////
	//功能：在USBKey区查询指定密钥的句柄（USBKey）
	//输入：BOOL bRemotePublicKey,     (TRUE:获取subjec="Romete RSA Public Key"的公钥；
	//                                  FALSE：第一个公钥)
	//      BOOL bPrivate               是否存放在保密区（私钥）
	//输出：CK_OBJECT_HANDLE &hObject   密钥句柄
	//      CString &info               辅助信息
	//返回：BOOL                        状态
	BOOL CUK_Et199Class::GetUsbKeyObject(int ukey_object_type, CK_OBJECT_HANDLE &hObject, CString &Info);
	//BOOL GetUsbKeyObject(BOOL bRemotePublicKey,BOOL bPrivate,CK_OBJECT_HANDLE &hObject,CString &Info);
};



