

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Apr 07 16:51:47 2013
 */
/* Compiler settings for .\ChargeClientActiveX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_ChargeClientActiveXLib,0x3351A0BA,0xCB0A,0x421F,0xB4,0x2A,0xF8,0xA2,0xBB,0x8B,0xD4,0xA3);


MIDL_DEFINE_GUID(IID, DIID__DChargeClientActiveX,0xF5B4C55F,0xAE4F,0x4F6B,0x90,0xE6,0x6F,0xF8,0x61,0x00,0x49,0x34);


MIDL_DEFINE_GUID(IID, DIID__DChargeClientActiveXEvents,0x1893A798,0x6B36,0x4A5D,0xB1,0xEE,0xFF,0xC0,0xA5,0xE7,0x56,0xA9);


MIDL_DEFINE_GUID(CLSID, CLSID_ChargeClientActiveX,0x40A92D87,0x2FD0,0x40F7,0x95,0xB0,0xA3,0xEE,0x1E,0xE8,0xB7,0xB6);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



