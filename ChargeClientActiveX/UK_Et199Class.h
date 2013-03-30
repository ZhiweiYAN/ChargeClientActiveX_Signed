#pragma once

//#include "Include/pkcs11/cryptoki.h"
//#include "Include/pkcs11/wincrypt.h"
#include <cryptoki_ext.h>
#include <auxiliary.h>


const CK_ULONG MODULUS_BIT_LENGTH = 1024;	// should be  512/1024/2048

// CUK_Et199Class ����Ŀ��
#define HASH_SHA1_LEN 20

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
	//���ܣ������豸
	//���룺CString strUserPIN  �û�PIN
	//�����CString &info       ������Ϣ
	//���أ�BOOL                �����豸״̬
	BOOL ConnectDev(CString strUserPIN,CString &info);

	////////////////////////////////////////////////////
	//���ܣ��Ͽ�������豸
	//���룺void
	//�������
	//���أ�void
	void DisConnectDev(void);
	
	////////////////////////////////////////////////////
	//���ܣ�RSAǩ��
	//���룺
	//      unsigned char pbMsg[]       ��ǩ����Ϣ
	//      DWORD ulMsgLen              ����
	//�����pSignature[]                ǩ��������Ϣ
	//      DWORD &ulSignatureLen       ����
	//      CString &info               ������Ϣ
	//���أ�BOOL                        ״̬
	BOOL RSA_Signed(unsigned char pbMsg[],DWORD ulMsgLen,
						unsigned char pSignature[],DWORD &ulSignatureLen,CString &Info);
	
	///////////////////////////////////////////
	////////////////////////////////////////////////////
	//���ܣ�ǩ��ǰ��ɢ�в�����ժҪ��
	//���룺
	//      unsigned char pbMsg[]       ��ǩ����Ϣ
	//      DWORD ulMsgLen              ����
	//      DWORD ulMaxMsgSHA1Len     ɢ�н����󳤶�
	//�����pMsgSHA1[]                ɢ�н����Ϣ
	//      DWORD &ulMsgSHA1Len       ɢ�н������
	//      CString &info               ������Ϣ
	//���أ�BOOL                        ״̬
	BOOL RSA_Digest(unsigned char pbMsg[],DWORD ulMsgLen,
								unsigned char pMsgSHA1[],DWORD ulMaxMsgSHA1Len,DWORD &ulMsgSHA1Len,
								CString &Info);
	////////////////////////////////////////////////////
	//���ܣ�RSAǩ����֤(ʹ�ù�ԿPublicKey)
	//���룺BOOL bRometPublickey,     (TRUE:��ȡsubjec="Romete RSA Public Key"�Ĺ�Կ��
	//                                  FALSE����һ����Կ)
	//      unsigned char pbMsg[]       ��ǩ����Ϣ
	//      DWORD ulMsgLen              ����
	//�����pSignature[]                ǩ��������Ϣ
	//      DWORD &ulSignatureLen       ����
	//      CString &info               ������Ϣ
	//���أ�BOOL                        ״̬
	BOOL RSA_Verify_PublicKey(BOOL bRometPublicKey,	unsigned char pbMsg[],DWORD ulMsgLen,
								unsigned char pSignature[],DWORD ulSignatureLen,
								CString &Info);

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	//���ܣ�RSA����(ʹ�ù�ԿPublicKey)
	//���룺BOOL bRometPublickey,     (TRUE:��ȡsubjec="Romote RSA Public Key"�Ĺ�Կ��
	//                                  FALSE����һ����Կ)
	//      unsigned char pbMsg[]       ��ǩ����Ϣ
	//      DWORD ulMsgLen              ����
	//�����pCipherBuffer[]             ������Ϣ
	//      DWORD &ulCipherLen          ����
	//      CString &info               ������Ϣ
	//���أ�BOOL              
	BOOL RSA_Encrypt(BOOL bRometePublicKey,unsigned char pbMsg[],DWORD ulMsgLen,
								unsigned char pCipherBuffer[],DWORD &ulCipherLen,
								CString &Info);

	//���ܣ�RSA����(ʹ��˽Կ)
	//���룺pCipherBuffer[]              ������Ϣ
	//      DWORD ulCipherLen            ����     
	//�����unsigned char pbMsg[]        ���ܺ���Ϣ
	//      DWORD &ulMsgLen              ����
	//      CString &info                ������Ϣ
	//���أ�BOOL              
	BOOL CUK_Et199Class::RSA_Decrypt(unsigned char pCipherBuffer[],DWORD ulCipherLen,
								 unsigned char pbMsg[],DWORD &ulMsgLen,								
								 CString &Info);

	////////////////////////////////////////////////////
	//���ܣ��豸�ײ��ʽ��
	//���룺CString strTokenName   �豸��
	//      CString strSOPin       ����ԱPIN
	//      CString strUserPin     �û�PIN
	//�����CString &info          ������Ϣ
	//���أ�BOOL                   ״̬
	BOOL FormatDev(CString strTokenName,CString strSOPin,CString strUserPin,CString &info);
	
	BOOL TestHardDev(CString &Info);	
	
	////////////////////////////////////////////////////
	//���ܣ������ݱ��浽ָ�����򣨷�USBKey��
	//���룺CString label               ��Ϣ��label
	//      BOOL bPrivate               �Ƿ��ŵ�������
	//      unsigned char databuffer[]  ��Ϣ
	//      DWORD len                   ����
	//�����CString &info               ������Ϣ
	//���أ�BOOL                        ״̬
	BOOL SaveUSBData(CString label,BOOL bPrivate,unsigned char databuffer[],DWORD len,CString &Info);
	
	BOOL SaveRometePublicKey(unsigned char keybuffer[],DWORD keylen,CString &info);
	//BOOL SavePublicKey(CString label,unsigned char databuffer[],DWORD len,CString &Info);

	////////////////////////////////////////////////////
	//���ܣ���ָ�����򣨷�USBKey����ѯָ��label������
	//���룺CString label               ��Ϣ��label
	//      BOOL bPrivate               �Ƿ�����
	//      DWORD MaxBufferlen          ��Ϣ��󳤶�	
	//�����unsigned char databuffer[]  ��ѯ������Ϣ
	//      CString &info               ������Ϣ
	//���أ�DWORD                       ��ѯ�������ݳ���
	DWORD GetData(CString label,BOOL bPrivate,unsigned char databuffer[],DWORD MaxBufferlen,CString &Info);
	
	////////////////////////////////////////////////////
	//���ܣ���ѯ��ԿUSBKey������
	//���룺BOOL bRometPublickey,     (TRUE:��ȡsubjec="Romete RSA Public Key"�Ĺ�Կ��
	//                                  FALSE����һ����Կ)
	//      DWORD MaxBufferlen          ��Ϣ��󳤶�	
	//�����unsigned char databuffer[]  ��ѯ������Ϣ
	//      CString &info               ������Ϣ
	//���أ�DWORD                       ��ѯ���Ĺ�Կ���ݳ���
	DWORD GetPublicKey(BOOL bRometPublickey,unsigned char databuffer[],DWORD MaxBufferlen,CString &Info);
	
	////////////////////////////////////////////////////
	//���ܣ�����RSA��׼�ķǶԳ���Կ�ԣ������豸
	//���룺	
	//�����CString &info               ������Ϣ
	//���أ�BOOL                      �������
	BOOL MakeKeypairgen(CString &info);
	CUK_Et199Class();
	virtual ~CUK_Et199Class();
protected:
	////////////////////////////////////////////////////
	//���ܣ�����������ѯָ��label�ľ������USBKey��
	//���룺CString label               ��Ϣ��label
	//      BOOL bPrivate               �Ƿ����ڱ�����
	//�����CK_OBJECT_HANDLE &hObject   ���ݾ��
	//      CString &info               ������Ϣ
	//���أ�BOOL                        ״̬
	BOOL GetDataObject(CString label,BOOL bPrivate,CK_OBJECT_HANDLE &hObject,CString &Info);

	////////////////////////////////////////////////////
	//���ܣ���USBKey����ѯָ����Կ�ľ����USBKey��
	//���룺BOOL bRometPublickey,     (TRUE:��ȡsubjec="Romete RSA Public Key"�Ĺ�Կ��
	//                                  FALSE����һ����Կ)
	//      BOOL bPrivate               �Ƿ����ڱ�������˽Կ��
	//�����CK_OBJECT_HANDLE &hObject   ��Կ���
	//      CString &info               ������Ϣ
	//���أ�BOOL                        ״̬
	BOOL GetUsbKeyObject(BOOL bRometPublickey,BOOL bPrivate,CK_OBJECT_HANDLE &hObject,CString &Info);
};



