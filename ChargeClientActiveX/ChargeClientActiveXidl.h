

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Apr 07 00:51:08 2013
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __ChargeClientActiveXidl_h__
#define __ChargeClientActiveXidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DChargeClientActiveX_FWD_DEFINED__
#define ___DChargeClientActiveX_FWD_DEFINED__
typedef interface _DChargeClientActiveX _DChargeClientActiveX;
#endif 	/* ___DChargeClientActiveX_FWD_DEFINED__ */


#ifndef ___DChargeClientActiveXEvents_FWD_DEFINED__
#define ___DChargeClientActiveXEvents_FWD_DEFINED__
typedef interface _DChargeClientActiveXEvents _DChargeClientActiveXEvents;
#endif 	/* ___DChargeClientActiveXEvents_FWD_DEFINED__ */


#ifndef __ChargeClientActiveX_FWD_DEFINED__
#define __ChargeClientActiveX_FWD_DEFINED__

#ifdef __cplusplus
typedef class ChargeClientActiveX ChargeClientActiveX;
#else
typedef struct ChargeClientActiveX ChargeClientActiveX;
#endif /* __cplusplus */

#endif 	/* __ChargeClientActiveX_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __ChargeClientActiveXLib_LIBRARY_DEFINED__
#define __ChargeClientActiveXLib_LIBRARY_DEFINED__

/* library ChargeClientActiveXLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_ChargeClientActiveXLib;

#ifndef ___DChargeClientActiveX_DISPINTERFACE_DEFINED__
#define ___DChargeClientActiveX_DISPINTERFACE_DEFINED__

/* dispinterface _DChargeClientActiveX */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DChargeClientActiveX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F5B4C55F-AE4F-4F6B-90E6-6FF861004934")
    _DChargeClientActiveX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DChargeClientActiveXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DChargeClientActiveX * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DChargeClientActiveX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DChargeClientActiveX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DChargeClientActiveX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DChargeClientActiveX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DChargeClientActiveX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DChargeClientActiveX * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DChargeClientActiveXVtbl;

    interface _DChargeClientActiveX
    {
        CONST_VTBL struct _DChargeClientActiveXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DChargeClientActiveX_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DChargeClientActiveX_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DChargeClientActiveX_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DChargeClientActiveX_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DChargeClientActiveX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DChargeClientActiveX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DChargeClientActiveX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DChargeClientActiveX_DISPINTERFACE_DEFINED__ */


#ifndef ___DChargeClientActiveXEvents_DISPINTERFACE_DEFINED__
#define ___DChargeClientActiveXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DChargeClientActiveXEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DChargeClientActiveXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1893A798-6B36-4A5D-B1EE-FFC0A5E756A9")
    _DChargeClientActiveXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DChargeClientActiveXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DChargeClientActiveXEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DChargeClientActiveXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DChargeClientActiveXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DChargeClientActiveXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DChargeClientActiveXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DChargeClientActiveXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DChargeClientActiveXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DChargeClientActiveXEventsVtbl;

    interface _DChargeClientActiveXEvents
    {
        CONST_VTBL struct _DChargeClientActiveXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DChargeClientActiveXEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DChargeClientActiveXEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DChargeClientActiveXEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DChargeClientActiveXEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DChargeClientActiveXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DChargeClientActiveXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DChargeClientActiveXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DChargeClientActiveXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ChargeClientActiveX;

#ifdef __cplusplus

class DECLSPEC_UUID("40A92D87-2FD0-40F7-95B0-A3EE1EE8B7B6")
ChargeClientActiveX;
#endif
#endif /* __ChargeClientActiveXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


