#pragma once

//#include "Include/pkcs11/cryptoki.h"
//#include "Include/pkcs11/wincrypt.h"
#include <cryptoki_ext.h>
#include <auxiliary.h>


const CK_ULONG MODULUS_BIT_LENGTH = 1024;	// should be  512/1024/2048

// CUK_Et199Class ����Ŀ��
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
	BOOL RSA_Signed(CK_BYTE_PTR pbMsg, CK_ULONG ulMsgLen,
		CK_BYTE_PTR pSignature, CK_ULONG_PTR ulSignatureLen,
		CString &Info);
	
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
	BOOL RSA_Digest(CK_BYTE_PTR msg, CK_ULONG msg_len,
		CK_BYTE_PTR msg_sha1, CK_ULONG msg_sha1_len,
		CString &info);
	////////////////////////////////////////////////////
	//���ܣ�RSAǩ����֤(ʹ�ù�ԿPublicKey)
	//���룺BOOL bRemotePublicKey,     (TRUE:��ȡsubjec="Romete RSA Public Key"�Ĺ�Կ��
	//                                  FALSE����һ����Կ)
	//      unsigned char pbMsg[]       ��ǩ����Ϣ
	//      DWORD ulMsgLen              ����
	//�����pSignature[]                ǩ��������Ϣ
	//      DWORD &ulSignatureLen       ����
	//      CString &info               ������Ϣ
	//���أ�BOOL                        ״̬
	BOOL RSA_Verify(BOOL bRemotePublicKey, 
		CK_BYTE_PTR pbMsg, CK_ULONG ulMsgLen,
		CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen,
		CString &info);

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	//���ܣ�RSA����(ʹ�ù�ԿPublicKey)
	//���룺BOOL bRemotePublicKey,     (TRUE:��ȡsubjec="Romote RSA Public Key"�Ĺ�Կ��
	//                                  FALSE����һ����Կ)
	//      unsigned char pbMsg[]       ��ǩ����Ϣ
	//      DWORD ulMsgLen              ����
	//�����pCipherBuffer[]             ������Ϣ
	//      DWORD &ulCipherLen          ����
	//      CString &info               ������Ϣ
	//���أ�BOOL              
	BOOL RSA_Encrypt(BOOL bRemotePublicKey,
		CK_BYTE_PTR pbMsg, CK_ULONG ulMsgLen,
		CK_BYTE_PTR pCipherBuffer, CK_ULONG_PTR ulCipherLen,
		CString &info);

	//���ܣ�RSA����(ʹ��˽Կ)
	//���룺pCipherBuffer[]              ������Ϣ
	//      DWORD ulCipherLen            ����     
	//�����unsigned char pbMsg[]        ���ܺ���Ϣ
	//      DWORD &ulMsgLen              ����
	//      CString &info                ������Ϣ
	//���أ�BOOL              
	BOOL RSA_Decrypt(CK_BYTE_PTR pCipherBuffer, CK_ULONG ulCipherLen,
								 CK_BYTE_PTR pbMsg, CK_ULONG_PTR ulMsgLen,								
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
	
	BOOL SaveRemotePublicKey(unsigned char keybuffer[],DWORD keylen,CString &info);
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
	//���룺BOOL bRemotePublicKey,     (TRUE:��ȡsubjec="Romete RSA Public Key"�Ĺ�Կ��
	//                                  FALSE����һ����Կ)
	//      DWORD MaxBufferlen          ��Ϣ��󳤶�	
	//�����unsigned char databuffer[]  ��ѯ������Ϣ
	//      CString &info               ������Ϣ
	//���أ�DWORD                       ��ѯ���Ĺ�Կ���ݳ���
	DWORD GetPublicKey(BOOL bRemotePublicKey,unsigned char databuffer[],DWORD MaxBufferlen,CString &Info);
	
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
	//���룺BOOL bRemotePublicKey,     (TRUE:��ȡsubjec="Romete RSA Public Key"�Ĺ�Կ��
	//                                  FALSE����һ����Կ)
	//      BOOL bPrivate               �Ƿ����ڱ�������˽Կ��
	//�����CK_OBJECT_HANDLE &hObject   ��Կ���
	//      CString &info               ������Ϣ
	//���أ�BOOL                        ״̬
	BOOL CUK_Et199Class::GetUsbKeyObject(int ukey_object_type, CK_OBJECT_HANDLE &hObject, CString &Info);
	//BOOL GetUsbKeyObject(BOOL bRemotePublicKey,BOOL bPrivate,CK_OBJECT_HANDLE &hObject,CString &Info);
};



